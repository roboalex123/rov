#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Stepper.h>
//#include "stepper.h"
// testing a stepper motor with a Pololu A4988 driver board or equivalent
// on an Uno the onboard led will flash with each step
// this version uses delay() to manage timing
/*
byte directionPin = 7;
byte stepPin = 4;
int numberOfSteps = 2000;
byte ledPin = 13;
int pulseWidthMicros = 20;  // microseconds
int millisbetweenSteps = 25; // milliseconds - or try 1000 for slower steps


void setup() { 

  Serial.begin(9600);
  Serial.println("Starting StepperTest");
  digitalWrite(ledPin, LOW);

  delay(2000);

  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

/*
  digitalWrite(directionPin, HIGH);
  for(int n = 0; n < numberOfSteps; n++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros); // this line is probably unnecessary
    digitalWrite(stepPin, LOW);

    delay(millisbetweenSteps);

    digitalWrite(ledPin, !digitalRead(ledPin));
  }

  delay(3000);



  digitalWrite(directionPin, LOW);
  for(int n = 0; n < numberOfSteps; n++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros); // probably not needed
    digitalWrite(stepPin, LOW);

    delay(millisbetweenSteps);

    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}

void loop() { 
}
 

Stepper test(5,2,"test");

void setup(){
  test.clockwise();
  test.setSpeed(1.0);
  test.run();
  delay(5000);
  test.setSpeed(0);
  test.run();
}
void loop(){
}


#define pul1 5
#define dir1 2

AccelStepper stepper1(1, pul1,dir1);

void setup(){
  //stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(100);
  stepper1.moveTo(1000);
}

void loop(){
  stepper1.run();
  //stepper1.runToNewPosition(4000);
}

const int step = 5;
const int dir = 2;

AccelStepper stepper(AccelStepper::DRIVER, step, dir);

void setup(){
 stepper.setMaxSpeed(2000);
 stepper.setAcceleration(1000);
}

void loop(){
  stepper.moveTo(1000);
  stepper.run();
}
*/
const int STEPS = 2000; 
Stepper test = Stepper(STEPS,7,4);

void setup(){
  test.setSpeed(60);
}

void loop(){
 test.step(-STEPS);
 delay(500); 
 test.step(STEPS);
 delay(500);
}
