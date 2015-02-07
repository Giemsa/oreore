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
            bool TutorialManagerBase::deserialize(const picojson::value &data, const std::string &error)
            {
                using namespace oreore;

                if(!error.empty())
                {
                    return false;
                }

                if(!data.is<picojson::object>())
                {
                    return false;
                }

                return true;
            }

            bool TutorialManagerBase::serialize(picojson::value &out) const
            {
                using namespace oreore;

                picojson::object obj;

                return true;
            }

            void TutorialManagerBase::load()
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
                        if(!result)
                        {
                            CCLOG("failed to load tutorial progress data");

                            // 進捗データをクリアして保存
                            resetAll();
                            save();
                        }
                    }
                );
            }

            void TutorialManagerBase::save()
            {
                using namespace oreore;

                AsyncTask task(
                    [this]() -> bool {
                        return Step::Serializer(*this) >> Step::FileStorage(fileName);
                    },
                    [this](const bool result) {
                        if(!result)
                        {
                            CCLOG("failed to save tutorial progress data");
                        }
                    }
                );
            }

            void TutorialManagerBase::resetAll()
            {
            
            }
        }
    }

}
