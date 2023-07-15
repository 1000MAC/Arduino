/*
*******************************************************************************
* DHT11で読み取った温度をHTTPでPOSTするサンプル
* date：2022/6/26
*******************************************************************************
*/
#include <M5Stack.h>
#include "M5StackUpdater.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <string>

#define SSID_ "elecom2g-091e9f"
#define PASSWORD "mxw6hj6wvh4k"

//PIN5にDHT11を接続する設定
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String config_ini;
String URL;
String before_str="1";

void setup(){

  String config_ini;
  String ssid;
  String password;
  
  M5.begin();  //Init M5Core.  初始化 M5Core
  /* SD初期化 */
  
  
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
  M5.Power.begin(); //Init Power module.  初始化电源模块
                    /* Power chip connected to gpio21, gpio22, I2C device
                      Set battery charging voltage and current
                      If used battery, please call this function in your project */
  M5.Lcd.print("Hello World");  // Print text on the screen (string) 在屏幕上打印文本(字符串)
  
  M5.Speaker.begin();       // ノイズ(ポップ音)が出る 
  M5.Speaker.setVolume(1);  // 0は無音、1が最小、8が初期値(結構大きい)
  
  // WiFi接続
  //WiFi.mode(WIFI_STA);
  WiFi.begin();
  //WiFi.setSleep(false);
  for (int i = 0; WiFi.status() != WL_CONNECTED; i++) { // 接続できるまで最大30秒間試行
      if (i >= 30) esp_restart(); // 接続できない場合はリセット
      delay(1000);
      M5.Lcd.print(".");
  }
  M5.Lcd.println(WiFi.localIP());
  
  dht.begin();

  SD.begin();

   /* ファイルオープン */
  File datFile = SD.open("/config.ini");
  if( datFile )
  {
    M5.Lcd.println("File open successful");
    /* サイズ分ループ */
    while(datFile.available())
    {
      config_ini = config_ini + datFile.readString();
    }
    /* ファイルクローズ */   
    datFile.close();
  } 
  else
  {
    M5.Lcd.println("File open error hello.txt");
  }
  /* URL取得 */
  config_ini = config_ini.substring(config_ini.indexOf("#URL\r\n") + 6);
  URL = config_ini.substring(0, config_ini.indexOf("\r\n"));
  M5.Lcd.println(URL);
  delay(1000);
  M5.Lcd.setTextSize(5);
}



String php_post(String url,String str) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");  
  //http.POST("comment="+command);
  http.POST("{\"name\": \""+str+"\"}");
  String response = http.getString();
  http.end();
  return response;
}


/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {


  M5.Lcd.setCursor(0, 0); 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String str=String(t);
  String result_str = php_post(URL,str);    //FericaのIDM番号を認証サーバーに送る
  if(result_str=="1")M5.Lcd.fillScreen(RED);
  if(result_str=="2")M5.Lcd.fillScreen(BLUE);
  if(result_str!=before_str){
    M5.Speaker.beep();        // ビープ開始
    delay(100);               // 100ms鳴らす(beep()のデフォルト)
    M5.Speaker.mute();        //　ビープ停止
  }
  before_str=result_str;
  
  M5.Lcd.print("temp:");
  M5.Lcd.print(t);
  M5.Lcd.print("\nhumi:");
  M5.Lcd.println(h);
  delay(900);
  

}
