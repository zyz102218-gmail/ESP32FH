



void setup() {
  Serial.begin(115200);
}


void loop() {
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  
  delay(1500);
}

//数据用#号包裹，以便app采用字符串切割，分割出来数据，#23#80#on，即#温度#湿度#按钮状态，
//小程序端会根据#号分割字符串进行取值，以便显示