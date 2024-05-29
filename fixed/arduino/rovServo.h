#pragma once
//#include <ArduinoSTL.h>
//#include <vector>
#include <Servo.h>

class rovServo {
  byte pin;
  Servo servo;
  String name;
  int angle;
  int zeroAngle;
  int range;

  // 0 is the minimum value for the servo
  static const int MIN = 0;
  // 180 is the maximum value for the servo
  static const int MAX = 180;

  int mapAngle(float input) {
    return zeroAngle + (input*range);
  }

  public:
    rovServo(byte pin, String name, int zeroAngle = 90, int range = 30) {
      this->pin = pin;
      this->name = name;
      this->zeroAngle = zeroAngle;
      this->angle = zeroAngle;
      this->range = range;
    }

    void init() {
      servo.attach(pin);
      reset();
    }

    void setAngle(float angle) {
      this->angle = mapAngle(angle);
      servo.write(this->angle);
    }

    void reset() {
      angle = zeroAngle;
      servo.write(angle);
    }

    int getAngle() {
      return angle;
    }

    String getName() {
      return name;
    }
};
