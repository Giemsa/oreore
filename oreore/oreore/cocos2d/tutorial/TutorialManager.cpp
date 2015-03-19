#include "TutorialManager.h"
#include "../utils/async.h"
#include "../../utils/step/storage/FileStorage.h"

namespace oreore
{
    namespace Tutorial
    {
        using namespace cocos2d;

        namespace detail
        {
            bool TutorialManagerBase::load()
            {
                using namespace oreore;

                if(!FileUtils::getInstance()->isFileExist(fileName))
                {
                    CCLOG("tutorial progress data file not found.");
                    resetAll();
                    return false;
                }

                const bool r = Step::FileStorage(fileName) >> Step::Serializer(*this);
                if(!r)
                {
                    resetAll();
                }

                return r;
            }

            void TutorialManagerBase::loadAsync()
            {
                using namespace oreore;

                if(!FileUtils::getInstance()->isFileExist(fileName))
                {
                    CCLOG("tutorial progress data file not found.");
                    resetAll();
                    return;
                }

                AsyncTask task(
                    [this]() -> bool {
                        const bool r = Step::FileStorage(fileName) >> Step::Serializer(*this);
                        if(!r)
                        {
                            resetAll();
                        }

                        return true;
                    },
                    [this](const bool result) {
                        // このresultは今のところ確実にtrue
                        if(!result)
                        {
                            CCLOG("failed to load tutorial progress data");

                            resetAll();
                        }
                    }
                );
            }

            bool TutorialManagerBase::save()
            {
                using namespace oreore;

                const bool r = Step::Serializer(*this) >> Step::FileStorage(fileName);
                if(!r)
                {
                    CCLOG("failed to save tutorial progress data");
                }

                return r;
            }

            void TutorialManagerBase::saveAsync(const std::function<void(const bool)> &callback)
            {
                using namespace oreore;

                AsyncTask task(
                    [this]() -> bool {
                        return Step::Serializer(*this) >> Step::FileStorage(fileName);
                    },
                    [this, callback](const bool result) {
                        if(!result)
                        {
                            CCLOG("failed to save tutorial progress data");
                        }

                        if(callback)
                        {
                            callback(result);
                        }
                    }
                );
            }

            // 進捗データをクリアして保存
            void TutorialManagerBase::resetAll()
            {
                saveAsync([](const bool) {
                    CCLOG("save complete");
                });
            }
        }
    }

}
