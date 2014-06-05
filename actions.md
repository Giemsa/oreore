---
layout: default
---
# cocos2dx Action library
## 概要
　cocos2dxで利用できる拡張アクションのライブラリです。使えるものから使えないものまで雑多に含まれています。

## リファレンス
**cocos2dx-2.2.2ブランチでは、全てのアクション名の先頭に“CC”が付加されています。またcocos2dx-2.2.2ブランチには存在しないアクションもあります。**

### AttractMove
　指定された一点に収束するように移動するアクションです。

	static AttractMove* create(
		const float duration,
		const cocos2d::Point &target,
		const cocos2d::Point &v0
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| target | Point & | 移動先の座標 |
| v0 | Point & | 初速ベクトル。現在地からv0の速度で移動を開始します。 |

### CircleMoveBy
　ある点を中心に指定された度数(度数法)だけ回転移動します。

	static CircleMoveBy* create(
		const float duration,
		const cocos2d::Point &center,
		const float deg
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| center | Point & | 中心の座標 |
| deg | float | 回転する度数(度数法) |

### CircleMoveTo
　ある点を中心に指定された度数(度数法)まで回転移動します。

	static CircleMoveTo* create(
		const float duration,
		const cocos2d::Point &center,
		const float deg
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| center | Point & | 中心の座標 |
| deg | float | 回転する度数(度数法) |

### CallInterval
　指定された間、毎フレーム関数を実行します。cocos2dx 3.0系専用です。

	static CallInterval* create(
		const float duration,
		const std::function<void (float)> &func
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| func |　std::function<void (float)> &　| 呼び出される関数。引数にはアクションの累計経過時間(開始:0〜終了:1)が指定されます。 |

### Move
　指定された時間の間、等速で移動します。

	static Move* create(
		const float duration,
		const cocos2d::Point &vec
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| vec | Point & | 速度ベクトル |

### Shake
　ノードを指定された量だけランダムに振動させます。

	static Shake* create(
		const float duration,
		const float level
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| level | float | 振動量。ピクセル。 |

	static Shake* create(
		const float duration,
		const float level_x,
		const float level_y
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| level_x | float | X方向の振動量。ピクセル。 |
| level_y | float | Y方向の振動量。ピクセル。 |

### ShakeFadeOut
　ノードを指定された量だけランダムに振動させます。振動量は徐々に減っていきます。

	static ShakeFadeOut* create(
		const float duration,
		const float level
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| level | float | 振動量。ピクセル。 |

	static ShakeFadeOut* create(
		const float duration,
		const float level_x,
		const float level_y
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| level_x | float | X方向の振動量。ピクセル。 |
| level_y | float | Y方向の振動量。ピクセル。 |

### MusicFadeTo
　BGMを指定された音量までフェードさせます。

	static MusicFadeTo* create(
		const float duration,
		const float volume,
		const bool pauseOnComplete = false
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | フェード時間 |
| volume | float | フェード先の音量 |
| pauseOnComplete | bool | フェード終了時にBGMの再生を停止するかどうか。trueの時、アクション終了時にBGMの再生が停止されます。デフォルト値はfalse |

### PlaySE
　SEを鳴らします。

	static PlaySE* create(
		const std::string &filename
	);

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| filename | std::string & | SEファイル名 |

### SwayMoveBy
　指定された移動量だけ、ゆらゆら揺れながら移動します。

	static SwayMoveBy* create(
		const float duration,
		const cocos2d::Point &pos,
		const float amp,
		const float freq
	);

	static SwayMoveBy* create(
		const float duration,
		const cocos2d::Point &pos,
		const float amp,
		const float freq,
		const SwayDirection type
	);

	// 揺れの方向
	enum class SwayDirection
	{
		Vertical, // 縦揺れ
		Horizontal // 横揺れ
	};

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| pos | Point & | 移動量 |
| amp | float | 振動の強さ。大きいほど揺れの幅が大きくなる |
| freq | float | 揺れの周波数。大きいほど揺れの回数が多くなる |
| type | SwayDirection | 揺れの方向。 |



### SwayMoveto
　指定された位置まで、ゆらゆら揺れながら移動します。

	static SwayMoveTo* create(
		const float duration,
		const cocos2d::Point &pos,
		const float amp,
		const float freq
	);

	static SwayMoveTo* create(
		const float duration,
		const cocos2d::Point &pos,
		const float amp,
		const float freq,
		const SwayDirection type
	);

	// 揺れの方向
	enum class SwayDirection
	{
		Vertical, // 縦揺れ
		Horizontal // 横揺れ
	};

| 引数 | 型 | 説明 |
| --- | -- | ---- |
| duration | float | アニメーション時間 |
| pos | Point & | 移動先の座標 |
| amp | float | 振動の強さ。大きいほど揺れの幅が大きくなる |
| freq | float | 揺れの周波数。大きいほど揺れの回数が多くなる |
| type | SwayDirection | 揺れの方向。 |
