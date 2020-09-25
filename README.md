# Aircraft_Framework
ハイブリッドロケット電装の開発を容易にするためのフレームワークです。

分離条件、パラシュート開傘条件を記述するだけで一般的な電装を実装可能になっています。

未完成（2020/09/26時点）

## 動作環境（確認済み）
### Mbed LPC1768
- OS: Mbed OS 6.2.0
- 開発環境: Mbed Studio
- 使用センサ: IM920, LSM9DS1, LPS331

### Todo: Arduino等

## サンプルプログラム
本フレームワークを使用した電装のリポジトリ（[**FTE09ちんちろりん**](https://github.com/FROM-THE-EARTH/FTE09_chinchirorin)）

## 構成
### Aircraft
- Aircraft: 分離条件、開傘条件を記述するための、AircraftMbedBaseクラスを継承したクラスです。

- AircraftMbedBase: Mbed用にAircraftBaseクラスを継承した抽象クラスです。主要な実装はすべてここに記述されています。

- AircraftBase: 一般的なハイブリッドロケットを対象にした抽象クラスです。

### ModuleWrapper
- ModuleWrapper: ライブラリによって異なる実装をなるべく統一した形で記述できるようにするための抽象クラスです。

- ○○Wrapper: 各サードパーティ製ライブラリをラップしたクラスです。元となるライブラリと、ModuleWrapperを継承しています。

## 利用方法
1. ソースコードを全てプロジェクト下に置く
2. main.cppにAircraft.hをincludeして以下のようなコードを記述する（適宜pinや名前は変える）
3. 製作するロケットに応じて分離条件、開傘条件を以下のように記述する。
4. LPC1768向けにビルドする
5. 失敗する場合はMbed OSバージョンが違う可能性があります。動作確認済みのバージョンは6.2.0です。[**ここ**](https://os.mbed.com/mbed-os/releases/)からMbed OSのダウンロードができます。

```C++
/* main.cpp */ 
#include "Aircraft.h"

constexpr float launchThreshold = 2.5;    // G
constexpr float landingTime = 140 * 1000; // ms
constexpr float basePressure = 1015.0;    // Pa

// modules
IM920Wrapper receiver("Receiver_A",p28, p27, p29, p30);
IM920Wrapper transmitter("Sender_A", p28, p27, p29, p30);
LPSWrapper lps331("LPS331_A", p9, p10, LPS331_I2C_SA0_HIGH);
LSMWrapper lsm("LSM9DS1_A", p9, p10);

Aircraft aircraft(launchThreshold, landingTime, basePressure, &receiver, &transmitter,
                   &lps331, &lsm);

int main()
{
  aircraft.setDebugMode(true);

  aircraft.initialize();

  aircraft.begin();
}
```


```C++
/* Aircraft.cpp */
#include "Aircraft.h"

//分離条件
bool Aircraft::detachFlag() {

  return (datas.altitude > 500);//trueであれば分離
  
  //多段式でないなら
  //return false;
}

//開傘条件
bool Aircraft::decelerationFlag() {

  return (datas.altitude < datas.maxAltitude - 10);//trueであれば開傘
}
```

## 拡張
AircraftMbedBaseをAircraftArduinoBaseクラスなどにして、新たにArduino向けに実装すると様々なプラットフォーム向けに拡張できると思います。

その際はサードパーティ製ライブラリの更新、WrapperをArduino向けに再実装する必要があります。
