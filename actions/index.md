---
layout: default
---
# cocos2dx Action library
## 概要
　cocos2dxで利用できる拡張アクションのライブラリです。使えるものから使えないものまで雑多に含まれています。

## リファレンス
　書きかけ  
**cocos2dx-2.2.2ブランチでは、全てのアクション名の先頭に“CC”が付加されています**

### AttactMove
　指定された一点に収束するように移動するアクションです。
　
* コンストラクタ  

		static AttractMove* create(
			const float duration,
			const cocos2d::Point &target,
			const cocos2d::Point &v0
		);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
|duration| float | アニメーション時間 |
| target | Point & | 移動先の座標 |
| v0 | Point & | 初速ベクトル。現在地からv0の速度で移動を開始します。|

### CircleMoveBy
　ある点を中心に指定された度数(度数法)だけ回転移動します。

* コンストラクタ  

		static AttractMove* create(
			const float duration,
			const cocos2d::Point &target,
			const cocos2d::Point &v0
		);


### CircleMoveTo
　ある点を中心に指定された度数(度数法)まで回転移動します。

### Move
　指定された時間、等速で移動します。

### Shake
　ノードを指定された量だけランダムに振動させます。

### ShakeFadeOut
　ノードを指定された量だけランダムに振動させます。振動量は徐々に減っていきます。

### MusicFadeTo
　BGMを指定された音量までフェードさせます。

### PlaySE
　SEを鳴らします。

### SwayMoveBy
　指定された移動量だけ、ゆらゆら揺れながら移動します。

### SwayMoveto
　指定された位置まで、ゆらゆら揺れながら移動します。