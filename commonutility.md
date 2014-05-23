---
layout: default
---
# Common Utility

## 概要
　cocos2dxの有無にかかわらず、何となく使えるかもしれない雑多な処理をまとめたユーティリティ群です。

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。  
　また、このユーティリティは「NOT_USE_COCOS2DX」マクロが設定されている場合(cocos2dx以外で利用する場合)でも使用することができます。

### 含まれているユーティリティ一覧
* null.h
	* nullが定義されています。

* Utils.h
	* 雑多なユーティリティです

* TimeCounter.h (非推奨)
	* 時間を計測するときに利用していたクラスです。メンテナンスされていません。

* Observer [cocos2dx-3.0RC1のみ]
	* cocos2dxで言うところのNotificationCenter(CCNotificationCenter)と同等の機能を持つクラスです。

### null.h
　「null」が定義されています。C++11が使える環境ではnullptrと同等です。そうでない場合は、Effective C++に掲載されているテンプレートクラスを利用します。

　C++におけるNULLはマクロであるため、処理系によっては0として定義されている場合があります。この場合、ヌルポインタを取る関数と整数を取る関数がオーバーロードされていた場合に、整数を取る関数が呼びされます。この問題を回避するために、このnull.h内で定義されているnullを使用してください。  
　ただし、**cocos2d::CCSequenceなど、可変長引数の最後にNULLが必要となる関数の場合は、このnullを使用しないでください**。可変長引数の場合、型の推定が行われないため、NULLとして扱われません。

### util.h
　主に数学的な関数群をまとめています。  

#### random

	unsigned long random();
	unsigned long random(const unsigned long max);
	unsigned long random(const unsigned long min, const unsigned long max);

	template<typename T>
	unsigned long random<T>(const unsigned long max);

	template<typename T>
	unsigned long random<T>(const unsigned long min, const unsigned long max);

　xorshiftを利用して乱数を生成します。引数にmin、maxが指定された場合、その範囲で乱数を生成します。乱数生成ルーチンはとてもシンプルなので、ある程度偏りが発生します(モジュロバイアス)。xorshiftは、周期が2^128-1ですが他の乱数生成ルーチンよりも高速に動作します。  
　テンプレート関数となっているrandomは、戻り値の型が指定された型にキャストされます。以下のコードを同等です。

	random<float>(10, 20);
	static_cast<float>(random(10, 20));

#### range

	template<typename T>
	T range(const T min, const T value, const T max);

　値を指定された範囲内に収めます。

	int v = 20;
	range(10, v, 50); // 20
	
	v = 0;
	range(10, v, 50); // 10

	v = 100;
	range(10, v, 50); // 50

#### digits

	template<typename T>
	constexpr T digits(const T n);

　指定されたnの桁数を返します。

	template<typename T>
	constexpr int digits();

　int型の最大桁数を返します。
	
	template<int N>
	constexpr int digits();

　指定されたNの桁数を返します。

#### sgn

	constexpr int sgn(const int v);
	constexpr int sgn(const double v);
	constexpr int sgn(const float v);

　指定された数値の符号を返します。

* v < 0の時、-1
* v == 0の時、0
* v > 0の時、1

　がそれぞれ返却されます。

### deg2rad, rad2deg

	constexpr double rad2deg(const double rad);
	constexpr double deg2rad(const double deg);

　それぞれラジアンを度数に、度数をラジアンに変換します。

### TimeCounter.h
　時間計測に使用していたクラスですが、使う機会が全くないのでメンテナンスしていません。ので、使いたい場合はソースを参照してください。

### Observer
　cocos2dxでいうところのNotificationCenter(CCNotificationCenter)と同等の機能を提供するクラスです。いわゆるObserverパターンです。cocos2dx 3.0ではNotificationCenterが非推奨になっているようなので、代替するクラスとして実装しました。**C++11の機能を使用しているので、cocos2dx-2.x系のブランチでは動作しません。**

#### 使い方
　基本的にはNotificationCenterと同じです。ただし、キーに指定するのは文字列ではなく、列挙型です。  
　また、任意の引数を渡すことが可能です。


	enum class NotificationID
	{
		CHANGE_SCENE,
		RETURN_MENU
	};

	// Subjectの登録
	Observer::getInstance().addSubject<void(int, int)>(
		NotificationID::CHANGE_SCENE,
		[](int x, int y) {
			printf("receive: %d, %d", x, y);
		}
	);

	Observer::getInstance().addSubject<void(double, std::string &msg)>(
		NotificationID::RETURN_MENU,
		[](double n, std::string &msg) {
			printf("receive: %f, %s", n, msg.c_str());
		}
	);

	// 通知
	Observer::getInstance().notify(NotificationID::CHANGE_SCENE, 10, 20);


#### getInstance

	Observer &Observer::getInstance();

　Observerインスタンスを返します。Observerオブジェクトははシングルトンです。

#### addSubject

	template<typename T, typename E>
	void Observer::addSubject(const E key, const std::function<T> &callback);

　Subjectを登録します。C++11ではラムダ式の型(戻り値と引数の型)が取得できないので、Tは省略できません(ただし、callbackにstd::function&lt;T&gt;を指定する場合は省略可能)。Eは省略可能です。

	Observer::getInstance().addSubject<void(int, int)>(
		NotificationID::CHANGE_SCENE,
		[](int x, int y) {
			printf("receive: %d, %d", x, y);
		}
	);

	// または
	const std:function<void(int, int)> callback = [](int x, int y) {
		printf("receive: %d, %d", x, y);
	};
	Observer::getInstance().addSubject(
		NotificationID::CHANGE_SCENE, callback
	);

　指定するラムダ式や関数は任意の引数を取ることができますが、戻り値はvoidである必要があります。

#### removeSubject

	template<typename E, typename>
	void Observer::removeSubject(const E key);

　指定されたキーのSubjectを削除します。

#### notify

	template<typename E, typename ...Ts>
	void Observer::notify(const E key, const Ts &...args);

　Subjectに通知を送信します。任意の引数を指定可能です。

#### clear

	void clear();

　登録されている全てのSubjectを削除します。