//********** include **********
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <string.h>

//********** define **********
#define LCD_RS 2        // LCD RS Pin
#define LCD_EN 3        // LCD Enable Pin
#define LCD_D4 4        // LCD Data Bus 1
#define LCD_D5 5        // LCD Data Bus 2
#define LCD_D6 6        // LCD Data Bus 3
#define LCD_D7 7        // LCD Data Bus 4
#define SPEAKER_PIN 12  // スピーカー接続ピン
#define GPS_TX 10       // GPS通信PIN(TX)
#define GPS_RX 11       // GPS通信PIN(RX)
#define TONE_HIGT 2000  // 高音周波数
#define TONE_LOW 1000   // 低音周波数
#define SOUND_DELAY 100 // 待ち時間(ms)
#define MAX_ROW 16      // 最大文字数
#define MAX_COL 2       // 最大行数
#define INIT_WAIT 5000  // 初期表示待ち時間
#define MINIMUM_SAT 4   // 3次元計測に必要な最低衛星数
#define DISP_DELAY 200  // メッセージ表示ディレイ

//********** prototype **********
static void play98();
