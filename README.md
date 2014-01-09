# Oreore
## 概要
俺がゲーム開発するときに得するライブラリ。  
OTL(Oreore Template Library)とOreore cocos2dx Libraryをまとめたもの。

**ドキュメントは[Oreore/俺々 cocos2dxライブラリ](http://giemsa.github.io/oreore/)に移行します。**

##OTL
汎用的っぽい感じの処理をまとめたライブラリです。乱数とか、色変換とか。  
cocos2dxに限らず利用できます。USE_COCOS2DXマクロを定義するとcocos2dx向けにビルドします。  
定義しない場合は汎用的にビルドします。
### 使い方
#### TimeCounter.h
時間測るのに使おうと思ったけど使い道なかったから多分使うことないと思う。

#### Overlay.h
**OverlayLayer**: オーバーレイ周りのクラス。継承するとメニュー画面とかが簡単に作れます。  
onShowとonCloseをオーバーライドすると、それぞれオーバーレイの表示が完了した時、非表示が完了した時にそれらのメソッドが呼ばれます。  
showとcloseはオーバーライド可能です。表示開始前、非表示開始前になにかやりたいときはこれらのメソッドをオーバーライドしてください。オーバーライドしたらかならずOverlayLayerのshowとcloseを呼んでくだしあ。

	class KagekinaLayer : public cocos2d::OverlayLayer
    {
    private:
    protected:
        void onShow() override;
        void onClose() override;
    public:
        CREATE_FUNC(KagekinaLayer);

        KagekinaLayer() { }
        ~KagekinaLayer() { }
    
        bool init() override;
    };

こんな感じで。

#### Shake.h
**Shake**: 画面を揺らすアクション
#### Utils.h
ユーティリティのうち、cocos2dxで役に立ちそうなものをぶっこんだ

##Oreore cocos2dx Library

俺得フレームワーク。面倒なリソース管理とか全部やってくれる勢い。  
ビルドにはC++11に対応したコンパイラが必要です。たぶん。全部使うにはcocos2dx 3.0以降が必要です。
### 使い方
#### CommonGameBase::ScreenBase
シーン1枚に相当するクラス。このクラスを継承してシーンを作成します。

    class GameScreen : public CommonGameBase::ScreenBase
    {
    private:
    public:
        CONNECT_ID;
        CREATE_FUNC(GameScreen);

        GameScreen();
        virtual ~GameScreen();

        bool init() override;
    };

CONNECT_IDマクロとCREATE_FUNCマクロは必須です。また、initメソッド内で初期化を行います。  
**コンストラクタ内で初期化は行わないようにしてください。**oreoreが内部でメモリ管理を行っているので、コンストラクタ内で初期化すると死にます。

また、cpp側に

	INIT_BASE_SCREEN(GameScreen);
	
を記述してください。

#### CommonGameBase::GameManager
リソースの管理を行うクラスです。このクラスを継承してGameManagerを作成します。

	// Sprite Index
    enum class SpriteIndex
    {
        Count
    };

    class GameManager : public CommonGameBase::GameManager<GameManager, 2, SpriteIndex, SpriteIndex::Count>
    {
    private:
    public:
        GameManager();
        virtual ~GameManager();

        void init();
    };

CommonGameBase::GameManagerはテンプレートクラスです。それぞれ

 * 継承後のクラス名
 * 総シーン数
 * スプライト管理に使うインデックスのenum型
 * 総スプライト数(enumの最後にCountを付けておくとこのような書き方ができます。)

です。
initメソッド内でリソースの読み込みを行います。

    void GameManager::init()
    {
    	// スクリーンを生成して登録する
        GameScreen *screen = new GameScreen();
        registerScreen(screen);

		// スプライトレジストラを取得する。
        CommonGameBase::SpriteRegistrar<SpriteIndex> reg = 
        	loadSprite(screen, "ball.png");

        // テクスチャの登録
        reg.add(SpriteIndex::Ball);

		// メソッドチェーン的な使い方
        loadSprite(screen, "images.png")
        	.add(SpriteIndex::LifeBar, 0, 140, 320, 16)
        	.add(SpriteIndex::ItemLifeRecover, 0, 156, 32, 32);
       
		// 初期化(このタイミングで各シーンのinitメソッドが呼ばれます)
        initScreens();
    }

CommonGameBase::SpriteRegistrar::addメソッドは、引数にスプライトのインデックスを指定すると画像全体をテクスチャとして登録します。第二〜第五引数を指定すると、その範囲をテクスチャとして登録します。

##### GameManager::sprite(const E index)
登録されたテクスチャからスプライトを生成します。

	cocos2d::Sprite *sprite = GameManager::sprite(SpriteIndex::Ball);
	addChild(sprite);

##### GameManager::texture(const E index)
登録されたテクスチャを返します。

##### GameManager::rect(const E index)
登録されたテクスチャの矩形領域を返します。

	cocos2d::Sprite *sprite = cocos2d::Sprite::create();
	sprite->setTexture(GameManager::texture(SpriteIndex::Ball));
	sprite->setTextureRect(GameManager::rect(SpriteIndex::Ball));
	
通常はSpriteを継承して、init内でこのメソッドを使います。

	bool SugoiSprite::init()
	{
		if(Sprite::init())
		{
			setTexture(GameManager::texture(SpriteIndex::Ball));
			setTextureRect(GameManager::rect(SpriteIndex::Ball));
		}
		return false;
	}

#### その他
適当にどうぞ


##ライセンス
This software is released under the zlib License, see LICENSE.txt.
