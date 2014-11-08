#ifndef __OREORE_COCOS2D_ASSETLOADER_H__
#define __OREORE_COCOS2D_ASSETLOADER_H__

#include <functional>
#include "cocos2d.h"

namespace oreore
{
    class AssetLoaderWorker;
    typedef void (cocos2d::CCObject::*SEL_AssetLoaded)(const std::string &filename, const std::size_t count, const std::size_t total);
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


    class AssetLoaderWorkerChild : public cocos2d::Ref
    {
        friend class AssetLoaderWorker;
    private:
        AssetLoaderWorker *worker;
        AssetData data;

        AssetLoaderWorkerChild(AssetLoaderWorker *worker, const AssetData &data);
        AssetLoaderWorkerChild(const AssetLoaderWorkerChild &rhs) { }
        ~AssetLoaderWorkerChild() { CCLOG("AssetLoaderWorkerChild work finished!"); }
        void operator=(const AssetLoaderWorkerChild &rhs) { }
   };


    class AssetLoaderWorker : public cocos2d::Ref
    {
        friend class AssetLoader;
        friend class AssetLoaderWorkerChild;
        typedef std::vector<AssetData> AssetList;
    private:
        std::size_t count;
        std::function<void(const std::string &filename, const std::size_t count, const std::size_t total)> assetLoadedCallback;
        std::function<void()> callback;
        AssetList assets;

        AssetLoaderWorker() : count(0) { }
        AssetLoaderWorker(const AssetLoaderWorker &rhs) { }
        ~AssetLoaderWorker() { CCLOG("AssetLoaderWorker work finished!"); }
        void operator=(const AssetLoaderWorker &rhs) { }

        void assetLoaded(const AssetData &data, cocos2d::Texture2D *tex);
        void load(
            const std::function<void()> &callback,
            const std::function<void(const std::string &filename, const std::size_t count, const std::size_t total)> &assetLoadedCallback
        );
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
        void load(
            const std::function<void()> &callback,
            const std::function<void(const std::string &filename, const std::size_t count, const std::size_t total)> &assetLoadedCallback = nullptr
        );
    };
}

#endif

