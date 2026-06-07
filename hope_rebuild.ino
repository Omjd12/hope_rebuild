#include "Angle.h"
#include "Control.h"
#include "Motor.h"
#define pass (void)0
mpu m;
pid p(1, 0, 0);
motor a(26, 27, 25, 33);
unsigned long dt =0;
float target = 0.0;
float angle;
unsigned long now = 0;
unsigned long lastMeasured = 0;
bool iniT = false;
bool dmpInit = false;
float error = 0;
int8_t output;
void setup() {
  a.setPWM(13, 32);  // setting up pwm pins
  Serial.begin(115200);
  if (m.initialize()) { // initializing mpu
    Serial.println("mpu initialized");
    iniT = true;
  }
  else{
    Serial.println("terminated");
    while(1);
  }
  if (iniT) {
    dmpInit = m.dmpInit(); // initializing dmp 
  }
  m.caliberate(dt);
  p.reset(); // refer the control.h library
 // p.setTarget(target);
}

void loop() {
  if ( m.check()) {
    now = micros();
    dt = now - lastMeasured;
    if (now - lastMeasured >= 10000) {
      m.update();
      lastMeasured = micros();
    } else pass;
  } else pass;
    angle = m.getYaw();
    error = angle - target;
    output = p.compute(error, dt);
    a.forward(output, error);
}
