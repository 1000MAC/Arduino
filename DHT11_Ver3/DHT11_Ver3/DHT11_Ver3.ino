/*
*******************************************************************************
* DHT11で読み取った温度を表示するだけ
* date：2023/7/15
*******************************************************************************
*/
#include <M5Stack.h>
#include <DHT11.h>
#include <string>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "elecom2g-091e9f";
const char* password = "mxw6hj6wvh4k";

WebServer server(80); // WebServerオブジェクト作成

DHT11 dht11(5);

void WiFiConnect(){
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    M5.Lcd.print('.');
  }
  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());
}


void handleTemp(){
  char buf[400];
  
  sprintf(buf, 
    "<html>\
     <head>\
        <title>Temperature Measurement</title>\
     </head>\
     <body>\
     <font size=\"20\">\     
        <h1>Temp Sensor</h1>\
        <p>Temp: %.2f 'C</p>\
        <p>Humi: %.2f 'C</p>\
     </font>\
     </body>\
     </html>",
  dht11.DHT_temp,dht11.DHT_humi);
  server.send(200, "text/html", buf);
  //M5.Lcd.println("accessed on \"/\"");
}

void handleNotFound(){
  server.send(404, "text/plain", "File Not Found\n\n");
  M5.Lcd.println("File Not Found");
}


void setup(){
 
  M5.begin();
  M5.Power.begin();
  WiFiConnect(); // WiFi接続

  //マルチキャストDNSにホスト名を登録
  if(MDNS.begin("m5stack2")){
    M5.Lcd.println("MDNS responder started");
  }

  server.on("/", handleTemp); // URLにアクセスされた際の動作を登録
  server.onNotFound(handleNotFound); // server.onで登録されていないURLにアクセスされた際の動作を登録
  server.begin(); // クライアントからの接続応答待ちを開始
  M5.Lcd.println("HTTP server started");

  
  M5.Lcd.setTextSize(5);


  delay(1000);
  
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setBrightness(50);



}
  
 
void loop() {
  //float h = dht11.readHumidity();
  dht11.readTemperature();
    
  M5.Lcd.setCursor(0, 0); 
  M5.Lcd.print("temp:");
  M5.Lcd.print(dht11.DHT_temp);
  M5.Lcd.print("\nhumi:");
  M5.Lcd.println(dht11.DHT_humi);

  server.handleClient();

  delay(5000);
  

}
