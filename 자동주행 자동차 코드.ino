#include <Servo.h>

int in1 = 6;  //모터 a의 +    
int in2 = 5;  //모터 a의 -
int in3 = 4;  //모터 b의 +
int in4 = 3;  //모터 b의 -
int ENA = 7;  //모터 a의 속도를 결정하는 핀
int ENB = 2;  //모터 b의 속도를 결정하는 핀
int trig = 8; //초음파를 보내는 핀
int echo = 9; //초음파를 받는 핀
int servopin = 10; //서브모터 제어하는 핀
int left, right;

Servo servo; //객체 생성
  
void setup() {
  pinMode(in1, OUTPUT); //정보를 내보내는 핀은 OUTPUT 
  pinMode(in2, OUTPUT);      
  pinMode(in3, OUTPUT);      
  pinMode(in4, OUTPUT);       
  pinMode(ENA, OUTPUT);      
  pinMode(ENB, OUTPUT);   
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); //정보를 받는 핀이라 INPUT  

  servo.attach(servopin); //서브모터 연결
  servo.write(90); //서브모터를 90도로 초기화 

  Serial.begin(9600);
}

long dis()
{
  float duration, dis = 0; //초음파 센서를 이용하기 위한 함수
  for(int i=0; i<5; i++){
    digitalWrite(trig, HIGH); 
    delay(10); 
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    
    dis += 34000 * duration / 1000000.0 / 2.0;
   
    delay(10);
  }

  return (dis/5.0); //평균 리턴
}

void Car_front(int v)
{
   digitalWrite(in1, HIGH);  
   digitalWrite(in2, LOW);    
   analogWrite(ENA, v);   
   digitalWrite(in3, HIGH);      
   digitalWrite(in4, LOW);    
   analogWrite(ENB, v); 
}

void Car_rear(int v)
{
   digitalWrite(in1, LOW);  
   digitalWrite(in2, HIGH);    
   analogWrite(ENA, v);    
   digitalWrite(in3, LOW);      
   digitalWrite(in4, HIGH);    
   analogWrite(ENB, v); 
}

void Car_left(int v)
{
    digitalWrite(in1, HIGH);     
    digitalWrite(in2, LOW);    
    analogWrite(ENA, v);    
    digitalWrite(in3, LOW);      
    digitalWrite(in4, HIGH);    
    analogWrite(ENB, v);  
}

void Car_right(int v)
{
   digitalWrite(in1, LOW);   
   digitalWrite(in2, HIGH);    
   analogWrite(ENA, v);    
   digitalWrite(in3, HIGH);      
   digitalWrite(in4, LOW);    
   analogWrite(ENB, v);      
}

void Car_stop()
{
    digitalWrite(in1, LOW);     
    digitalWrite(in2, LOW);     
    digitalWrite(in3, LOW);      
    digitalWrite(in4, LOW);  
}

void loop() 
{     
  float front_dis,left_dis,right_dis;
  front_dis = dis(); //거리측정(5회 평균)

  Serial.println(front_dis);
  
  if (front_dis > 10)  
  {
     Car_front(130); //차 앞으로
  }
  else
  {
     Car_stop(); //카 정지
     servo.write(86-80);    // 왼쪽바라보기
     delay(400);
     left_dis=dis(); //거리측정(10회 평균)
     delay(100);
     servo.write(86+80);   // 오른쪽 바라보기
     delay(400);
     right_dis=dis(); //거리측정(10회 평균)
     delay(100); 
     servo.write(90);   // 정면 바라보기
     delay(100); 
          
       if(left_dis <= 10 && right_dis<= 10)
       {      
           Car_rear(130);  //좌우에 공간이 없을 때  후진
           delay(10);
       }
       else
       {
          if(left_dis <= right_dis)
          {
              Car_left(130);       //좌회전
              delay(200);
          }
          else
          {
              Car_left(130);       //좌회전
              delay(200);
          }
        }
   }
}
