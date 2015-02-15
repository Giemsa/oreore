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

            using PhaseList = std::unordered_multimap<int, TutorialPhase>;
            using TutorialList = std::vector<detail::TutorialBaseBase *>;
        public:
            using TutorialID = T;
            using DataConnectorID = D;
            using DataConnector = oreore::Tutorial::DataConnector<D>;

            /* チュートリアルベース */
            class TutorialBase : public detail::TutorialBaseBase
            {
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
                    for(auto &p : seq)
                    {
                        getInstance().add(p);
                    }
                }
            public:
                TutorialBase() { }
                virtual ~TutorialBase() { }
            };

        private:
            DataConnector dataConnector;
            PhaseList phaseList;
            TutorialList tutorialList;
            detail::TutorialBaseBase *tutorial;

            TutorialManager()
            : tutorial(nullptr)
            { }

            ~TutorialManager()
            { }

            void add(const TutorialPhase &phase)
            {
                const size_t idx = phaseList.size();
                TutorialPhase &p = phaseList.insert(
                    std::make_pair(phase.getTrigger(), phase)
                )->second;
                p.setIndex(idx);
                p.setType(idx == 0 ? PhaseType::Begin : PhaseType::Stay);
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
                const std::pair<
                    typename PhaseList::iterator,
                    typename PhaseList::iterator
                > &r = phaseList.equal_range(static_cast<int>(id));

                // 未再生シーケンスのうち、最もIDの若いものを再生
                size_t m = std::numeric_limits<size_t>::max();
                TutorialPhase *phase = nullptr;
                for(typename PhaseList::iterator it = r.first; it != r.second; ++it)
                {
                    TutorialPhase &p = it->second;
                    if(!p.isPlayed() && p.getIndex() < m)
                    {
                        m = p.getIndex();
                        phase = &p;
                    }
                }

                if(!phase)
                {
                    return false;
                }

                return phase->proceed();
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
