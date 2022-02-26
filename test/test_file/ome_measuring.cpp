void setup(){
    Serial.begin(115200);
    delay(1000); 
    Serial.println("ESP32 measuring Test:");
}

void loop(){
    int sensorValue = analogRead(GPIO15);
    float voltage = sensorValue * (3.3 / 1023.0);
    Serial.println(voltage);
}