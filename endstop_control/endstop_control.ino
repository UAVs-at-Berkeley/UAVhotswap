#include <AccelStepper.h>
#include <MultiStepper.h>

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_StepperMotor *belt_stepper = AFMS.getStepper(200, 2);

//TODO: Change these
const int startStop = 3; 
const int endStop = 12;

const int resetButton = 5;

void back() { // go towards the start stop
  Serial.println("Stepped back");
  belt_stepper->onestep(FORWARD, SINGLE);
}
void forward() {  // go away from start stop
  Serial.println("Stepped forward");
  belt_stepper->onestep(BACKWARD, SINGLE);
}

void release() {
  belt_stepper->release();
}

int state = 0;

int location = 0;

void setup() {
  // put your setup code here, to run once: 
  pinMode(startStop, INPUT);  
  pinMode(endStop, INPUT);
  pinMode(resetButton, INPUT);

  AFMS.begin();
  
  Serial.begin(9600);
  
  Serial.println("======= STARTING =======");

    
  //reset on start
  int sts = digitalRead(startStop);
  while (!sts) {
    back();
    delay(2);
    sts = digitalRead(startStop);
  }
  Serial.println("-- RESET --");
  release();

  location = 0;
  
  state = 0;
}

int startStopState = 0;

// ALTERNATES between going to end or start stop
void loop() {
  delay(1500);
  
  
  if (state == 0) {
    
    Serial.println("-- Waiting for End stop --");
    int est = digitalRead(endStop);
    while(!est) {
      forward();
      delay(2);
      est = digitalRead(endStop);
    }
    Serial.println("-- FOUND End stop --");
    release();
    state = 1;
  } 
  else {
  
    Serial.println("-- Waiting for Start stop --");
    int sts = digitalRead(startStop);
    while(!sts) {
      back();
      delay(2);
      sts = digitalRead(startStop);
    }
    Serial.println("-- FOUND Start stop --");
    release();
    state = 0;
  }
  
  delay(1500);
//  Serial.print(digitalRead(startStop));
//  Serial.print(" ---- ");
//  Serial.println(digitalRead(endStop));
}
