#ifndef __OREORE_TUTORIAL_TUTORIALMANAGER_H__
#define __OREORE_TUTORIAL_TUTORIALMANAGER_H__

#include "TutorialBase.h"
#include "DataConnector.h"
#include <unordered_map>
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

            using SequenceList = std::unordered_map<int, TutorialSequence>;
            using TutorialList = std::vector<detail::TutorialBaseBase *>;
        public:
            using TutorialID = T;
            using DataConnectorID = D;
            using DataConnector = oreore::Tutorial::DataConnector<D>;

            /* チュートリアルベース */
            class TutorialBase : public detail::TutorialBaseBase
            {
            protected:
                static TutorialSequence &addTrigger(const T trigger)
                {
                    return getInstance().add(trigger);
                }

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
            public:
                TutorialBase() { }
                virtual ~TutorialBase() { }
            };

        private:
            DataConnector dataConnector;
            SequenceList seqList;
            TutorialList tutorialList;
            detail::TutorialBaseBase *tutorial;

            TutorialManager()
            : tutorial(nullptr)
            { }

            ~TutorialManager()
            { }

            TutorialSequence &add(const T trigger)
            {
                return seqList.insert(
                    std::make_pair(static_cast<int>(trigger), TutorialSequence(tutorial))
                ).first->second;
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
                const typename SequenceList::iterator it = seqList.find(static_cast<int>(id));
                if(it == seqList.end())
                {
                    return false;
                }

                return it->second.proceed();
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
