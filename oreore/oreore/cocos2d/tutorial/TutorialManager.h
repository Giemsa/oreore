#ifndef __OREORE_TUTORIAL_TUTORIALMANAGER_H__
#define __OREORE_TUTORIAL_TUTORIALMANAGER_H__

#include <unordered_map>
#include "TutorialBase.h"
#include "DataConnector.h"
#include "TutorialPhase.h"
#include "../../utils/step/serializer/JSON.h"

namespace oreore
{
    namespace Tutorial
    {
        namespace detail
        {
            class TutorialManagerBase : public oreore::Step::JSON
            {
            private:
                std::string fileName;
            protected:
                void resetAll();
            public:
                TutorialManagerBase()
                {
                    fileName = cocos2d::FileUtils::getInstance()->getWritablePath() + "tutorial.json";
                }

                virtual ~TutorialManagerBase()
                { }

                void load();
                void save(const std::function<void(const bool)> &callback);

                void setFileName(const std::string &name) { fileName = fileName; }
                const std::string &getFileName() const { return fileName; }
            };
        }

        template<typename T, typename D>
        class TutorialManager final : public detail::TutorialManagerBase
        {
            friend class TutorialBase;
            friend class detail::TutorialBaseBase;
        public:
            using TutorialID = T;
            using DataConnectorID = D;
            using DataConnector = oreore::Tutorial::DataConnector<D>;

            /* チュートリアルベース */
            class TutorialBase : public detail::TutorialBaseBase
            {
                template<typename S, typename E>
                friend class TutorialManager;
            private:
                bool trigger(const T id)
                {
                    bool played = true;
                    size_t index = 0;
                    for(PhaseList::iterator it = phaseList.begin(); it != phaseList.end(); ++it, ++index)
                    {
                        if(it->isPlayed())
                        {
                            continue;
                        }

                        if(it->getTrigger() == static_cast<int>(id))
                        {
                            if(played || it->getType() == PhaseType::Begin)
                            {
                                return it->proceed(index);
                            }
                        }

                        played = false;
                    }

                    return false;
                }


                bool canPlay(const T id) const
                {
                    bool played = true;
                    for(auto &phase : phaseList)
                    {
                        if(phase.isPlayed())
                        {
                            continue;
                        }

                        if(phase.getTrigger() == static_cast<int>(id))
                        {
                            if(played || phase.getType() == PhaseType::Begin)
                            {
                                return true;
                            }
                        }

                        played = false;
                    }

                    return false;
                }

                void removeTutorial() override
                {
                    getInstance().removeTutorial(this);
                }
            protected:
                template<typename U>
                U &getData(const D id)
                {
                    return getInstance().dataConnector.template get<U>(id);
                }

                template<typename U>
                const U &getData(const D id) const
                {
                    return getInstance().dataConnector.template get<U>(id);
                }

                TutorialPhase phase(const T trigger, const TutorialPhase::func_type &func, const PhaseType type)
                {
                    return TutorialPhase(static_cast<int>(trigger), func, type, this);
                }

                TutorialPhase phase(const T trigger, const TutorialPhase::func_type &func)
                {
                    return TutorialPhase(static_cast<int>(trigger), func, this);
                }

                TutorialPhase phase(const TutorialPhase::func_type &func)
                {
                    return TutorialPhase(func, this);
                }

                void addSequence(const std::initializer_list<TutorialPhase> seq)
                {
                    for(auto &phase : seq)
                    {
                        phaseList.push_back(phase);
                        if(phaseList.size() == 1)
                        {
                            phaseList.back().setType(PhaseType::Begin);
                        }
                    }
                }
            public:
                TutorialBase(const char *name)
                : detail::TutorialBaseBase(name)
                { }

                virtual ~TutorialBase() { }
            };

        private:
            using TutorialList = std::vector<TutorialBase *>;

            DataConnector dataConnector;
            TutorialList tutorialList;
            detail::TutorialBaseBase *tutorial;

            TutorialManager()
            : tutorial(nullptr)
            { }

            ~TutorialManager()
            {
                for(auto *t : tutorialList)
                {
                    t->release();
                }
            }

            void removeTutorial(TutorialBase *tutorial)
            {
                tutorial->release();
                tutorialList.erase(
                    std::remove(tutorialList.begin(), tutorialList.end(), tutorial),
                    tutorialList.end()
                );
            }

            bool serialize(picojson::value &out) const override
            {
                picojson::array list;

                bool r = true;
                for(auto *t : tutorialList)
                {
                    r &= t->saveTutorial(list);
                }

                // シリアライズに失敗してたら保存しない
                if(r)
                {
                    out = picojson::value(list);
                }

                return r;
            }

            bool deserialize(const picojson::value &data, const std::string &error) override
            {
                using namespace oreore;

                if(!error.empty())
                {
                    return false;
                }

                if(!data.is<picojson::array>())
                {
                    return false;
                }

                const picojson::array &list = data.get<picojson::array>();
                if(list.size() != tutorialList.size())
                {
                    return false;
                }

                bool r = true;
                typename TutorialList::iterator it = tutorialList.begin();
                for(auto &e : list)
                {
                    r &= (*it++)->loadTutorial(e);
                }

                return r;
            }
        public:
            static TutorialManager &getInstance()
            {
                static TutorialManager tm;
                return tm;
            }

            /* 再生可能であればチュートリアルを再生する */
            bool trigger(const T id)
            {
                for(auto *t : tutorialList)
                {
                    if(t->trigger(id))
                    {
                        return true;
                    }
                }

                return false;
            }

            /* 再生可能か */
            bool canPlay(const T id) const
            {
                for(auto *t : tutorialList)
                {
                    if(t->canPlay(id))
                    {
                        return true;
                    }
                }

                return false;
            }

            template<typename U>
            void loadTutorial()
            {
                U *t = new U();
                if(t && t->init())
                {
                    t->autorelease();
                    t->retain();
                    tutorial = t;
                    t->registerPhase();
                    tutorial = nullptr;
                    tutorialList.push_back(t);
                    return;
                }

                delete t;
            }

            DataConnector &getDataConnector() { return dataConnector; }
            const DataConnector &getDataConnector() const { return dataConnector; }
        };
    }
}

#endif
