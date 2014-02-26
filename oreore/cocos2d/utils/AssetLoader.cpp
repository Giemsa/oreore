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
            CCObject *info = CCDictionary::createWithContentsOfFile(data.filename.c_str())->objectForKey("metadata");
            info = static_cast<CCDictionary *>(info)->objectForKey("textureFileName");
            filename = static_cast<CCString *>(info)->getCString();
        }
        else
            filename = data.filename;
        CCTextureCache::sharedTextureCache()->addImageAsync(filename.c_str(), this, callfuncO_selector(AssetLoaderWorkerChild::assetLoaded));
    }
    
    void AssetLoaderWorkerChild::assetLoaded(cocos2d::CCObject *object)
    {
        worker->assetLoaded(data, object);
        delete this;
    }

    /* AssetLoaderWorker */
    void AssetLoaderWorker::addAsset(const AssetData &data)
    {
        assets.push_back(data);
    }

    void AssetLoaderWorker::load(CCObject *target, const SEL_CallFunc callback, const SEL_AssetLoaded assetLoadedCallback)
    {
        this->target = target;
        this->callback = callback;
        this->assetLoadedCallback = assetLoadedCallback;

        for(AssetList::iterator it = assets.begin(); it != assets.end(); ++it)
            new AssetLoaderWorkerChild(this, *it);
    }

    void AssetLoaderWorker::assetLoaded(const AssetData &data, CCObject *object)
    {
        if(data.type == AssetData::SpriteFrame)
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(data.filename.c_str());
    
        if(assetLoadedCallback)
            (target->*assetLoadedCallback)(data.filename, count, assets.size());

        count++;
        if(count == assets.size())
        {
            (target->*callback)();
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

    void AssetLoader::load(CCObject *target, const SEL_CallFunc callback, const SEL_AssetLoaded assetLoadedCallback)
    {
        worker->load(target, callback, assetLoadedCallback);
    }
}