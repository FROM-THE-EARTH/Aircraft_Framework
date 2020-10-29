# Program Document

Avionicsクラスについては[**CLASSDOCUMENT.md**](https://github.com/FROM-THE-EARTH/Avionics-Software/blob/master/CLASSDOCUMENT.md)でより詳しく解説してます。

プラットフォームの切り替えはPlatformDefinition.hで#defineを切り替えて行います。見ればわかると思いますがあまり美しくはないです。

## Class構成

### 概要

AvionicsBaseクラスを基底クラスとし、このクラスを継承したクラスとして、mbed, arduino, etc...用にAvionicsクラスを作成していきます。

Avionicsクラスにプラットフォーム依存の動作を記述します。

最後にREADME.mdで書いたように、main.cppで各種動作条件や動作を記述します。

作ってから思いましたがマイコン用にしては相当贅沢にリソース使ってるプログラムになってしまいました。その代わり見通しはいいかなと思います。動作が不安定であったり速度が足りないようでしたらその都度書き換えてください。

### Avionics class

- AvionicsBase
    
  製作するロケットによって仕様が全然違う！といったことがないように、CSVファイルのフォーマットや、「準備中→発射待機→飛行中→機構作動→着陸」の流れ、テレメトリのコマンドや送信データのフォーマットを定義しています。

- Avionics

  タイマーやセンサ類の初期化、読み書きなどプラットフォーム依存の動作を記述しています。

  取得したデータは全てAvionicsBaseで定義されたdatas構造体に格納する必要があります。

### ModuleWrapper class
かなり雑な感じのコードになってます。もはやいらないかもしれない。

- ModuleWrapper: ライブラリによって異なる実装をなるべく統一した形で記述できるようにするための抽象クラスです。

- ○○Wrapper: 各ライブラリなどをラップしたクラスです。元となるライブラリと、ModuleWrapperを継承しています。

## Type
### Datas
time, pressure, temperatureなどをまとめた構造体です。

### Vec3
accel, gyro, magnで毎回.xとかするのが面倒なので作りました。パフォーマンスを求めるならまずこれを消すのがいいと思います。

### XString
Mbedではstd::string, arduinoではStringを使わなければならないため、PlatformDefinition.hの内容によってどちらを使用するか切り替えるための型です。to_string系は全てto_XStringを使用してください。引数はintとfloatだけ対応してますが、追加するのは楽だと思います。

パフォーマンスを求めるならここも削るべきだと思いますが、個人的にstring使わないと死んでしまうので作りました。

## Utils
どこかで使う関数たちをまとめてます。

XStringと同様に、Mbedとarduinoで使うべき関数が違うのでそれの切り替えもしてます。

## Filter
9軸用フィルタです。Madgwickのみ実装(サードパーティ)済み。

## Constants
その名の通りです。