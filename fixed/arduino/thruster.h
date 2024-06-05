#pragma once
#include <Servo.h>

class Thruster {
  byte pin;
  Servo servo;
  String name;
  int thrust;
  float speed;

  // 1500 is the stop value for the ESC
  static const int STOP = 1500;
  // 1100 is the minimum value for the ESC
  static const int MIN = 1100;
  // 1900 is the maximum value for the ESC
  static const int MAX = 1900;

  // offset for the speed mapping (used to prevent deadzone)
  static const int OFFSET = 50;

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
    float normThrust = speed + 1;
    if (normThrust < 1) {
      return (normThrust) * 400 + (1100 - OFFSET);
    } else if (normThrust > 1) {
      return (normThrust) * 400 + (1100 + OFFSET);
    } else {
      return (speed + 1) * 400 + 1100;
    }
  }
  

  public:
    Thruster(byte pin, String name) {
      this->pin = pin;
      this->name = name;

      thrust = STOP;
      speed = 0.0;
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

    float getSpeed() {
      return speed;
    }

    int getThrust() {
      return thrust;
    }

    String getName() {
      return name;
    }
};
