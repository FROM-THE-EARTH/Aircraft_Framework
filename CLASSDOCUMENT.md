# Avionics Class Document

所々更新しきれていないところがあります。

## AvionicsBase Class

変数やenumについては見ればわかると思います。

### 基底(抽象)クラスについて
あまり使う機会がないと思うので簡単に解説しておきます。

基底クラスとは、同じ機能を持ったクラスを、その機能を毎回記述する手間を省くために使われるクラスのことです。

電装においては、MbedやArduinoなどのプラットフォームによってセンサの動作実装が変わってしまいますが、発射待機→飛行→着地の流れは変わりません。また、プラットフォームが変わっても、テレメトリやSDへ書き込むデータのフォーマットは同じであるべきです。

もちろん基底クラスを使わずにすべての処理を同じクラスに記述したほうがパフォーマンスは上がりますが、フォーマットを変えたいときなどに全てのプラットフォームで仕様変更をする必要があり、手間ですしバグの原因にもなります。基底クラスを使えば基底クラスを変更するだけで済みます。

AvionicsBaseクラスを見ると、
```C++
virtual method() = 0;
```
という見慣れない文字列があると思います。ググればすぐ出ますが、簡単に言うと、このクラスでは定義しないから継承したクラスでちゃんと定義してくださいってことです。

例えば、センサからデータを取得する関数getDatas()はどんな電装を作っても必要になりますが、その中身はプラットフォームに依存します。そのため、基底クラスで「getDatas()は絶対実装しないとダメです！でもプラットフォーム依存なので継承したクラスで実装してね」とする必要があるわけです。

この関数(純粋仮想関数と呼ぶ)を含むクラスは抽象クラスと呼びます。

また、この関数を実装する際は
```C++
virtual method() override;
```
と宣言する必要があります。

ちなみにpublic, privateはわかると思いますが、protectedは、このクラスを継承したクラスでは使えるような変数や関数です。

以下各関数について解説します。virtual method() = 0;の関数はプラットフォーム依存なんだなあと思ってください。

### public method(main.cppで使用)
```C++
void begin();
```
いわゆるメインループです。

```C++
virtual bool initialize() = 0;
```
センサなどの初期化。

```C++
virtual void setDebugMode(bool mode) = 0;
```
mode=trueにするとUSBでPCにつないだときにコンソールにもテレメトリの内容が送信されます。

```C++
const Datas &data() const { return datas; }
```
datasを取得する関数です。間違えて代入しないように直接アクセスはできないようにしています。

```C++
bool (*Condition_Launch)();
bool (*Condition_Detach)() = Function::Condition::None;
bool (*Condition_Deceleration)();
bool (*Condition_Landing)();

void (*Operation_Detach)() = Function::Operation::None;
void (*Operation_OpenParachute)();
void (*Operation_CloseServo)();
```
発射条件や分離動作を記述する関数(変数?)です。main.cppで実装できると見やすいかと思い関数ポインタにしています。

Function::**::Noneは何もしない(常にfalseを返す)関数です。

### protected method(Avionicsクラスで使用)
```C++
virtual void update() = 0;
```
毎回更新する何か。getDatasとは違います。タイマーの更新など。

```C++
virtual void end() = 0;
```
すべての処理終了時に実行する関数です。sd.close()など。

```C++
virtual bool isReady(bool showDetail = true) = 0;
```
全てのセンサが使用可能であればtrueを返します。showDetail=trueの場合は各センサの状況を無線で送信します。(デフォルト)

```C++
virtual void reboot() = 0;
```
マイコンをリセットします。

```C++
virtual void getDatas() = 0;
```
センサから値を取得しdatasに格納します。高度の計算もここでやってます。

```C++
virtual void writeDatas() = 0;
```
SDへの書き込み。テレメトリで送信もしてます。

```C++
virtual void transmit(const xString &str) = 0;
```
AvionicsBaseで無線送信する関数使いたいなと思ったけどプラットフォーム依存だしどうしようと思った結果作った関数です。

```C++
virtual xString receive() = 0;
```
上記の受信版。

```C++
void beginRecord() { recording_ = true; }
```
writeDatas()を開始します。

```C++
void endRecord() { recording_ = false; }
```
writeDatas()を停止します。

```C++
void onReceiveCommand();
```
コマンドを受信したときの動作を記述しています。"reboot"と受け取ったらreboot()を実行するなど。

```C++
xString getCSVFormattedData() const {省略}
```
datasを元に、csvに書き込む用の文字列を返します。**フォーマットを統一するため、SDへの書き込みはこの関数を使用してください。**

```C++
bool isElapsed(float time){省略}
```
time秒経過したらtrueを返します。

### private method
```C++
bool checkFunctions();
```
Condition_Launchなどの関数ポインタがちゃんと実装されているかを確認します。実装されてなければエラーを吐いてプログラムは終了します。

```C++
void waiting();
```
準備中。コマンド"escape"で発射待機状態に遷移します。

```C++
void waitingLaunch();
```
発射待機中。Condition_Launch()が満たされたら飛行状態に遷移します。

```C++
void inFlight();
```
飛行中。分離や機構作動します。Condition_Landing()が満たされたら着陸状態に遷移します。

```C++
void landing();
```
着陸。hasGPS_=trueならGPS情報を送信します。

```C++
Commands checkCommand(const xString &recv);
```
受け取ったコマンドを識別します。

```C++
void applyIMUFilter();
```
9軸にフィルタを適用します。

## Avionics Class
ほぼAvionicsBaseクラスからオーバーライドした関数たちなので省略。

AvionicsBaseの純粋仮想関数はここで全て実装します。