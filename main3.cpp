#include<Arduino.h>
#include<SPI.h>
#include<MFRC522.h>

#include<Servo.h>
Servo servo1;

#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

MFRC522 mfrc(SS_PIN, RST_PIN);

#define echoPin0 A0
#define trigPin0 A1
#define echoPin1 A2
#define trigPin1 A3
#define echoPin2 A4
#define trigPin2 A5
#define servo1Pin 2
int servo1Pos = 0;
long duration0, duration1, duration2, duration3, duration4;
int distance0, distance1, distance2, distance3, distance4;
#define IN1andIN3 8
#define IN2andIN4 7
int ENA_A=6;
int ENA_B=3;
bool d;

void rfidOperation();
int gateOperation();
int convayerUltraSonic();
int convayer1Operation();
int verticalUltraSonic();
int verticalOperation();

void setup(){
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  servo1.attach(servo1Pin);
  pinMode(echoPin0,INPUT);
  pinMode(trigPin0,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(IN1andIN3,OUTPUT);
  pinMode(IN2andIN4,OUTPUT);
  pinMode(ENA_A, OUTPUT);
  pinMode(ENA_B,OUTPUT);
  Serial.println("Scan your PCCID");
  servo1.write(0);
  }

void loop(){
  rfidOperation();
  if(d==true){
    convayerUltraSonic();
    convayer1Operation();
    Serial.print(distance2);
    Serial.print("  ");

    verticalUltraSonic();
    verticalOperation();
    Serial.println(distance0);
    Serial.print("  ");
    horizontalUltraSonic();
    horizontalOperation();
    Serial.println(distance1);
    Serial.println("Access Authorized.");
    }
  }

void rfidOperation(){
  //Serial.println(distance0);
  if(! mfrc.PICC_IsNewCardPresent() && ! mfrc.PICC_ReadCardSerial()){
    return;
    }
  Serial.print("UID tag :");
  String content="";
  byte letter;
  for(byte i=0; i<mfrc.uid.size;i++){
    Serial.print(mfrc.uid.uidByte[i] <0x10 ? " 0" : " ");
    Serial.print(mfrc.uid.uidByte[i],HEX);
    content.concat(String(mfrc.uid.uidByte[i] <0x10 ? " 0" : " "));
    content.concat(String(mfrc.uid.uidByte[i],HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();

  if (content.substring(1) == "97 57 37 3C" || content.substring(2)=="A1 6B 40 43"){
      d=true;
      return d;
    Serial.println();
    delay(7500);
    }
  else{
    Serial.println(" Access denied");
    delay(DENIED_DELAY);
    }
  }

int gateOperation(){
  servo1.write(170);
  delay(15);


  }

int convayerUltraSonic(){
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2=pulseIn(echoPin2,HIGH);
  distance2=(duration2*0.034)/2;
  return distance2;
  }

int convayer1Operation(){
  switch(distance2){
    case 9 ... 21:
    digitalWrite(IN1andIN3, LOW);
    digitalWrite(IN2andIN4, HIGH);
    analogWrite(ENA_A,200);
    break;

    default:
    digitalWrite(IN1andIN3, LOW);
    digitalWrite(IN2andIN4, HIGH);
    analogWrite(ENA_A,0);
    break;
  }
}

 int verticalUltraSonic(){
  digitalWrite(trigPin0, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin0, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin0, LOW);
  duration0=pulseIn(echoPin0,HIGH);
  distance0=(duration0*0.034)/2;
  return distance0;
  }

int verticalOperation(){
  switch(distance2){
  case 0 ... 9:
  switch(distance0){
    case 7 ... 35:
    digitalWrite(IN1andIN3, HIGH);
    digitalWrite(IN2andIN4, LOW);
    analogWrite(ENA_B,255);
    delay(4000);
    digitalWrite(IN1andIN3, LOW);
    digitalWrite(IN2andIN4, HIGH);
    analogWrite(ENA_B,255);
    break;
    }
  }
  }


int horizontalUltraSonic(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1=pulseIn(echoPin1,HIGH);
  distance1=(duration1*0.034)/2;
  return distance1;
  }

int horizontalOperation(){
  if(distance2<=15){
    digitalWrite(IN1andIN3, HIGH);
    digitalWrite(IN2andIN4, LOW);
    analogWrite(ENA_A, 255);
    Serial.println("Moving Forward");
    }
  }
