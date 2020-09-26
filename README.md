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

- AircraftBase: 一般的な機体を対象にした抽象クラスです。<br>
　電装のメインループの動作（データ取得→（準備中→発射待機→飛行中→着地後））の流れを記述しています。このクラスではこれらの具体的な動作については実装していません。

- AircraftMbedBase: Mbed用にAircraftBaseクラスを継承した抽象クラスです。<br>
　主要な実装はここでされています、データ取得書き込みの方法（どのモジュールをどのように実行するか）や、各シーケンスの動作などを記述しています。

- Aircraft: 各種条件や動作を記述するための、AircraftMbedBaseクラスを継承したクラスです。<br>
　AircraftMbedBaseクラスによって、機体に搭載されたモジュールが全て動作するという前提の元、個々の機体によって異なる条件（離床分離開傘）やその時の動作（分離時は点火、開傘時はサーボを動かすなど）を記述します。

### ModuleWrapper
- ModuleWrapper: ライブラリによって異なる実装をなるべく統一した形で記述できるようにするための抽象クラスです。

- ○○Wrapper: 各サードパーティ製ライブラリをラップしたクラスです。元となるライブラリと、ModuleWrapperを継承しています。

## 利用方法
1. ソースコードを全てプロジェクト下に置く
2. main.cppにAircraft.hをincludeして以下のようなコードを記述する（適宜pinや名前は変える）
3. 製作するロケットに応じて分離条件、開傘条件を以下のように記述する。
4. LPC1768向けにビルドする

失敗する場合はMbed OSバージョンが違う可能性があります。動作確認済みのバージョンは6.2.0です。[**ここ**](https://os.mbed.com/mbed-os/releases/)からMbed OSのダウンロードができます。

搭載しているモジュールが異なる場合は必要に応じて書き換えて下さい。

```C++
/* main.cpp */ 
#include "Aircraft.h"

constexpr float launchThreshold = 2.5;    // G
constexpr float landingTime = 140 * 1000; // ms

Aircraft aircraft(launchThreshold, landingTime);

int main() {
  printf("Hello Mbed\r\n");

  aircraft.setDebugMode(true);

  aircraft.initialize();

  aircraft.begin();
}
```


```C++
/* Aircraft.cpp */
#include "Aircraft.h"

//離床条件
bool Aircraft::launchCondition(){
  return 加速度 > 2.5G;
}

//分離条件
bool Aircraft::detachCondition() {
  return (経過時間 - 離床時間) > 8.0s;
}

//減速機構作動条件
bool Aircraft::decelerationCondition() {
  return 高度 < (最高高度 - 10m);
}

//着地判定条件
bool Aircraft::landingCondition(){
  return (経過時間 - 開傘時間) > 120.0s;
}

//分離時の動作
void Aircraft::detachAircraft(){
  点火電装に信号を送信
}

//パラシュート開傘時の動作
void Aircraft::openParachute(){
  サーボを動かす
}
```

## 拡張
### AircraftMbedBaseの拡張
Mbed以外のプラットフォーム向け（例えばArduino）に拡張するには、AircraftMbedBaseをAircraftArduinoBaseクラスなどにして、新たにArduino向けに実装すると様々なプラットフォーム向けに拡張できると思います。

その際はサードパーティ製ライブラリの更新、WrapperをArduino向けに再実装する必要があります。

実装済みセンサ以外のセンサを利用したい場合はそのセンサ用のライブラリとWrapperを用意し、AircraftMbedBaseを拡張する必要があります。

### AircraftBaseの拡張
- 準備中・発射待機・飛行中・着陸　以外の状態を持つ
- Mbed再起動・準備完了・センサーのチェック・サーボモーターを閉じる　以外のコマンドが存在する

以上の機能を実装するためにはAircraftBase及びAircraftMbedBaseの拡張が必要です。


