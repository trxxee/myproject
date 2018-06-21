#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

int sensorPin = A0;        //pin number to use the ADC
float sensorVal = 0;      //Float
int h = 0;
int cw = 5;
int ccw = 6;

void setup() {
  Serial.begin (9600);
  mp3_set_serial (Serial);      // DFPlayer-mini mp3 module 시리얼 세팅
  delay(1);                     // 볼륨값 적용을 위한 delay
  mp3_set_volume (30);          // 볼륨조절 값 0~30
  pinMode(cw, OUTPUT);
  pinMode(cw, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  int i, j;
  
  analogWrite(9, 0);
  analogWrite(cw, 0);
  analogWrite(ccw, 0);
  mp3_stop();
  sensorVal = analogRead(sensorPin);    //read the sensor value using ADC
  h = sensorVal;
  if(h>500){
    mp3_play (1);    //0001 파일 플레이
    delay (1000);
    
    mp3_next ();    //0002 파일 플레이 , next
    analogWrite(9, 80);
    for(j = 0;j<10;j++) {
      
      for(i=0;i<5;i++) {
        analogWrite(cw, 127);
        analogWrite(ccw, 0);
        delay(10);
        analogWrite(cw, 0);
        analogWrite(ccw, 0);
        delay(100);
      }
  
      for(i=0;i<5;i++) {
        analogWrite(cw, 0);
        analogWrite(ccw, 127);
        delay(10);
        analogWrite(cw, 0);
        analogWrite(ccw, 0);
        delay(100);
      }
    }
  }
  
}

