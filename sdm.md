---
layout: default
---
# Simple Debug Menu

## 概要
　シンプルなデバッグメニューを提供します。  
　現在cocos2dx 2.2.2版のみ提供されています。

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。

### Debuggerクラス
　SceneManagerのgetDebuggerメソッドで取得可能です。このDebuggerクラスに対してaddMenuItemを呼び出すと、メニューを追加することができます。

	void callback(Dbugger *debugger)
	{
		CCLOG("callback!");
	}

	Debbuger *dbg = SceneManager::getInstance().getDebugger();
	dbg->addItem("メニュー", callback);

### メニューの起動
　アプリ画面の右上に「俺々」ボタンが表示されるので、それをタップするとメニューが表示されます。もう一度アイコンをタップするとメニューを閉じます。

### SceneManager
　Simple Debug Menuは、[SceneManager](./scenemanager.html)上で動作させることを前提に設計されていますが、SceneManagerを利用しなくても、Simple Debug Menu単体で利用することもできます。