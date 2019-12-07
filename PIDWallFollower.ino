/*
* Author: Marcus 
* Date: 2/10/2019
* Aim: PID wall follower
* NEEDS getUltraonicValue TO READ VALUES
*https://forum.makeblock.com/t/need-hint-to-get-mbot-ranger-to-move-through-libraries/8909/3
*http://learn.makeblock.com/en/Makeblock-library-for-Arduino/class_me_encoder_on_board.html
*https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html
*/
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>

//Encoder Motor Declare
MeEncoderOnBoard Encoder_1(SLOT1); // Declare encoder motors
MeEncoderOnBoard Encoder_2(SLOT2);

//Ultrasonic Declare
MeUltrasonicSensor frontUltraSensor(PORT_9);

//PID Declare
int desiredLength = 6;
int speedValue = 100;

int threshold = 5;
int error = 0;
int lastError = 0;
int proportional = 0;
int integral = 0;
int derivative = 0;

int pidValue = 0;

int kP = 5.5;
float kI = 0.01;
float kD = 0.3;


//Motor Declare 
void isr_process_encoder1(void)
{
      if(digitalRead(Encoder_1.getPortB()) == 0){
            Encoder_1.pulsePosMinus();
      }else{
            Encoder_1.pulsePosPlus();
      }
}

void isr_process_encoder2(void)
{
      if(digitalRead(Encoder_2.getPortB()) == 0){
            Encoder_2.pulsePosMinus();
      }else{
            Encoder_2.pulsePosPlus();
      }
}




void setup(){
    //PWM Motor Setup
    Serial.begin(9600);
    attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
    attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
    //Set Pwm 8KHz
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
}
void loop(){
  if (frontUltraSensor.distanceCm() <= 6){  // checks distance if less then (input), if so motors stop and the turnAbsolute function is ran
        Encoder_1.setMotorPwm(0);
        Encoder_2.setMotorPwm(0);
        exit(0);
   }
   int distance = getUltrasonicTopRight();
   error = distance - desiredLength;
   if (abs(error) < threshold){
      integral = integral + error;
   }else{
      integral = 0;
      derivative = error - lastError;
      lastError = error;
      pidValue = error*kP + integral*kI + derivative*kD;
   if (pidValue > 100){
      pidValue = 60;
   }
   else if (pidValue < -100){
      pidValue = -60;
    }
    Encoder_1.setMotorPwm(-speedValue + pidValue);
    Encoder_2.setMotorPwm(speedValue + pidValue);
    Encoder_1.updateSpeed(); //update speed
    Encoder_2.updateSpeed();
    }
}
