/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.10 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.13.11 or later version;
     - for iOS 1.10.3 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID"RemoteXY7.1" // group.number
#define REMOTEXY_WIFI_PASSWORD "" //NO PASSWARD
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 53 bytes
  { 255,4,0,0,0,46,0,17,0,0,0,31,1,106,200,1,1,3,0,5,
  23,67,60,60,32,177,181,31,2,9,150,41,20,0,12,1,31,31,79,78,
  0,79,70,70,0,3,57,152,44,16,131,94,202 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  uint8_t switch_01; // =1 if switch ON and =0 if OFF
  uint8_t select_01; // from 0 to 3

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <Servo.h>
Servo pushballservo;
int servoPin1=12;

#define pwmMotorA 4
#define pwmMotorB 5
#define dirMotorA 0 //A for right
#define dirMotorB 2 // B for left
#define mos 15

int ServoAngle = 0;
int speed = 0;
float angle;
const float MAX_ANGLE = 1.5707963;
const float ZERO_ANGLE = 0.0;
int DEAD_ZONE = 20, NEAR_Y_AXIS_THRESHOLD = 20, NEAR_X_AXIS_THRESHOLD = 20;
int i=0;
void setMotorSpeed(float speedA, float speedB, int dirA, int dirB) { 
  digitalWrite(dirMotorA, dirA);
  digitalWrite(dirMotorB, dirB);
  analogWrite(pwmMotorA, speedA);
  analogWrite(pwmMotorB, speedB);
}

void controlMotors(int joystickX, int joystickY) {
    float speed = sqrt(pow(joystickX, 2) + pow(joystickY, 2));
    speed = constrain(speed*2.55,0,255); // do this
    float angle = atan2(joystickY, joystickX);

    if (abs(joystickX) < DEAD_ZONE && abs(joystickY) < DEAD_ZONE) {
        setMotorSpeed(0, 0, LOW, LOW);
        return;
    }
    if (abs(joystickX) < NEAR_Y_AXIS_THRESHOLD) {
        // Near y-axis: go straight forward or backward
        if (joystickY > 0) {
            setMotorSpeed(speed, speed, HIGH, HIGH);
        } else {
            setMotorSpeed(speed, speed, LOW, LOW);
        }
    } else if (abs(joystickY) < NEAR_Y_AXIS_THRESHOLD) {
        speed = speed*0.5;
        // Near x-axis: turn left or right
        if (joystickX > 0) {
            setMotorSpeed(speed, speed, HIGH, LOW); // Right turn
        } else {
            setMotorSpeed(speed, speed, LOW, HIGH); // Left turn
        }
    } else {
        float speedFactor = abs(speed * joystickY / joystickX);
        speedFactor = constrain(speedFactor,0,255);
        if (angle > 0 && angle <= 1.5707963) {
            setMotorSpeed(speedFactor, speed, HIGH, HIGH);
        } else if (angle > 1.5707963 && angle < 3.1415926) {
            setMotorSpeed(speed, speedFactor, HIGH, HIGH);
        } else if (angle > -1.5707963 && angle < 0) {
            setMotorSpeed(speedFactor, speed, LOW, LOW);
        } else if (angle > -3.1415926 && angle < -1.5707963) {
            setMotorSpeed(speed, speedFactor, LOW, LOW);
        }
    }
}



void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);
  
  // TODO you setup code
  pushballservo.attach(servoPin1);
  pinMode(pwmMotorA , OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);//right
  pinMode(dirMotorB, OUTPUT);//left
  pinMode(mos,OUTPUT);

}

int startTime;
int nowtime;
int count = 0;
void loop() 
{ 
    if(i<=0){
      digitalWrite(dirMotorA,HIGH);
      digitalWrite(dirMotorB,HIGH);
      analogWrite(pwmMotorA, 30);
      analogWrite(pwmMotorB, 30);
      digitalWrite(mos,LOW);
      delay(1500);

      digitalWrite(dirMotorA,LOW);
      digitalWrite(dirMotorB,LOW);
      analogWrite(pwmMotorA, 30);
      analogWrite(pwmMotorB, 30);
      digitalWrite(mos,HIGH);
      delay(1500);
      digitalWrite(mos,LOW);
      digitalWrite(pwmMotorA,LOW);
      digitalWrite(pwmMotorB,LOW);

      pushballservo.write(155);
      delay(500);
      pushballservo.write(90);
      delay(500);
      pushballservo.write(0);
      delay(500);
      i++;
   }
  RemoteXY_Handler ();

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 

  switch (RemoteXY.select_01) {
  case 0:  // If RemoteXY.select_01 is 0, set ServoAngle to 151ㄋ
    ServoAngle = 151;
    break;
  case 1:  // If RemoteXY.select_01 is 1, set ServoAngle to 90
    ServoAngle = 90;
    break;
  case 2:  // If RemoteXY.select_01 is 2, set ServoAngle to 0
    ServoAngle = 0;
    break;
  default: // Handle unexpected values of RemoteXY.select_01
    ServoAngle = 0; // or another safe default value
    break;
  }
  pushballservo.write(ServoAngle);

  if (RemoteXY.switch_01){
    count++;
    if(count <= 1){
      Serial.println("here");
      startTime=millis();
      nowtime = millis()-startTime;
      Serial.print("start:");
      Serial.println(startTime);
    }
  }else{
    count=0;
    digitalWrite(mos, LOW);
    }
  if(nowtime <= 3000){
    Serial.print("nowtime:");
    Serial.println(nowtime);
    analogWrite(mos,150);
    nowtime = millis()-startTime;
    }else digitalWrite(mos,LOW);
  controlMotors(RemoteXY.joystick_01_x, RemoteXY.joystick_01_y); 
}