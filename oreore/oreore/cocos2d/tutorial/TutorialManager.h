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
                bool deserialize(const picojson::value &data, const std::string &error) override;
                bool serialize(picojson::value &out) const override;

                void resetAll();
            public:
                TutorialManagerBase()
                {
                    fileName = cocos2d::FileUtils::getInstance()->getWritablePath() + "tutorial.json";
                }

                virtual ~TutorialManagerBase()
                { }

                void load();
                void save();

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

                void removeTutorial() const override
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
                TutorialBase() { }
                virtual ~TutorialBase() { }
            };

        private:
            using TutorialList = std::vector<TutorialBase *>;

        private:
            DataConnector dataConnector;
            TutorialList tutorialList;
            detail::TutorialBaseBase *tutorial;

            TutorialManager()
            : tutorial(nullptr)
            { }

            ~TutorialManager()
            { }

            void removeTutorial(const TutorialBase *tutorial)
            {
                tutorialList.erase(
                    std::remove(tutorialList.begin(), tutorialList.end(), tutorial),
                    tutorialList.end()
                );
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
