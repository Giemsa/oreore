---
layout: default
---
# MultiResolution(実装中)

**実装中につき不安定です**

## 概要
　MultiResolutionは、アプリの多解像度対応を行うユーティリティです。アプリの解像度等の条件から、アセットのフォルダを適切に読み込み、ContentScaleFactorを設定します。

## 使い方

### はじめに
　oreore.hをインクルードしてください。

	#include "./oreore/oreore.h"

　名前空間はoreoreになっています。面倒なので、cppの頭で「using namespace oreore;」しておきましょう。  
ここから先のコードは名前空間oreoreを省略します。

### 解像度設定の登録

　MultiResolutionを使うには、まず解像度と読み込むアセットフォルダの設定が必要です。以下のコードで設定が可能です。  

    MultiResolution &mr = MultiResolution::getInstance();
    mr.setResolutionList({
        { "resources-iphone",   1.0f, Application::Platform::OS_IPHONE,  Size(320, 480) },
        { "resources-iphonehd", 2.0f, Application::Platform::OS_IPHONE,  Size(640, 960) },

        { "resources-small",    1.0f, Application::Platform::OS_ANDROID, Size(384, 568) },
        { "resources-large",    2.0f, Application::Platform::OS_ANDROID, Size(768, 1136) },
    });
    mr.resolve();

　MultiResolutionはシングルトンです。getInstance()でインスタンスを取得します。  

　setResolutionListを用いて解像度設定を行います。上記の例では、上から  

 * 非Retina iPhone (3.5inch)
 * Retina iPhone(3.5 & 4.0 inch) / iPad
 * Android端末(解像度低)
 * Android端末(解像度高)

の設定になっています。  
　1つ目のパラメータはアセットフォルダです。今回はCocosBuilderを用いた場合の例を記載しています。  
　2つ目のパラメータはContentScaleFactorの算出に利用される値です。上記の例では非Retina iPhoneを1と定めて、そこからの倍率で指定しています。つまり、Retina iPhoneでは非Retina iPhoneの倍のサイズの画像の読み込みを行います。したがって、resources-iphonehdフォルダには、resources-iphoneに入っているアセットの倍のサイズの画像を用意する必要があります。  
　3つ目のパラメータは、判定を行うOSの指定です。Application::Platform列挙体の値を指定します。**iPhoneとiPadはどちらを指定しても同等の判定が行われます。つまり、OS_IPHONEだけを指定した場合でも、iPad端末での判定対象となります。**  
　4つ目のパラメータは、アセット切り替えを行う判定基準となる解像度を指定します。一番上の例では、320x480以上の画面サイズでその設定が適用されます。すなわち、非Retina iPhoneの設定が適用されることになります。2番目の例では、640x960以上の画面サイズでその設定が適用されます。つまり、非Retina iPhoneを除くiOS端末でこの値が適用されることになります。解像度による判定では、**解像度の大きい設定から比較が行われます。**  

　解像度設定における第1パラメータ、第2パラメータは必須ですが、第3パラメータ以降のパラメータには、

 * 判定対象OS
 * 解像度
 * カスタムコンパレータ

　のどれか1つ以上を指定することが可能です。  

 カスタムコンパレータには、戻り値boolのラムダ関数やファンクタ等を指定することができます。この関数の戻り値がtrueの場合に設定が適用されます。判定対象OSの設定や解像度の指定だけでは判定できない場合に利用してください。  
 
　設定が終わったら`resolve()`を呼び出して設定が完了します。

### BaseScaling

　CocosBuilderを利用している場合、Publish Settingsの「Automatic scaling from」の値によってContentScaleFactorの計算が変わってきます。MultiResolutionでは、この値が`2x (hd)`に設定されているものとして計算を行います(2xがベースということは、Retina iPhoneの設定を1.0とし、非Retina iPhoneが0.5とみなされる、ということです)。  
　このAutomatic scaling fromの値を別の値にした場合、MultiResolutionの`setBaseScaling`を利用して値を変更してください。この値を変更しないと、画像や座標計算が正しく行われません。