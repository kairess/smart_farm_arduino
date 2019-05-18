/*   이 스케치는 디지털 핀 12번에 연결된 온습도 센서 DHT11 모듈을 사용하여
*   주변의 온도와 습도를 측정하여 I2C LCD에 결과값을 출력하며 설정된
*   온도값에 따라 릴레이를 동작시킨다.
*/
 
// 온습도 센서 모듈 사용을 위한 라이브러리 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
 
//I2C LCD 사용을 위한 라이브러리
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
// 온습도 센서 설정
#define DHTPIN            12         // DHT 센서 data핀을 12번 핀에 연결한다.
#define DHTTYPE            DHT11     // DHT 11를 사용한다.
 
DHT_Unified dht(DHTPIN,DHTTYPE);     // dht 객체를 형성한다.
 
uint32_t delayMS;
int Relaypin =7; 

//LCD 설정
LiquidCrystal_I2C lcd(0x27,16,2);    // LCD I2C 주소를 설정한다. 16칸2줄LCD 사용
                                     
void setup() {
  Serial.begin(9600);                // 9600 속도로 시리얼 통신 시작

 pinMode(Relaypin, OUTPUT);          // 릴레이를 출력으로 설정
 digitalWrite(Relaypin,HIGH);            // 1채널릴레이OFF(Low level trigger)

  // LCD 초기화
  lcd.init();
  lcd.backlight();                   // 백 라이트를 켠다. (lcd.noBacklight()는 백 라이트 끈다.)
  lcd.setCursor(0,0);               // 첫 번째 줄 첫째 칸
  lcd.print("Hello~~~");
  lcd.setCursor(0,1);               // 두 번째 줄 첫째 칸
  lcd.print("DHT Sensor Start");
  delay(1000);  //1초 딜레이
   lcd.clear(); 
 // 온습도 센서 모듈 시작
  dht.begin();             
  Serial.println("DHT11 Unified Sensor Example");
 
  // 온도센서 정보 출력 (UART)
  sensor_t sensor;
  
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print ("Sensor:    "); Serial.println(sensor.name);
  Serial.print ("Driver Ver: "); Serial.println(sensor.version);
  Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
  Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print ("Resolution:"); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
 
  // 습도센서 정보 출력 (UART)
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print ("Sensor:     "); Serial.println(sensor.name);
  Serial.print ("Driver Ver: "); Serial.println(sensor.version);
  Serial.print ("Unique ID:  "); Serial.println(sensor.sensor_id);
  Serial.print ("Max Value:  "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print ("Min Value:  "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
 
  // 센서를 읽는 시간
  delayMS =sensor.min_delay /1000;
}
 
void loop() {
  // Delay between measurements.
  delay(delayMS);
  
  sensors_event_t event;  
  dht.temperature().getEvent(&event);  // Get temperature event and print its value. 
  if(isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  
  else{
    //UART
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    //LCD 
    lcd.setCursor(0,0);                             // 첫 번째 줄 첫째 칸
    lcd.print("Temp    : " );                       // 메세지 출력
    lcd.print(event.temperature,0);             // 측정 온도 소숫점 없이 출력
    lcd.print(" [C]");                                // 단위 출력
    //온도에 따른 릴레이 동작
      if(int(event.temperature) <= 27)  	       //기준온도 27도 이하
      {
        digitalWrite(Relaypin,HIGH);              // 1채널릴레이 OFF
      }
      else
      {
        digitalWrite(Relaypin,LOW);              // 1채널릴레이 ON
      }
  }
  
  dht.humidity().getEvent(&event);   // Get humidity event and print its value.
  if(isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else{
    //UART
    Serial.print("Humidity: "); 
    Serial.print(event.relative_humidity);
    Serial.println("%");
    //LCD
    lcd.setCursor(0,1);                             // 두 번째 줄 첫째 칸
    lcd.print("Humidity: " );                       // 메세지 출력
    lcd.print(event.relative_humidity,0);        // 측정 습도 출력
    lcd.print(" [%]");                                // 단위 출력    
  }


  // LCD에 온도와 습도 결과 나타내기
 delay(1000);                      // 안정적으로 값을 읽기 위해 1초(1s=1000ms) 지연
}
