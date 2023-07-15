/*
*******************************************************************************
* DHT11で読み取った温度をHTTPでPOSTするサンプル
* date：2022/6/26
*******************************************************************************
*/
#include <M5Stack.h>
//#include <DHT11.h>
#include <string>


//PIN5にDHT11を接続する設定
#define DHTPIN 5
#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

void setup(){

  M5.begin(); 
  M5.Lcd.print("Hello World");
  //dht.begin();

}


void loop() {
  M5.Lcd.setCursor(0, 0); 
  //float h = dht.readHumidity();
  //float t = dht.readTemperature();
    
  M5.Lcd.print("temp:");
  //M5.Lcd.print(t);
  M5.Lcd.print("\nhumi:");
  //M5.Lcd.println(h);
  delay(2000);
}
