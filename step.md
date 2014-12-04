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
　ユーザが実装するのは、主にSerializableクラスです。

#### シリアライズ
    class SaveData final : public oreore::Step::Serializable
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

　また、「utils/step/Serializer/JSON.h」をインクルードすることで、JSONへのシリアライズを簡単に行うことが可能です。

    class SaveData final : public oreore::Step::Serializer::JSON
    {
        bool fromJSON(const picojson::value &data, const std::string &error) override
        {
            // json読み込み(デシリアライズ)
        }
    
        bool toJSON(picojson::value &out) const override
        {
            // json書き込み(シリアライズ)
        }
    };

　Serializer::JSONを用いた場合、JSONの操作にはpicojsonを利用します。将来MessagePackもサポートされます。

#### 暗号化
　Blowfishを用いた暗号化が可能です。が、今は実装中なので利用できません。

#### ストア/リストア
　データの書き込み処理を行います。以下の2つのクラスが実装されています。

 * StringStorage
 * FileStorage

　StringStorageは、文字列へのストア、リストアを行います。FileStorageはファイルの読み込み、書き込みを行います。**FileStorageは現在書き込みのみ可能です。**

### 例
　以下は文字列へデータを保存する方法です。

	using namespace oreore::Step::Storage;
	SaveData data; // Serializableを継承したクラス
	const std::string &str = data >> StringStorage();

　読み込みは以下のとおりです。

	const std::string str = "..."; // データ
	SaveData data;
	const bool success = StringStorage(str) >> data; // 戻り値は成功したかどうか

現在サポートされていませんが、暗号化をする場合は

	using namespace oreore::Step::Storage;
	using namespace oreore::Step::Encrypt;
	SaveData data; // Serializableを継承したクラス
	const std::string &str = data >> Blowfish("key") >> StringStorage();

のようになります。
