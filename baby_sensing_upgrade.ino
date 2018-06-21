#include <SoftwareSerial.h>

int blueTx=3;   //Tx (보내는핀 설정)at
int blueRx=2;   //Rx (받는핀 설정)
SoftwareSerial swSerial(blueTx, blueRx); 
int sensorPin = A0;        //pin number to use the ADC
float sensorValue = 0;      //initialization of sensor variable, equivalent to EMA Y
int count = 0;
int count2 = 0;
int freq = 0;
int h = 0;
int h_before = 0;
int timecount = 0;
int peakcnt = 0;
float sensorVal_High = 0;
float sensorVal_Low = 0;
int amp = 0;
int temp =0;
int timecount2 = 0;
int max_min_cnt = 0;
int min_freq = 0;
int max_freq = 0;
int freq2 = 0;
int freq3 = 0;
int start1 = 1;     //min_freq, max_freq 구하는 루프 
int start2 = 0;     //아기 울음 소리 감지 루프
int temp_off = 0;
uint8_t data=0;


void setup(){
  Serial.begin(115200);
  swSerial.begin(9600); //블루투스 시리얼
  analogWrite(9,0);   
  }

void loop(){
  sensorValue = analogRead(sensorPin);    //read the sensor value using ADC
  timecount ++; 
  
  h = sensorValue;
      
  if(h_before > h){
     sensorVal_High = h_before;    //소리 데시벨 최대값    
     peakcnt=1;    
  }
      
  if(peakcnt==1){
     if(h_before < h){
       sensorVal_Low = h_before;   //소리 데시벨 최소값
       peakcnt=0;
       amp = sensorVal_High-sensorVal_Low;   //진폭 추출
       count++;
     }
  }
  if(timecount==100){
     count2=count;
  }
          
  if(timecount==500){
     freq = (count-count2)*20.017;     //실시간 주파수 추출
           
     if(freq<5000){
       timecount = 0;
       count = 0;       
     }
    
      
     if(start1==1){        //초기값 start=1;
    
        if(amp>20){

          if(timecount2==0){
            freq2 = freq;          //최소값       
          }    
          if(timecount2!=0){
            if(freq > freq3){
              freq3 = freq;         //최대값
            }
          }
          if((freq - freq2 < 500)&&(freq - freq2 > -2)){   
            max_min_cnt ++;
            timecount2 ++;          //freq2 값 유지
          }
          else{
            max_min_cnt = 0;
            timecount2 = 0;         //freq2 새롭게 갱신
          }
      
    
        if((max_min_cnt==6)&&(freq3 - freq2 < 600)&&(freq3 - freq2 > 0)){      //이렇게 자세하게 안쓰면 오류남
        
          min_freq = freq2-20;   
          max_freq = freq3+20;      //max, min 좀 더 안전하게 +- 10    
   
          Serial.print("min_freq : ");
          Serial.println(min_freq);
          Serial.print("max_freq : ");
          Serial.println(max_freq);
          start1=0;                 //현재 start1 if문 중단
          start2=1;                 //start2 if문 시작
        
        }
      }
    }

     
     if(start2==1){
        data = swSerial.read();
           if(int(data)==50){
             swSerial.write("0");
              analogWrite(9,0);
              temp = 0;
           }
           if((freq<max_freq)&&(freq>min_freq)&&(amp>20)){         
              temp++;
              temp_off=0;                                       

              Serial.print("FREQ : ");
              Serial.print(freq);
              Serial.print(" / temp : ");
              Serial.println(temp);
           }
          if(((freq>max_freq)||(freq<min_freq))&&(amp>20)){     
             temp = 0;
          }
          if((freq>max_freq)||(freq<min_freq)){     
             temp_off++;
          }
          if(temp>5){       //애기가 운다                    
            Serial.println("Baby is crying");
            analogWrite(9,255);
            data = swSerial.read();
            swSerial.write("1111");        
       
            temp=0;        
            start1=0;  
            start2=1;                 
          }
          if(temp_off>100){     //애기 안운다고 판단
             Serial.println("Baby is sleeping");
             analogWrite(9,0);
             swSerial.write("0");
             start1 = 0;
             start2 = 1;
             temp_off = 0;
          }      
     }  
   }    
    h_before = h;
    delay(0.5);   //ms 단위
}

