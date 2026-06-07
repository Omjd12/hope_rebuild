#ifndef MOTOR_H
#define MOTOR_H


class motor {
private:
  uint8_t a1, a2, b1, b2, pwm1, pwm2;
  int16_t pwmL, pwmR;
  uint8_t baseSpeedFr = 200;
  uint8_t baseSpeedBk = 150;
  int8_t correctionLeft = 0;
  int8_t correctionRight = 0;



public:
  motor(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
    a1 = x1;
    a2 = x2;
    b2 = y2;
    b1 = y1;

    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(b1, OUTPUT);
    pinMode(b2, OUTPUT);
  }
  void setPWM(uint8_t p1, uint8_t p2) {
    pwm1 = p1;
    pwm2 = p2;

    pinMode(pwm1, OUTPUT);
    pinMode(pwm2, OUTPUT);
  }

  void forward(int8_t &output, float &error) {
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(b1, LOW);  
    digitalWrite(b2, HIGH);
      correctionLeft = -(output);
      correctionRight = output;
    
    analogWrite(pwm1, (baseSpeedFr + correctionLeft));
    analogWrite(pwm2, (baseSpeedFr + correctionRight));
    Serial.print(error); // its actually printing the angle since the target angle for forward movenent is 0.00
    Serial.print("\t");
    Serial.print(correctionLeft);
    Serial.print("\t");
    Serial.println(correctionRight);
  }

  void backward(int8_t &output, float &error){
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(b1, HIGH);  
    digitalWrite(b2, LOW);

    analogWrite(pwm1, (baseSpeedBk - correctionLeft));
    analogWrite(pwm2, (baseSpeedBk - correctionRight));
    Serial.print(error);
    Serial.print("\t");
    Serial.print(correctionLeft);
    Serial.print("\t");
    Serial.println(correctionRight);
  }
};
#endif
