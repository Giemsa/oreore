#ifndef __OREORE_COCOS2D_ASSETLOADER_H__
#define __OREORE_COCOS2D_ASSETLOADER_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class AssetLoaderWorker;
    typedef void (cocos2d::CCObject::*SEL_AssetLoaded)(const std::string &filename, const int count, const int total);
#define assetloaded_selector(_SELECTOR) (SEL_AssetLoaded)(&_SELECTOR)

    struct AssetData
    {
        enum AssetType
        {
            SpriteFrame,
            Texture
        };

        AssetType type;
        std::string filename;

        AssetData(const std::string &filename, const AssetType type)
            : type(type), filename(filename) { }
        AssetData() : filename(""), type(Texture) { }
    };


    class AssetLoaderWorkerChild : public cocos2d::CCObject
    {
        friend class AssetLoaderWorker;
    private:
        AssetLoaderWorker *worker;
        AssetData data;

        AssetLoaderWorkerChild(AssetLoaderWorker *worker, const AssetData &data);
        AssetLoaderWorkerChild(const AssetLoaderWorkerChild& rhs) { }
        ~AssetLoaderWorkerChild() { CCLOG("AssetLoaderWorkerChild work finished!"); }
        void operator=(const AssetLoaderWorkerChild& rhs) { }

        void assetLoaded(cocos2d::CCObject *object);
    public:
    };


    class AssetLoaderWorker : public cocos2d::CCObject
    {
        friend class AssetLoader;
        friend class AssetLoaderWorkerChild;
        typedef std::vector<AssetData> AssetList;
    private:
        int count;
        cocos2d::CCObject *target;
        cocos2d::SEL_CallFunc callback;
        SEL_AssetLoaded assetLoadedCallback;
        AssetList assets;

        AssetLoaderWorker() : count(0) { }
        AssetLoaderWorker(const AssetLoaderWorker& rhs) { }
        ~AssetLoaderWorker() { CCLOG("AssetLoaderWorker work finished!"); }
        void operator=(const AssetLoaderWorker& rhs) { }

        void assetLoaded(const AssetData &data, cocos2d::CCObject *object);
        void load(cocos2d::CCObject *target, const cocos2d::SEL_CallFunc callback, const SEL_AssetLoaded assetLoadedCallback);
        void addAsset(const AssetData &data);
    public:
    };


    class AssetLoader
    {
    private:
        AssetLoaderWorker *worker;
    public:
        AssetLoader();
        void addTexture(const std::string &filename);
        void addSpriteFrame(const std::string &filename);
        void load(cocos2d::CCObject *target, const cocos2d::SEL_CallFunc callback, const SEL_AssetLoaded assetLoadedCallback = null);
    };
}


#endif
