---
layout: default
---
# Fluxion (cocos2dx Action Generator)

## 概要
　Fluxionは、cocos2dxの面倒なアクション構築を簡潔に記述するためのライブラリです。  
一部APIはcocos2dx 2.x系でも利用できます。

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。

### アクションの生成
　基本的なアクションは「oreore::x」という名前空間に定義されています。

	Sprite *sprite = Sprite::create("hoge.png");
	addChild(Sprite);

	sprite->runAction(x::moveTo(1.0f, 100.0f, 100.0f));

　引数はcocos2dxのアクションのcreateメソッドに渡すものと同等です。moveToやmoveByのように、本来cocos2dx::Pointで渡さなければならないメソッドは、上記コードのようにfloat, floatで渡すことも出来ます。  
　現在鋭意実装中のため、基本アクション以外は使用できません。使用する場合は、cocos2dxのアクションをそのまま利用してください。

### 連続アクション
　連続でアクションを実行したい場合は「>>」演算子を利用します。

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) >>
	    x::fadeOut(1.0f)
	);

　このように記述することで、MoveToの後にFadeOutが実行されます。また、x::sやx::seqを利用することもできます。

	sprite->runAction(
	    x::seq(
	        x::moveTo(1.0f, 100.0f, 100.0f),
	        x::fadeOut(1.0f)
	    )
	);

　[2.2.2] C++03では可変長引数の都合から、x::seq及びx::sに渡せるアクションの数は**8個まで**に制限されています。

### 並列アクション
　同時に複数のアクションを実行したい場合は、「+」演算子を利用します。

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) + x::fadeOut(1.0f)
	);

　このように記述することで、MoveToと同時にFadeOutが実行されます。また、x::pやx::parを利用することもできます。

	sprite->runAction(
	    x::p(
	        x::moveTo(1.0f, 100.0f, 100.0f),
	        x::fadeOut(1.0f)
	    )
	);

　[2.2.2] C++03では可変長引数の都合から、x::par及びx::pに渡せるアクションの数は**8個まで**に制限されています。

### Ease
　Easingを行う場合は、「*」演算子を利用します。

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) * x::Ease::bounceOut()
	);

　Easeアクションは、「x::Ease」という名前空間に定義されています。
Ease/Sine/Exp/BounceのIn/Out/InOutがが利用可能です。

	x::Ease::EaseInOut
	x::Ease::ExpIn
	...

### 繰り返し
　アクションの繰り返しを行うには、「*」演算子を利用します。

	sprite->runAction(
	    (x::moveTo(1.0f, 500.0f, 500.0f) >> x::moveTo(1.0f, 100.0f, 100.0f)) * 5
	);

　このように記述することで、移動が5回繰り返されます。

### 無限繰り返し
　アクションを無限に繰り返すには、-1を掛けるか、`x::inf()`を利用します。同様の動作をする`x::infinite()`も用意されています。

	sprite->runAction(
	    (x::moveTo(1.0f, 500.0f, 500.0f) >> x::moveTo(1.0f, 100.0f, 100.0f)) * x::inf()
	);

### 関数呼び出し
　CallFuncに相当するアクションを追加します。  
　[3.0] cocos2dxにおけるCallFuncを登録するには、ラムダ関数をそのまま追加します。

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) >>
	    []() {
	        log("complete!");
	    }
	);

　[2.2.2] x::callを利用します。

	void Sample::callback()
	{
		CCLOG("complete!");
	}

	...

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) >>
	    x::call(this, &Sample::callback)
	);

　3.0系でも、明示的に`x::call`を利用することができます。


### cocos2dxのアクションを混ぜる
　Fluxionでは、cocos2dxのアクションを同時に扱うことが可能です。

	sprite->runAction(
	    x::moveTo(1.0f, 100.0f, 100.0f) >>
	    MoveBy::create(1.0f, Point(100.0f, 100.0f)) // cocos2dxのアクション
	);

　ただし、C++の仕様上cocos2dxのアクション2つをつなぐことは**出来ません**。

	sprite->runAction(
	    MoveTo::create(1.0f, Point(100.0f, 100.0f)) >>
	    MoveBy::create(1.0f, Point(100.0f, 100.0f)) // 両方ともcocos2dxのアクション
	);

### アクションをFluxionアクションに変換する
 `x::flux`を利用することで、cocos2dxのアクションをFluxionアクションに変換できます。

	sprite->runAction(
	    x::fluc<CallInterval>(1.0f, [](float dt) {
	    	dlog("#", dt);
	    }) >>
	    []() {
	    	log("complete!");
	    }
	);


## 複雑なアクション

	sprite->runAction(
	    x::moveTo(1.0f, 200.0f, 200.0f) >>
	    x::moveBy(1.0f, -100.0f, -100.0f) + (x::fadeOut(0.5f) >> x::fadeIn(0.5f)) >>
	    []() {
	        log("complete!");
	    }
	);