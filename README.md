# GPS-Multi-Meter
arduinoのGPSを使用したマルチメーターです。  
GPSから取得した各情報をLCDディスプレイに表示することができます。  
現在表示しているのは時刻、標高、速度です。  
コンパイルする前にarduinoにTinyGPS＋＋のライブラリをインストールしてください。
  
[arduinoと各モジュールの接続]

1.LCDディスプレイ->arduino  
  VSS->GND  
  VDD->5V  
  V0 ->GND(コントラスト調整用にGNDとの間に10kΩの可変抵抗を間に挟んでいます。)  
  RS ->No.2  
  E  ->No.3  
  DB4->No.4  
  DB5->No.5  
  DB6->No.6  
  DB7->No.7  
  A  ->5V(明るさ調整用にGNDとの間に10kΩの可変抵抗を間に挟んでいます。)  
  K  ->GND  

2.GPSモジュール->arduino
  TX->No.10  
  RX->No.11  

3.スピーカー  
  No.12、GND    
バグフィックスなどご自由にどうぞ。  
