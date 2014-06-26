#include "AssetLoader.h"


namespace oreore
{
    using namespace cocos2d;

    /* AssetLoaderWorkerChild */
    AssetLoaderWorkerChild::AssetLoaderWorkerChild(AssetLoaderWorker *worker, const AssetData &data)
        : worker(worker), data(data)
    {
        std::string filename;
        if(data.type == AssetData::SpriteFrame)
        {
            const ValueMap &dic = FileUtils::getInstance()->getValueMapFromFile(data.filename.c_str());
            filename = dic.at("metadata").asValueMap().at("textureFileName").asString();
        }
        else
            filename = data.filename;

        Director::getInstance()->getTextureCache()->addImageAsync(filename.c_str(), [this](Texture2D *tex) {
            this->worker->assetLoaded(this->data, tex);
            delete this;
        });
    }

    /* AssetLoaderWorker */
    void AssetLoaderWorker::addAsset(const AssetData &data)
    {
        assets.push_back(data);
    }

    void AssetLoaderWorker::load(const std::function<void()> &callback, const std::function<void(const std::string &filename, const int count, const int total)> &assetLoadedCallback)
    {
        this->callback = callback;
        this->assetLoadedCallback = assetLoadedCallback;

        for(AssetData &data : assets)
            new AssetLoaderWorkerChild(this, data);
    }

    void AssetLoaderWorker::assetLoaded(const AssetData &data, Texture2D *tex)
    {
        if(data.type == AssetData::SpriteFrame)
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(data.filename.c_str());

        if(assetLoadedCallback)
            assetLoadedCallback(data.filename, count, assets.size());

        count++;
        if(count == assets.size())
        {
            callback();
            delete this;
        }
    }


    /* AssetLoader */
    AssetLoader::AssetLoader() : worker(new AssetLoaderWorker()) { }

    void AssetLoader::addTexture(const std::string &filename)
    {
        worker->addAsset(AssetData(filename, AssetData::Texture));
    }

    void AssetLoader::addSpriteFrame(const std::string &filename)
    {
        worker->addAsset(AssetData(filename, AssetData::SpriteFrame));
    }

    void AssetLoader::load(const std::function<void()> &callback, const std::function<void(const std::string &filename, const int count, const int total)> &assetLoadedCallback)
    {
        worker->load(callback, assetLoadedCallback);
    }
}

