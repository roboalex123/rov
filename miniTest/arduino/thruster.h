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

  // private methods
  int mapSpeed(float speed) {
    return ((speed + 1) * 400) + 1100;
  }


  public:
    Thruster(byte pin, String name) {
      this->pin = pin;
      this->name = name;
    }

    void init() {
      servo.attach(pin);
      stop();
      delay(1000);
    }

    void setSpeed(float speed) {
      thrust = mapSpeed(speed);
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
