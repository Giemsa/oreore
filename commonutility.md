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

### Persistent
　cocos2dxの参照カウントを管理するクラスです。**cocos2dx-3.0リポジトリのみで実装されています。**

	Persistent<Sprite> sprite = Sprite::create("image.png");
	
	// 以降Spriteとして利用可能
	sprite->setPosition(Point(50.0f, 50.0f));

 上記の使い方だけでは意味をなしません。cocos2dxのノードをメンバ変数として永続的に所持したい場合に有効です。

	class Hoge
	{
	private:
		// ここで宣言
		Persistent<Sprite> sprite;
	public:
		...
	};

	// あとは普通に初期化
	bool Hoge::init()
	{
		// 代入
		sprite = Sprite::create("image.png");
	}

　cocos2dxでは参照カウントを用いてメモリの管理が行われています。したがって、シーングラフに追加されていないノードは、そのフレーム終了時に破棄されてしまいます。そのため、retainを用いて参照カウントを手動で管理しなければならないのですが、retainしたノードはreleaseを用いて解放する必要があり、管理が煩雑になります。  
　このPersistentは、ノードの参照カウントの管理を自動で行うためのラッパークラスです。Persistent同士での代入等では、この参照カウントが正しく計算されます。

### DLog
　DLogは、デバッグログを出力するクラスです。**cocos2dx-3.0リポジトリのみで実装されています。**
cocos2dxのlog関数を拡張したデバッグログ出力を提供します。

	const int num = 28;
	dlog("{>\\06X}", num); // 00001C

　フォーマットの指定は、printfのものとは異なります。値を挿入したい部分に`{}`を記述します。このブレースの中に出力フォーマットを記述します。フォーマットによらず、ただ値を出力したい場合は`#`の1文字だけでも値の出力が可能です。

	dlog("#, #", 10, "hello"); // 10, hello

　以下に使用できるフォーマット指定子を載せます。

| 指定子 | 例 | 意味 |
| - | - | - |
| > | {>} | 出力桁数が指定された場合、右揃えで出力します。 |
| < | {<} | 出力桁数が指定された場合、左揃えで出力します(デフォルト)。 |
| ^ | {^} | 出力桁数が指定された場合、中央揃えで出力します(**未実装**)。 |
| 数値 | {6} | 出力桁数を指定します。|
| + | {+} | 出力対象が整数値の場合、符号を常に表示します。 |
| d | {d} | 出力対象が整数値の場合、10進数で出力します。 |
| o | {o} | 出力対象が整数値の場合、8進数で出力します。 |
| b | {b} | 出力対象がboolの場合、"true"と"false"の文字列で出力します。デフォルトでtrueは1、falseは0と表示されます。 |
| x | {x} | 出力対象が整数値の場合、16進数で出力します。アルファベットは小文字で出力されます。 |
| X | {X} | 出力対象が整数値の場合、16進数で出力します。アルファベットは大文字で出力されます。 |
| f | {f} | 出力対象が数値の場合、実数形式で出力します。 |
| s | {s} | 出力対象を文字列で出力します。通常出力形式は自動で判定されるため、この値が使用されることはありません。 |
| \\ | {\\d} | 次の文字をエスケープします。余ったスペースを埋める文字として使用されます。 |
| 文字 | {c} | 出力桁数を指定した場合に、余ったスペースに表示する文字を指定します。デフォルトで`' '`(スペース)です。 |

　これらのフォーマット指定子は、組み合わせて使用します。同一カテゴリに所属するフォーマット指定子を複数指定した場合、あとに指定したものが適用されます。

	dlog("{*6}", 10); // 10****

	// '*'を上書き。'>'はフォーマット指定子なので'\'でエスケープ
	dlog("{*6\\>}", 10); // 10>>>>

#### ログレベル
　DLogでは、ログレベルとして

 * Info
 * Debug
 * Warning
 * Error

の4段階を指定することが可能です。`dlog`関数ではDebugレベルの出力が行われます。その他のレベルで出力したい場合、

 * DLog::info()
 * DLog::debug()
 * DLog::warn()
 * DLog::error()

メソッドをそれぞれ使用します。パラメータは`dlog`のものと変わりありません。

	DLog::error("Error! msg: #", errorMsg);

#### ログフィルタ
　DLogは、出力するログのレベルを指定することが可能です。

	DLog::setFilter(DLog::LogType::Warning);

　上記のように記述することで、`LogType::Warinig`以上のログのみを出力することができます。ログフィルタは、デフォルトで`LogType::Debug`以上に指定されています。したがって、`setFilter`で`Info`あるいは`LogType::All`を指定しない場合、`DLog::info`による出力は行われません。

　`setFiletr`で`LogType::Disable`を指定すると、全てのログの出力が行われなくなります。

#### 時刻表示
　ログが出力された時刻を出力するには、`DLog::setShowTime`を利用します。この値はデフォルトで`false`に設定されています。

	DLog::setShowTime(true); // 時刻表示有効化

#### カスタムフォーマッタ
　「NOT_USE_COCOS2DX」マクロが指定されていない場合、cocos2dxで利用できるカスタムフォーマッタがインクルードされます。

 * coocs2d::Point
 * cocos2d::Size
 * cocos2d::Rect
 * cocos2d::Node を継承したクラスのポインタ

　をそのまま出力することが可能です。

	dlog("#, #", sprite->getPosition(), sprite->getContentSize());
	// (10, 10), (256, 128)

　のような形式で使用できます。cocos2d::Nodeを継承したクラスの場合、そのクラスの`getDescription`が呼び出されます。

	dlog("#", sprite); // spriteはcocos2d::Spriteのインスタンス
	// <Sprite | Tag = -1, TextureID = 3>

　DLogｎ出力にはstd::ostreamの`<<`演算子が利用されています。したがって、任意のクラスのデバッグ出力を行うフォーマッタを作成することが可能です。

	// 以下はcocos2d::Rectを出力するカスタムフォーマッタ
    std::ostream& operator <<(std::ostream& os, const Rect &rect)
    {
        DLog::initFlags();
        os << "(";
        DLog::outputFormat(rect.getMinX());
        os << ", ";
        DLog::outputFormat(rect.getMinY());
        os << ", ";
        DLog::outputFormat(rect.getMaxX());
        os << ", ";
        DLog::outputFormat(rect.getMaxY());
        os << ")";
        return os;
    }

　`DLog::initFlags`を必ず最初に呼び出してください。その後、`os`に出力したい値を書き込みます。書き込む値にフォーマットを適用したい場合、`os`に書き込むのではなく、`DLog::outputFormat`を使って出力します。