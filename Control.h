#ifndef CONRTOL_H
#define CONRTOL_H
//#include "Angle.h"
class pid {
  private:
  float derivative, integral, previousError, target;
  int16_t output;
  float kp, ki, kd;
  u_long dt;

  void setConstant(float p, float i, float d) {
    if (p < 0 || i < 0 || d < 0) {
      kp = 0.0;
      ki = 0.0;
      kd = 0.0;
    } else {
      kp = p;
      ki = i;
      kd = d;
    }
  }
public:
  pid(float p, float i, float d) {
    setConstant(p, i, d);
    reset();
  }

  void reset() {
  integral = derivative = previousError = 0; // just to avoid garbage values.
  }
  /*void setTarget(float target) {
    this->target = target;
  }*/
  
  int16_t compute(float &error, unsigned long &dt) {
    derivative = (error - previousError)*1000000/dt;
    integral += (error * dt)/1000000;
    previousError = error;

    output = int16_t(kp * error) + ki * integral + kd * derivative;
    return constrain(output, -155, 155);
  }
};

#endif