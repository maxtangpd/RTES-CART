#include <ESP8266WiFi.h>
#include <Servo.h>

//WIFI
const char* ssid = "Leiphone";
const char* password = "12345678";

#define STBY 13
//Motor A
#define AIN1 5
#define AIN2 4 
int PWMA=1;

//Motor B
#define BIN1 12
#define BIN2 15
int PWMB=3;

//sensor forward
#define trigPin 2
#define echoPinforward 16

//sensor left

#define echoPinleft 14

//sensor right

#define echoPinright 0

long durationforward;
int distanceforward;

long durationleft;
int distanceleft;

long durationright;
int distanceright;

int rssi;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  //wifi
  WiFi.begin(ssid, password);
  
  // motor driver
  pinMode(STBY, OUTPUT);
  pinMode(1,FUNCTION_3);
  pinMode(3,FUNCTION_3);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPinforward, INPUT);
 
  pinMode(echoPinright,INPUT);
  pinMode(echoPinleft, INPUT);

  // sensor
  delay(100);
  
}

void move(int motor, int speed, int direction){ 
  digitalWrite(STBY, HIGH);
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1){ // back
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (direction == 0){//forward
    inPin1 = LOW;
    inPin2 = HIGH;
  }

  if(motor == 1) {//left
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{// right
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
  digitalWrite(STBY, LOW);
}

int ForwardDistance_test(){
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durationforward = pulseIn(echoPinforward, HIGH);
  // Calculate the distance:
  distanceforward = durationforward/58;
}

int LeftDistance_test(){
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durationleft = pulseIn(echoPinleft, HIGH);
  // Calculate the distance:
  distanceleft = durationleft/58;
}

int RightDistance_test(){
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durationright = pulseIn(echoPinright, HIGH);
  // Calculate the distance:
  distanceright = durationright/58;
}


int Get_wifi(){
  rssi = WiFi.RSSI();
  
}

int Filter(){
  int FILTER_N = 4;
  int i,j;
  int filter_temp = 0;
  int filter_sum =0;
  int filter_buf[FILTER_N];
  for(i=0; i < 4;i++){
    filter_buf[i] = Get_wifi();
    delay(20);
  }
  for(j = 0; j < 4; j++) {
    for(i = j+1; i < 5; i++) {
      if(filter_buf[i] > filter_buf[j]) {
        filter_temp = filter_buf[i];
        filter_buf[i] = filter_buf[j];
        filter_buf[j] = filter_temp;
      }
    }
  }
   filter_sum = filter_buf[2];
  }

void loop() {
  int a,b,c,d;
  int turn = 0;
  //Serial.println("start");
  ForwardDistance_test();
  delay(30);
  RightDistance_test();
  delay(30);
  LeftDistance_test();
  if(distanceforward < 45 ){

       move(1,500,0); //  turn right 
       move(2,500,1);
       delay(420);
       
    }
    else if(distanceleft < 40){
       
       move(1,500,0); //  turn right 
       move(2,500,1);
       delay(420);  
       
    }
    else if(distanceright < 40){
       
       move(1,555,1); //  turn left 
       move(2,455,0);
       delay(420); 
       ForwardDistance_test();
    }else{   
       a = Filter();
         move(1,500,0);
         move(2,500,0);
         delay(2000); 
         stop();
         delay(1000);
         b = Filter();
         if(b >-55)
           {
            stop();
            delay(50000);
           }
         if(b > a){
           if(a < -70){
            move(1,555,1); 
             move(2,455,1);
             delay(450);
             stop();
           }
           
           else if(abs(b-a)>2){
            move(1,455,0); //  turn right 
             move(2,455,1);
             delay(1400);
             
           }
           if(b>-40) {
           stop();
           }   
    }
  }
}       
