#include "gps.h"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
const int serialTime = 200;

//********** arduino セットアップ **********
void setup()
{
    lcd.begin(MAX_ROW, MAX_COL);  // LCD通信開始
    lcd.clear();                  // LCD画面クリア
    gpsSerial.begin(9600);        // GPS通信開始
    lcd.setCursor(0, 0);          // LCD カーソル移動 (1行目)
    lcd.print("GPS Multi Meter"); // 初期表示 1
    lcd.setCursor(0, 1);          // LCD カーソル移動 (2行目)
    lcd.print("Version 0.1");     // 初期表示 2
    play98();                     // 起動音再生
    delay(INIT_WAIT);             // 待ち
    lcd.clear();
} // end setup

//********** メイン **********
void loop()
{
    char str[MAX_ROW] = {0};
    int start = millis();
    int now = millis();

    do
    {
        while (gpsSerial.available() > 0) // GPSモジュールとのシリアル通信が可能なら
        {
            gps.encode(gpsSerial.read()); // GPSデータ読み込み
        }
        now = millis();                   // 計測時刻更新
    } while ((now - start) < serialTime); // 更新されるまで

    if (gps.satellites.value() > 0) // 衛星から電文を受け取っているなら
    {
        if (gps.satellites.value() >= MINIMUM_SAT) // 3次元計測が可能なら
        {
            sprintf(str, "%02d:%02d ALT:%5dm",   // 時刻と標高表示
                    gps.time.hour() + 9,         // GPSから時を取得(GMT->JST変換)
                    gps.time.minute(),           // GPSから分を取得
                    (int)gps.altitude.meters()); // GPSから標高を取得
        }
        else // 3次元計測不可能なら
        {
            sprintf(str, "%02d:%02d ALT:-----m", // 時刻と標高表示(標高に無効表示)
                    gps.time.hour() + 9,         // GPSから時を取得(GMT->JST変換)
                    gps.time.minute());          // GPSから分を取得
        }
        lcd.setCursor(0, 0); // カーソルを始点へ移動(1行目)
        lcd.print(str);      // LCDにデータ表示

        lcd.setCursor(0, 1);            // カーソルを2行目に移動
        sprintf(str, "SPEED:%4dkm/h  ", // 速度表示
                (int)gps.speed.kmph()); // GPSから速度を取得
        lcd.print(str);                 // LCDにデータ表示
    }
    else // 測位無効の場合
    {
        lcd.setCursor(0, 0);           // カーソルを始点へ移動
        lcd.print("Searching Signal"); // 1行目メッセージ表示
        lcd.setCursor(0, 1);           // カーソルを2行目へ移動
        lcd.print("Just a moment..."); // 2行目メッセージ表示
    }
}

//********** 起動音再生 **********
static void play98()
{
    tone(SPEAKER_PIN, TONE_HIGT); // 高音再生
    delay(SOUND_DELAY);           // 待ち
    tone(SPEAKER_PIN, TONE_LOW);  // 低音再生
    delay(SOUND_DELAY);           // 待ち
    noTone(SPEAKER_PIN);          // 再生停止
    delay(SOUND_DELAY);           // 待ち
}
