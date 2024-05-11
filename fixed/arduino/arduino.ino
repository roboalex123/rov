#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "thruster.h"
#include "rovServo.h"
#include "test.h"
using namespace std;

const int NUM_NORMAL_SERVOS = 1;
rovServo normalServos[NUM_NORMAL_SERVOS] = {rovServo(22, "camera", 180)};

const int NUM_THRUSTERS = 6;
Thruster thrusters[NUM_THRUSTERS] = {
  (Thruster(28, "frontLeft")),
  (Thruster(23, "frontRight")),
  (Thruster(25, "midLeft")),
  (Thruster(26, "midRight")),
  (Thruster(27, "backLeft")),
  (Thruster(24, "backRight"))
};

const int NUM_STEPPER_MOTORS = 3;
Test stepperMotors[NUM_STEPPER_MOTORS] = {
  //(Stepper(5, 2, "X")),
  //(Stepper(6, 3, "Y")),
  //(Stepper(7, 4, "Z"))
  (Test(200,5, 2, "X")),
  (Test(200,6, 3, "Y")),
  (Test(200,7, 4, "Z"))
};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_THRUSTERS; i++) {
    thrusters[i].init();
  }

  for (int i = 0; i < NUM_NORMAL_SERVOS; i++) {
    normalServos[i].init();
  }

  /*for (int i = 0; i < 1; i++){
    stepperMotors[i].setFast(200);
  }*/

  delay(7000);
}
int stepperOn = 0;
void loop() {
  String data;
  int smallDelay = 100;

  if (Serial.available())
  {
    data = Serial.readStringUntil('\x7D');
    // Serial.println(data);
    StaticJsonDocument<500> readDoc;
    deserializeJson(readDoc, data);

    for (int i = 0; i < NUM_THRUSTERS; i++)
    {
      thrusters[i].setSpeed(readDoc[thrusters[i].getName()]);
    }

    for (int i = 0; i < NUM_NORMAL_SERVOS; i++)
    {
      normalServos[i].setAngle(readDoc[normalServos[i].getName()]);
    }

    for (int i = 0; i < 1; i++)
    {
      stepperOn = readDoc[stepperMotors[i].getName()];

      /*if (left < -0.01)
      {
        stepperMotors[i].clockwise();
      }
      else if (left > 0.01)
      {
        stepperMotors[i].counterclockwise();
      }*/

      stepperMotors[i].setFast(1200);
    }
  }

  if (stepperOn == 1)
  {
    stepperMotors[0].run();
  } else if (stepperOn == -1) {
    stepperMotors[0].run(true);
  }

}
// hello