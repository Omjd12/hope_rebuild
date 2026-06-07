#ifndef ANGLE_H
#define ANGLE_H
#define pass (void)0
#include <MPU6050_6Axis_MotionApps20.h>
#include <I2Cdev.h>
#include <Wire.h>

class mpu {
private:
  MPU6050 mpu;  // initialize MPU6050
  Quaternion q;
  VectorFloat gravity;  // gravity vector
  float ypr[3];
  uint8_t fifoBuffer[64];  // fifo buffer to get packets
  uint8_t devStatus;
  bool dmpReady = false;
  float yaw = 0.0;
  float YawOffset = 0.0;

public:
  // method to initialize mpu
  bool initialize() {
    Wire.begin();  // initializing wire library
    mpu.initialize();
    if (mpu.testConnection()) {
      Serial.println("check the connection");
      return false;
    } else {
      Serial.println("connection okay");
      return true;
    }
  }

  void caliberate(unsigned long &dt) {
    float_t sum = 0.0;
    /*float_t rate = 1;
    while(rate >= 0.003 ){
      long t= millis();
      for(int i = 0; i<100; i++){
        this->check();
        this->update();
        sum += this->getYaw();
        delay(1);
      }
      dt = millis() - t;
      rate = sum/(dt);
      Serial.println(rate);
      sum = 0.0; 
    }*/
    for(int i = 0; i<7000; i++){
      check();
      update();
      pass;
    }
    for(int i = 0; i<1000; i++){
      check();
      update();
      sum += getYaw(); 
    }
    YawOffset = sum/1000;
  }
  // method to initialize digital motion processing (DMP)
  bool dmpInit() {
    devStatus = mpu.dmpInitialize();

    if (devStatus == 0) {  // the devStatus indicates whether the dmp initialized or have any error : each error has a specific code
      dmpReady = true;
      mpu.setDMPEnabled(dmpReady);
      Serial.println("dmp ready");
      return true;
    } else {
      Serial.print("DMP init failed");
      Serial.println(devStatus);  // print the error code
      return false;
    }
  }
  // this method can be used frequently
  void update() {
    if (!dmpReady) return;
    else if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      yaw = ypr[0] * 180 / M_PI;
    }
  }
  bool check() {
    if (!mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      return true;
    } else return false;
  }
  float getYaw() {
    return yaw - YawOffset;
  }
  void zeroYaw() {
    YawOffset = this->yaw;
  }
};
#endif
