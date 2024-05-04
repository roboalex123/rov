class Stepper {
  private:
    String name;

    byte directionPin;
    byte stepPin;

    static const int stepDelay = 20;
    static const int delayPerStep = 250;
    static const int numTicks = 100;

  public:
    Stepper(byte newDirectionPin, byte newStepPin, String newName): directionPin(newDirectionPin), stepPin(newStepPin), name(newName) {
      pinMode(directionPin, OUTPUT);
      pinMode(stepPin, OUTPUT);
    }

    String getName() const {
      return name;
    }

    void left() const {
      digitalWrite(directionPin, HIGH);
      for (int i = 0; i < numTicks; i++) {
        digitalWrite(stepPin, HIGH);
        delay(stepDelay);
        digitalWrite(stepPin, LOW);
        delay(delayPerStep);
      }
    }

    void right() const {
      digitalWrite(directionPin, LOW);
      for (int i = 0; i < numTicks; i++) {
        digitalWrite(stepPin, HIGH);
        delay(stepDelay);
        digitalWrite(stepPin, LOW);
        delay(delayPerStep);
      }
    }
};
