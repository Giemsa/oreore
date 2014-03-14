---
layout: default
---
# SceneManager

## 概要
　シーン管理を行います。  
**あまりまじめにテストしていないので、現在使用非推奨です。**

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。

### SceneManagerクラス
　SceneManagerはシングルトンクラスです。getInstanceメソッドでインスタンスを取得できます。

	SceneManager &mgr = SceneManager.getInstance();


### シーンの定義
　oreore::SceneBaseを継承します。

	class SimpleScene : public SceneBase<SimpleScene> { };

　oreore::SceneBaseはテンプレートクラスです。テンプレート引数には自分のクラスを指定します(CRTP)。

### シーンの追加
　シーンをSceneManagerで管理するためには、上記で定義したクラスをSceneManagerに登録する必要があります。登録には、SceneManager::addSceneを使用します。

	mgr.addScene<SimpleScene>();

　addSceneの第一引数にtrueを指定すると、シーン生成が遅延化されます。遅延化されたシーンは、シーン使用時に自動的に生成され、シーンを抜ける際に自動で解放されます。

	mgr.addScene<SimpleScene>(true);

### シーン切り替え
　SceneManagerは、シーンをスタックで管理しています。以下の3つのメソッドでシーンを遷移させることが可能です。

* forwardScene\<T\>()  
	現在のシーンをスタックに積み、新しくTで指定されるシーンに遷移します。

		mgr.forward<NextScene>();

* backScene(bool dispose = false)  
	スタックからシーンをポップして、一つ前のシーンに遷移します。disposeがtrueの時、再生中のシーンを破棄します。

		mgr.back();

	cocos2dxと異なり、このメソッドを利用した場合でもトランジションが適用されます(CCDirector::popSceneではトランジションが有効になりません)。

* switchScene\<T\>(bool dispose = false)  
	スタックのトップのシーンを、Tで指定されるシーンに置き換えます。disposeがtrueの時、再生中のシーンを破棄します。

		mgr.switchScene<NextScene>();

### Transition
　シーン遷移時のトランジションは、SceneBaseクラス継承時に指定することができます。

	class SimpleScene : public SceneBase<SimpleScene>
	{
	private:
	public:
		CCTransitionScene *transition(CCScene *nextScene);
	};

	…...

	CCTransitionScene *SimpleScene::transition(CCScene *nextScene)
	{
		return CCTransitionFade::create(1.0f, nextScene);
	}

　ここで指定したトランジションは、**そのシーンから別のシーンへ切り替わる際に使用されます。**  
遷移時(switchScene等)にトランジションを指定する方法は現在提供されていません。cocos2dxの仕様上、綺麗に記述する方法がないためです。

### cococ2dxのシーンとして扱う
　SceneBase::getSceneメソッドでCCSceneを取得できます。

	CCScene *scene = mgr.getScene<SimpleScene>()->getScene();
	CCDirector::sharedDirector()->replaceScene(scene);

　このように、CCDirectorのメソッドを利用してシーン遷移が可能です。この方法を用いれば、シーン遷移時にトランジションを指定することが可能です。