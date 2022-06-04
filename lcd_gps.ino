#include "gps.h"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
const int serialTime = 200;
static int playFlag = OFF;            // 電文受信時ビープ再生フラグ
static int SearchingSignalFlag = OFF; // 電文未受信時ビープ再生フラグ

//********** arduino セットアップ **********
void setup()
{
    play98();                     // 起動音再生
    lcd.begin(MAX_ROW, MAX_COL);  // LCD通信開始
    lcd.clear();                  // LCD画面クリア
    gpsSerial.begin(GPS_SERIAL);  // GPS通信開始
    lcd.setCursor(0, 0);          // LCD カーソル移動 (1行目)
    lcd.print("GPS Multi Meter"); // 初期表示 1
    lcd.setCursor(0, 1);          // LCD カーソル移動 (2行目)
    lcd.print("Version 0.04");    // 初期表示 2
    delay(INIT_WAIT);             // 待ち
    lcd.clear();                  // LCD画面クリア
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
        signalReceivedBeep();                      // 電文受信時にビープ再生を行う
        if (gps.satellites.value() >= MINIMUM_SAT) // 3次元計測が可能なら
        {
            if ((gps.time.hour() + JST_OFFSET) > HOUR_23) // 時差補正時に24時以上になるか
            {
                sprintf(str, "%02d:%02d ALT:%5dm",              // 時刻と標高表示
                        gps.time.hour() + JST_OFFSET - HOUR_24, // GPSから時を取得(GMT->JST変換)
                        gps.time.minute(),                      // GPSから分を取得
                        (int)gps.altitude.meters());            // GPSから標高を取得
            }
            else
            {
                sprintf(str, "%02d:%02d ALT:%5dm",    // 時刻と標高表示
                        gps.time.hour() + JST_OFFSET, // GPSから時を取得(GMT->JST変換)
                        gps.time.minute(),            // GPSから分を取得
                        (int)gps.altitude.meters());  // GPSから標高を取得
            }
        }
        else // 3次元計測不可能なら
        {
            sprintf(str, "%02d:%02d ALT:-----m",  // 時刻と標高表示(標高に無効表示)
                    gps.time.hour() + JST_OFFSET, // GPSから時を取得(GMT->JST変換)
                    gps.time.minute());           // GPSから分を取得
        }

        lcd.setCursor(0, 0);            // カーソルを始点へ移動(1行目)
        lcd.print(str);                 // LCDにデータ表示
        lcd.setCursor(0, 1);            // カーソルを2行目に移動
        sprintf(str, "SPEED:%4dkm/h  ", // 速度表示
                (int)gps.speed.kmph()); // GPSから速度を取得
        lcd.print(str);                 // LCDにデータ表示
        SearchingSignalFlag = OFF;      // 未受信時ビープ再生フラグ ON
    }
    else // 測位無効の場合
    {
        SearchingSignalBeep();         // 未受信時ビープ再生
        lcd.setCursor(0, 0);           // カーソルを始点へ移動
        lcd.print("Searching Signal"); // 1行目メッセージ表示
        lcd.setCursor(0, 1);           // カーソルを2行目へ移動
        lcd.print("Just a moment..."); // 2行目メッセージ表示
        playFlag = OFF;                // 受信時ビープ 再生フラグ OFF
    }
    delay(MAIN_DELAY);
}

//********** 起動音再生 **********
static void play98()
{
    tone(SPEAKER_PIN, TONE_HIGT, SOUND_DELAY); // 高音再生
    delay(SOUND_DELAY);                        // 待ち
    tone(SPEAKER_PIN, TONE_LOW, SOUND_DELAY);  // 低音再生
    delay(SOUND_DELAY);                        // 待ち
    noTone(SPEAKER_PIN);                       // 再生停止
}

//********** 電文受信時ビープ再生 **********
static void signalReceivedBeep()
{
    // 電文受信時にビープ再生を行う
    if (playFlag == OFF) // 再生フラグOFFか
    {
        tone(SPEAKER_PIN, RECEIVE_TONE_LOW, SOUND_DELAY);  // 高音再生
        delay(SOUND_DELAY);                                // 待ち
        tone(SPEAKER_PIN, RECEIVE_TONE_HIGT, SOUND_DELAY); // 低音再生
        delay(SOUND_DELAY);                                // 待ち
        noTone(SPEAKER_PIN);                               // 再生停止
        playFlag = ON;                                     // 受信時ビープ再生フラグ ON
    }
}

//********** 電文未受信時ビープ再生 **********
static void SearchingSignalBeep()
{
    // 電文未受信時にビープ再生を行う
    if (SearchingSignalFlag == OFF) // 再生フラグOFFか
    {
        tone(SPEAKER_PIN, SEARCH_SIGNAL_TONE, SEARCH_SIGNAL_DELAY); // 高音再生
        delay(SEARCH_SIGNAL_DELAY);                                 // 待ち
        noTone(SPEAKER_PIN);                                        // 再生停止
        SearchingSignalFlag = ON;                                   // 受信時ビープ再生フラグ ON
    }
}
