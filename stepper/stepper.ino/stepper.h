#pragma once
#include <AccelStepper.h>

class Stepper
{
private:
  AccelStepper stepper;
  float currentSpeed;

  String name;

public:
  Stepper(int stepPin, int dirPin, String name) : stepper(AccelStepper::DRIVER, stepPin, dirPin)
  {
    this->name = name;
    currentSpeed = 0;
  }

  void setSpeed(float speedRatio)
  {
    currentSpeed = speedRatio;
    // Map speed ratio to the range of speeds for your stepper motor
    float mappedSpeed = speedRatio * 1000; // Adjust multiplier as needed
    stepper.setSpeed(mappedSpeed);
  }

  void clockwise()
  {
    stepper.setAcceleration(1000);
    stepper.moveTo(1000); // Move clockwise
  }

  void counterclockwise()
  {
    stepper.setAcceleration(1000);
    stepper.moveTo(-1000); // Move counterclockwise
  }

  void run()
  {
    stepper.runSpeed();
  }

  float getCurrentSpeed()
  {
    return currentSpeed;
  }

  String getName()
  {
    return name;
  }
};