---
layout: default
---
# 概要
"Oreore/俺々"は、俺がゲームを開発するときに得するユーティリティライブラリです。  
cocos2dx 3.0 alpha1 向けの汎用的な処理群をまとめました。

# 動作環境
* C++11がビルドできる環境
* cocos2dx 3.0 alpha1

# 使い方
## cocos2dxを利用しない場合
oreoreをcloneします。

	git clone https://github.com/Giemsa/oreore.git

oreoreフォルダ内の"oreore.h"をincludeしてください。  
また、oreoreフォルダ内の*.cppファイルをビルド対象に加えてください。

## cocos2dxを利用する場合
マクロ「USE_COCOS2DX」が有効になっている場合、oreore/cocos2dフォルダ以下のユーティリティ群も同時にincludeします。

# 構成
俺々は、以下のライブラリ群を含んでいます。

* common utility
* cocos2dx Action library
* cocos2dx Node library
* Fluxion (cocos2dx Action Generator)

## common utility
cocos2dxにかかわらず利用できるユーティリティです。

## cocos2dx Action library
cocos2dxで利用できるアクションのライブラリです。

## cocos2dx Node library
cocos2dxで利用できるノードやレイヤのライブラリです。

## Fluxion (cocos2dx Action Generator)
アクションを簡潔に記述、生成するためのDSLを提供します。  
[Fluxion Reference](/fluxion.html) を参照してください。

#ライセンス
This software is released under the zlib License, see LICENSE.txt.