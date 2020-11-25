## 使い方
①vscodeの階層




![image](https://user-images.githubusercontent.com/58362633/100183144-94189f80-2f21-11eb-829a-b9a547e7d408.png)

この階層でvscodeを開く

arduino IDE をインストール(windows storeじゃなくて配布されてるやつ)
https://www.arduino.cc/en/software

vscodeの拡張機能でarduinoをダウンロード

拡張機能の設定から
C:\Program Files (x86)\Arduino
のパスを通す

②c_cpp_properties.jsonの設定
"USER_PATH": を自身のものに設定

③シリアル設定とボードの設定
command + shift + P => Arduino: Board Config（使用しているボードを指定）
command + shift + P => Arduino: Select a serial port（使用しているシリアルポートの指定）

## memo
ターミナルの文字化けの治し方
https://qiita.com/nori-dev-akg/items/e0811eb26274910cdd0e
