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
        public:
            using TutorialID = T;
            using DataConnectorID = D;

            /* チュートリアルベース */
            class TutorialBase : public detail::TutorialBaseBase
            {
                template<typename U>
                friend class detail::TutorialPlayInfo;
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
            DataConnector<D> dataConnector;
            SequenceList seqList;
            detail::TutorialPlayInfoBase *instantiator;

            TutorialManager()
            { }

            ~TutorialManager()
            { }

            TutorialSequence &add(const T trigger)
            {
                return seqList.insert(
                    std::make_pair(static_cast<int>(trigger), TutorialSequence(instantiator))
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

            template<typename V>
            void loadTutorial()
            {
                static detail::TutorialPlayInfo<V> instance;
                instantiator = &instance;
                V::registerPhase();
                instantiator = nullptr;
            }

            DataConnector<D> &getDataConnector() { return dataConnector; }
            const DataConnector<D> &getDataConnector() const { return dataConnector; }
        };
    }
}

#endif
