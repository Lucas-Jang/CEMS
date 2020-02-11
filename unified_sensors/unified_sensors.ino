
// 1. 온습도 센서 DHT-22
// 2. 음압 센서 one-013
// 3. 진동 센서 one-023
// 4. 미세먼지 센서 pm-2008
#include "DHT.h"
#include <pm2008_i2c.h>

#define DHTPIN 2  //Digital Pin
#define VIBEPIN   3
#define NOISEPIN 0  //Analog Pin
#define DHTTYPE DHT22 //model name
#define SensorLED     13

DHT dht(DHTPIN, DHTTYPE);
PM2008_I2C pm2008_i2c;
unsigned char state = 0;  // for vibe

void setup() {
  Serial.begin(9600);
  // 1.
  dht.begin();
  // 2.
  pinMode(VIBEPIN, INPUT);
  attachInterrupt(1, blink, FALLING);
  // 4.
  pm2008_i2c.begin();
  pm2008_i2c.command();
  
  delay(1000);
}

void loop() {
  // 0.
  long time = millis();
  // 1. 온도, 습도 값 : 250 밀리세컨드정도 소요
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   // 2. 음압센서
  int val = analogRead(0);
  // 3. 진동센서
  // 4. 미세먼지
  uint8_t ret = pm2008_i2c.read();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  
  Serial.print(F("Humidity: "));  // 습도
  Serial.println(h);
  Serial.print(F("%  Temperature: "));  // 온도
  Serial.print(t);
  Serial.println(F("°C "));
  Serial.print(F("Noise : "));
  Serial.println(val,DEC);  //음압
  if(state!=0)
  {
    state = 0;
    Serial.println(F("VIBE STATE HIGH"));  // 진동
    digitalWrite(SensorLED,HIGH);
    delay(500);
  }  
  else{
    Serial.println(F("VIBE STATE LOW"));  // 진동
    digitalWrite(SensorLED,LOW);
  }
  if (ret == 0) {
    Serial.print("PM 2.5 (GRIMM) : : ");
    Serial.println(pm2008_i2c.pm2p5_grimm);
    Serial.print("PM 10 (GRIMM) : : ");
    Serial.println(pm2008_i2c.pm10_grimm);
  } // 미세먼지

}

void blink()//Interrupts function
{  
  state++;
}
