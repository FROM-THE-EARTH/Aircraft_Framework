# Avionics
ハイブリッドロケット電装の開発を容易にするためのフレームワークです。

条件、動作を記述するだけで一般的な電装を実装可能になっています。（少なくとも9期代で作成した電装に関しては出来るような設計です）

一部センサがまだ使えません（11月までにはします）

## 動作環境（確認済み）
### Mbed LPC1768
- OS: Mbed OS 6.2.0
- 開発環境: Mbed Studio
- 使用センサ型番: IM920, LSM9DS1, MM-TXS03, ADXL03, GYSFDMAXB

### Arduino nano

  センサ類の確認はしてません。書き込みは可能。<br>
  以下のようなディレクトリ構成にしないと読み込んでくれないらしいです。<br>
  プロジェクト名は適当(例:denso)、densoディレクトリ下にsrcディレクトリ(必ずこの名前)を置き、その下に全部置く。

  denso/<br>
  &emsp;&emsp;├ denso.ino<br>
  &emsp;&emsp;└ src/<br>
  &emsp;&emsp;&emsp;&emsp;├ Avionics/<br>
  &emsp;&emsp;&emsp;&emsp;├ AvionicsBase/<br>
  &emsp;&emsp;&emsp;&emsp;├ ...
  
  Arduinoに知見のある方、電装開発ついでにフレームワークの方もアップデートしてもらえると今後役立つと思います。

## サンプルプログラム
本フレームワークを使用した電装のリポジトリ（[**FTE09ちんちろりん**](https://github.com/FROM-THE-EARTH/FTE09_chinchirorin)）

## 構成
### Avionics class

- AvionicsBase: 一般的な機体を対象にした抽象クラスです。<br>
　プラットフォームに依存しない部分の動作を定義しています。<br>
  一部プラットフォーム依存の関数を仮想関数としてAvionicsクラスで実装するようにしています。

- Avionics: 各プラットフォーム用にAvionicsBaseクラスを継承したクラスです。<br>
  タイマーやセンサ類の初期化、読み書きなどプラットフォーム依存の動作を記述しています。

### ModuleWrapper class
- ModuleWrapper: ライブラリによって異なる実装をなるべく統一した形で記述できるようにするための抽象クラスです。

- ○○Wrapper: 各ライブラリなどをラップしたクラスです。元となるライブラリと、ModuleWrapperを継承しています。

## 利用方法
1. ソースコードを全てプロジェクト下に置く
2. PlatformDefinition.hで、使用するプラットフォームに応じて#defineを編集する。
3. 以下のようなコードを記述する
4. 製作するロケットに応じて分離条件、開傘条件を以下のように記述する。
5. ビルドする

失敗する場合はMbed OSバージョンが違う可能性があります。動作確認済みのバージョンは6.2.0です。[**ここ**](https://os.mbed.com/mbed-os/releases/)からMbed OSのダウンロードができます。

搭載しているモジュールが異なる場合は必要に応じて書き換えて下さい。

```C++
/* Mbed */ 
#include "Avionics.h"

//コンストラクタ(IMUフィルタ有無 = true, フィルタに地磁気を用いるか = false)
Avionics avionics;

DigitalIn flightpin(p12);

static void defineFunctions();

int main() {
  defineFunctions();

  avionics.setDebugMode(true);

  avionics.initialize();

  avionics.begin();
}
```

```C++
/* Arduino */
#include "src/Avionics/Avionics.h"

Avionics avionics;

static void defineFunctions();

void setup() {
  defineFunctions();

  avionics.initialize();
}

void loop() {
  avionics.begin();
}
```

```C++
/* 各種条件や動作の定義例 */
/* Detach以外は定義しないとnullptrなのでエラー吐きます */
static void defineFunctions(){

  avionics.Condition_Launch = []() {
    return !flightpin && (avionics.data().accel.length() > 2.5);
  };

  avionics.Condition_Detach = Function::Condition::None;
  //デフォルトでFunction::Condition::Noneとしているので必要ない場合は書く必要はない

  avionics.Condition_Deceleration =[](){
    return avionics.data().altitude < (avionics.data().maxAltitude - 10.0f);
  };

  avionics.Condition_Landing = []() {
    return (avionics.data().time - avionics.data().bootTime) > 140.0f;
  };

  avionics.Operation_Detach = Function::Operation::None;
  //デフォルトでFunction::Operation::Noneとしているので必要ない場合は書く必要はない

  avionics.Operation_OpenParachute = サーボ動かす関数;
}
```