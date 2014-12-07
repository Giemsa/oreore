---
layout: default
---
# Step (Savedata Serialization Utility)

## 概要
　簡潔にデータを保存するための機能を提供します。  
データのシリアライズ、デシリアライズや暗号化等の機能も提供します。  
**この機能は現在実装中&執筆中です**

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。

### データの保存方法
　Stepには、3つのフェーズが存在し、それぞれ

	シリアライズフェーズ → 暗号化フェーズ → ストアフェーズ


となっています。  

　シリアライズフェーズでは、任意のシリアライザブルクラスを継承したクラスを指定します。  
　暗号化フェーズでは、シリアライズフェーズでシリアライズされたデータに対し暗号化を施します。暗号化は任意の回数行うことが出来ます。暗号化を施さなくても構いません。  
　最後に、ストアフェーズでファイル等への保存を行います。

### データの読み込み方法
　読み込みは、保存と逆の方向にフェーズが流れます。

	リストアフェーズ → 復号化フェーズ → デシリアライズフェーズ

　リストアフェーズでは、データの読み込み等を行います。  
　復号化フェーズでは、暗号化されたデータの復号を行います。  
　デシリアライズフェーズでは、任意のクラスへのデシリアライズを行います。

### 実装

#### Stepの使い方
　Stepは、各フェーズを「>\>」演算子で繋ぐことで、様々な変換処理を柔軟に行うことが出来ます。この機能を使うためには、主にSerializableクラスの実装が必要です。

#### シリアライズ
	class SaveData final : public Step::Serializable
	{
		// この2つのメソッドを実装
		bool serialize(std::ostream &out) const
		{
			// outへ書き込む
		}

		bool deserialize(const std::istream &data)
		{
			// dataから読み込む
		}
	};

　serializeメソッドは、任意の構造体をoutへ書き込むことでシリアライズを行います。deserializeは逆に、引数として渡されたistreamからデータを読み込みます。  
　また、JSONへのシリアライズを行う場合、ユーティリティクラスとして「Step::JSON」があります。このクラスのserialize/deserializeを実装することで、JSONへのシリアライズが可能になります。

	class SaveData final : public Step::JSON
	{
		bool deserialize(const picojson::value &data, const std::string &error) override
		{
			// json読み込み(デシリアライズ)
		}
	
		bool serialize(picojson::value &out) const override
		{
			// json書き込み(シリアライズ)
		}
	};

　Step::JSONを用いた場合、JSONの操作にはpicojsonを利用します。将来MessagePackもサポートされます。  

　上記で定義したクラスの実体を、Serializerに引数として渡すことで、シリアライズ/デシリアライズが可能となります。

	SaveData data;
	Step::Serializer(data) >> ... // シリアライズ

	... >> Step::Serializer(data); // デシリアライズ

　シリアライザは、一連の流れの最初と最後にのみ配置可能です。中間のフェーズ(暗号化フェーズ)に置くことは出来ません。

#### 暗号化
　暗号化は、暗号化形式クラスと、Encrypter/Decrypterを用いて行います。  
現在oreoreに組み込まれている暗号化形式は「Blowfish」のみです。

	Step::Encrypt::Blowfish bf("key");

　まず、暗号化形式クラスを初期化します。そして、

	... >> Step::Encrypter(bf) >> ...

　のように、Encrypterの引数として渡すことで、暗号化が可能となります。また、復号化の場合は

	... >> Step::Decrypter(bf) >> ...

　のように、Decrypterの引数として渡します。


#### ストア/リストア
　データの書き込み処理を行います。以下の2つのクラスが実装されています。

 * StringStorage
 * FileStorage

　StringStorageは、文字列へのストア、リストアを行います。FileStorageはファイルの読み込み、書き込みを行います。

	std::string str;
	... >> Step::StringStorage(str); // strへストリームの内容が保存される。
	std::cout << str << std::endl;

	const std::string json = "{...}";
	Step::StringStorage(json) >> ... // 文字列をストリームに読み込む

以下はFileStorageの例です。 

	... >> Step::FileStorage("test.data"); // test.dataへ保存
	Step::FileStorage("test.data") >> ... // test.dataから読み込み

### 例
　以下は文字列へデータを保存する方法です。

	SaveData data; // Serializableを継承したクラス
	std::string json;
	const bool success = Step::Serializer(data) >> StringStorage(json);

　読み込みは以下のとおりです。

	const std::string json = "..."; // データ
	SaveData data;
	const bool success = StringStorage(json) >> data; // 戻り値は成功したかどうか

暗号化を行う場合は

	SaveData data; // Serializableを継承したクラス
	Step::Encrypt::Blowfish bf("hoge"); // 暗号化クラス
	std::string json;
	const bool success = Serializer(data) >> Step::Encrypter(bf) >> StringStorage(json);

のようになります。

### もっと便利な使い方

#### ストリームの事前構築
　Step::ProcessHolderを用いることで、ストリームを事前に構築することが出来ます。

	Step::ProcessHolder st = Step::Serializer(data) >> Step::StringStorage(str);
	const bool success = st.start();

ProcessHolderのstartを呼ぶことで、構築されたストリームを実行することが出来ます。  
ProcessHolderは、効率のためコピー禁止クラスとなっています。別の変数へ所有権を移動させる場合はstd::moveを利用して下さい。

#### 非同期処理
　**以下は実装中の機能になります。**  
　暗号化やディスクアクセス等、処理負荷の高い処理を行う場合は、ストリームの処理を非同期で行うことが出来ます。

	(Step::Serializer(data) >> Step::StringStorage(str)).startAsync([this](const bool success) {
		... // 結果
	}):

あるいは、ProcessHolderを用いて

	Step::ProcessHolder st = Step::Serializer(data) >> Step::StringStorage(str);
	st.startAsync([this](const bool success) {
		... // 結果
	}):

のように記述することで、別スレッドでストリームの処理を行い、処理が終了し次第指定されたコールバックを呼び出します。非同期処理の際、各フェーズで使用される変数はメインスレッドにあるものを利用しますので、ローカル変数をそのまま引数に渡すと危険です。