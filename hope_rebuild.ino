#include "Angle.h"
#include "Control.h"
#include "Motor.h"
#include "TCP.h"
#define pass (void)0
mpu m;
bool server_status = true;
pid p(40, 0., 1.3);
motor a(26, 27, 25, 33);
tcp server("narzo 50A", "123456789");
unsigned long dt =0;
float target = 0.0;
float angle;
unsigned long now = 0;
unsigned long lastMeasured = 0;
bool iniT = false;
bool dmpInit = false;
float error = 0;
int16_t output;
void setup() {
  a.setPWM(13, 32);  // setting up pwm pins
  Serial.begin(115200);
  server_status = server.connect();
  if(server_status){
    server.StartServer();
  }
  if (m.initialize()) { // initializing mpu
    Serial.println("mpu initialized");
     if(server_status){
      server.sendText("mpu initialized");
    }
    iniT = true;
  }
  else{
    Serial.println("terminated");
     if(server_status){
      server.sendText("terminated");
    }
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
    if (dt >= 10000) {
      m.update();
      angle = m.getYaw();
    } else pass;
      
      error = angle - target;
      output = p.compute(error, dt);
    lastMeasured = now;
  } else pass;

    a.forward(output,error)
    if(server_status){
      server.send(angle, now);
    }
}
