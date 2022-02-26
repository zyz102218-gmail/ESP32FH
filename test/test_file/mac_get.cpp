#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
//   Serial.println('hello world!');
//   Serial.print("ESP Board MAC Address:  ");
//   Serial.println(WiFi.macAddress());
}
 
void loop(){
  Serial.print("hello world!");
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  delay(2000);
}