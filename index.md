---
layout: default
---
# 概要
　"Oreore/俺々"は、俺がゲームを開発するときに得するユーティリティライブラリです。  
cocos2dx 3.0向けの汎用的な処理群をまとめました。

# 動作環境
* **俺々 for cocos2dx 2.2.2**
	* C++コンパイラ
	* cocos2dx 2.2.2
	* **現在メンテナンスされていません。**
* **俺々 for cocos2dx 3.0**
	* C++11がビルドできる環境
	* cocos2dx 3.0安定版

# 使い方

## cocos2dxを利用する場合
　oreoreをcloneします。

	git clone https://github.com/Giemsa/oreore.git

　iOSの場合、`oreore/proj.ios/oreore.xcodeproj`をサブプロジェクトとして追加してください (exampleフォルダ内にサンプルがあります)。

　Androidの場合、ゲームプロジェクトの`proj.android/jni/Android.mk`に、`oreore/proj.android/Android.mk`をインポートするように記述します。

	LOCAL_WHOLE_STATIC_LIBRARIES += oreore_static
	$(call import-module,../../../oreore/proj.android)

　上記の2行が必要です。import-moduleでは、ゲームプロジェクトの`proj.andorid`から`oreore/proj.android/Android.mk`への相対パスを指定します。詳しくは`example/proj.android`を参照してください。

## cocos2dxを利用しない場合
　マクロ「NOT_USE_COCOS2DX」が有効になっている場合、cocos2dxに関連する機能の読み込みが無効化されます。が、殆どの機能が無効化されるのであまり意味は無いです。

## サンプルのビルド
　`example`フォルダにAndroidとiOSのサンプルがそれぞれ入っています。サンプルと言ってもビルドが通るだけのシンプルなものです。これらのプロジェクトを参考に、oreoreプロジェクトを導入してください。

## ブランチ
　俺々は複数のバージョンのcocos2dxに対応するため、cocos2dxのバージョンごとに異なるブランチを利用しています。

* ococos2dx-2.2.2  
	　cocos2dx 2.2.2向けのブランチです。作者が2.x系を利用しなくなったため、このブランチの更新は停止しています。

* cocos2dx 3.0  
	　cocos2dx 3.0安定版向けのブランチです。このブランチが最新です。
* その他のブランチ
	　cocos2dxの過去のバージョンで利用されたoreoreのブランチです。更新は停止しています。

# 構成
　俺々は、以下のライブラリ群を含んでいます。

* common utility
* cocos2dx Action library
* cocos2dx Node library
* Fluxion (cocos2dx Action Generator)

## common utility
cocos2dxにかかわらず利用できるユーティリティです。  
[common utility](./commonutility.html) を参照してください。

## SceneManager
cocos2dxでシーン管理を行うクラスです。  
[SceneManager](./scenemanager.html) を参照してください。

## cocos2dx Action library
cocos2dxで利用できるアクションのライブラリです。  
[Actions Reference](./actions.html) を参照してください。

## cocos2dx Node library
cocos2dxで利用できるノードやレイヤのライブラリです。

## Fluxion (cocos2dx Action Generator)
アクションを簡潔に記述、生成するためのDSLを提供します。  
[Fluxion Reference](./fluxion.html) を参照してください。

## Step (Savedata Serialization Utility)
簡潔にデータを保存するための機能を提供します。
[Step](./step.html) を参照してください。

## Simple Debug Menu
シンプルなデバッグメニューを提供します。  
[Simple Debug Menu](./sdm.html) を参照してください。

#ライセンス
This software is released under the zlib License, see LICENSE.txt.