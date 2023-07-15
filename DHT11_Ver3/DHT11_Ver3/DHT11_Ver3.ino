/*
*******************************************************************************
* DHT11で読み取った温度を表示するだけ
* date：2023/7/15
*******************************************************************************
*/
#include <M5Stack.h>
#include <DHT11.h>
#include <string>

DHT11 dht11(5);

void setup(){
 
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextSize(5);
}
  
 
void loop() {
  float h = dht11.readHumidity();
  float t = dht11.readTemperature();
    
  M5.Lcd.setCursor(0, 0); 
  M5.Lcd.print("temp:");
  M5.Lcd.print(t);
  M5.Lcd.print("\nhumi:");
  M5.Lcd.println(h);

  delay(2000);
  

}
