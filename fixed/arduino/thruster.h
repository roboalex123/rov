#pragma once
//#include <ArduinoSTL.h>
//#include <vector>
#include <Servo.h>

class Thruster {
  byte pin;
  Servo servo;
  String name;
  int thrust = 1500;
  float speed = 0;

  // 1500 is the stop value for the ESC
  static const int STOP = 1500;
  // 1100 is the minimum value for the ESC
  static const int MIN = 1100;
  // 1900 is the maximum value for the ESC
  static const int MAX = 1900;

  int thrustClamp(int thrust) {
    if (thrust < MIN) {
      return MIN;
    } else if (thrust > MAX) {
      return MAX;
    } else {
      return thrust;
    }
  }

  // private methods
  int mapSpeed(float speed) {
    float normThrust = thrust + 1;
    int offSet = 50;
    if (normThrust < 1) {
      return (normThrust) * 400 + (1100 - offSet);
    } else if (normThrust > 1) {
      return (normThrust) * 400 + (1100 + offSet);
    } else {
      return (thrust + 1) * 400 + 1100;
    }
  }
  

  public:
    Thruster(byte pin, String name) {
      this->pin = pin;
      this->name = name;
    }

    void init() {
      servo.attach(pin);
      stop();
    }

    void setSpeed(float speed) {
      thrust = thrustClamp(mapSpeed(speed));
      servo.writeMicroseconds(thrust);
      this->speed = speed;
    }

    void stop() {
      thrust = STOP;
      servo.writeMicroseconds(thrust);
      this->speed = 0;
    }

    int getSpeed() {
      return speed;
    }

    String getName() {
      return name;
    }
};
