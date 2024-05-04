#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "stepper.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {rovServo(22, "camera", 180)};

const int NUM_THRUSTERS = 6;
Thruster thrusters[NUM_THRUSTERS] = {
  (Thruster(28, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(25, "middleLeft")),
  (Thruster(26, "middleRight")),
  (Thruster(27, "backLeft")),
  (Thruster(24, "backRight"))
};

const int NUM_STEPPER_MOTORS = 1;
Stepper stepperMotors[NUM_STEPPER_MOTORS] = {
  (Stepper(5, 2, "X")),
  (Stepper(6, 3, "Y")),
  (Stepper(7, 4, "Z"))
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_THRUSTERS; i++) {
    thrusters[i].init();
  }

  for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
    normalServos[i].init();
  }

  for (int i = 0; i < NUM_STEPPER_MOTORS; i++) {
    stepperMotors[i].init();
  }

  delay(7000);
}

void loop() {
  String data;
  int smallDelay = 100;
  while (!Serial.available()) {
    digitalWrite(13, HIGH);
    delay(smallDelay);
    digitalWrite(13, LOW);
    delay(smallDelay);
  }

  if (Serial.available()) {
    data = Serial.readStringUntil( '\x7D' );
   // Serial.println(data);
    StaticJsonDocument<1000> readDoc;
    deserializeJson(readDoc, data);
 

    for (int i = 0; i < NUM_THRUSTERS; i++) {
      thrusters[i].setSpeed(readDoc[thrusters[i].getName()]);
    }

    for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
      normalServos[i].setAngle(readDoc[normalServos[i].getName()]);
    }

    for (int i = 0; i < NUM_STEPPER_MOTORS; i++) {
      int left = readDoc[stepperMotors[i].getName()];

      if (left == 1) {
        stepperMotors[i].stepLeft();
      } else {
        stepperMotors[i].stepRight();
      }
    }
  }
}
