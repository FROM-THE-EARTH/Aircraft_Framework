# Avionics
ハイブリッドロケット電装の開発を容易にするためのフレームワークです。

少なくとも9期代で作成した電装に関しては出来るような設計です。

一部センサがまだ使えません（11月までにはします）

詳細な仕様については[**DOCUMENT.md**](https://github.com/FROM-THE-EARTH/Avionics-Software/blob/master/DOCUMENT.md)に書いてます。

本フレームワークを使用した電装のリポジトリ（[**FTE09ちんちろりん**](https://github.com/FROM-THE-EARTH/FTE09_chinchirorin)）

## 動作環境（確認済み）
### Mbed LPC1768
- OS: Mbed OS 6.2.0
- 開発環境: Mbed Studio
- 使用センサ: IM920, LSM9DS1, MM-TXS03, ADXL03, GYSFDMAXB, CK-40

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
#include "Avionics/Avionics.h"

//コンストラクタ(IMUフィルタ有無 = true, フィルタに地磁気を用いるか = false)
Avionics avionics;

DigitalOut igniter(p20, 0);

void defineFunctions();

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

void defineFunctions();

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
void defineFunctions(){
  avionics.Condition_Launch = []() {
    return (avionics.data().accel.length() > 2.5);
  };

  avionics.Condition_Detach = [](){
    return (avionics.data().time - avionics.data().launchTime) > 7.0f;
    //デフォルトでFunction::Condition::Noneとしているので必要ない場合は定義する必要はない
  };

  avionics.Condition_Deceleration = [](){
    return (avionics.data().time - avionics.data().launchTime) > 140.0f;
  };

  avionics.Condition_Landing = []() {
    return (avionics.data().time - avionics.data().bootTime) > 140.0f;
  };

  avionics.Operation_Detach = [](){
    igniter = 1;
    
    //デフォルトでFunction::Operation::Noneとしているので必要ない場合は定義する必要はない
  };

  avionics.Operation_OpenParachute = [](){
    //servo
  };

  avionics.Operation_CloseServo = [](){
    servo_1.pulsewidth(2.20/1000.0);
    servo_2.pulsewidth(2.20/1000.0);
  };
}
```