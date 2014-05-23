---
layout: default
---
# 概要
　"Oreore/俺々"は、俺がゲームを開発するときに得するユーティリティライブラリです。  
cocos2dx 3.0系及び 2.2.2 向けの汎用的な処理群をまとめました。

# 動作環境
* **俺々 for cocos2dx 3.0**
	* C++11がビルドできる環境
	* cocos2dx 3.0 beta
* **俺々 for cocos2dx 2.2.2**
	* C++コンパイラ
	* cocos2dx 2.2.2
* **俺々 for cocos2dx 3.0 RC1**
	* C++11がビルドできる環境
	* cocos2dx 3.0 RC1

# 使い方

## cocos2dxを利用する場合
　oreoreをcloneします。

	git clone https://github.com/Giemsa/oreore.git

　oreoreフォルダ内の"oreore.h"をincludeしてください。  
また、oreoreフォルダ内の*.cppファイルをビルド対象に加えてください。

## cocos2dxを利用しない場合
　マクロ「NOT_USE_COCOS2DX」が有効になっている場合、cocos2dxに関連する機能の読み込みが無効化されます。が、殆どの機能が無効化されるのであまり意味は無いです。

## ブランチ
　俺々は複数のバージョンのcocos2dxに対応するため、cocos2dxのバージョンごとに異なるブランチを利用しています。

* ococos2dx-2.2.2  
	　cocos2dx 2.x系最新版の2.2.2向けのブランチです。おそらく最も更新が頻繁なブランチです。

* cocos2dx3.0-beta  
	　cocos2dx 3.x系最新版の3.0beta向けのブランチです。C++11の機能を利用します。  
	RC1への移行により、今後開発が停止します。

* cocos2dx 3.0 RC1
	　cocos2dx 3.0安定版及びcocos2dx 3.0 RC1向けのブランチです。betaとほぼ同じ実装ですが、RC1で変更された部分への対応が組み込まれています。

* master  
	　cocos2dx 3.0 alpha0〜1用のブランチです。俺々ライブラリは、元々alpha1向けのライブラリでしたが、作者がalphaを利用しなくなったため、このブランチの更新は停止しています。

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

## cocos2dx Node library
cocos2dxで利用できるノードやレイヤのライブラリです。

## Fluxion (cocos2dx Action Generator)
アクションを簡潔に記述、生成するためのDSLを提供します。  
[Fluxion Reference](./fluxion.html) を参照してください。

## Simple Debug Menu
シンプルなデバッグメニューを提供します。  
[Simple Debug Menu](./sdm.html) を参照してください。

#ライセンス
This software is released under the zlib License, see LICENSE.txt.