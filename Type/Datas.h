#pragma once

#include "Vec3.h"

struct Datas
{
  //Current values
  float time, deltaTime;     //[s]
  float temperature;         //[C]
  float pressure;            //[Pa]
  Vec3 accel;                //[G]
  Vec3 gyro;                 //[dps]
  Vec3 magn;                 //[gauss]
  float latitude, longitude; //[degree]
  float altitude;            //[m]
  float roll, pitch, yaw;    //[degree]

  //Special values
  //bootTime is the time when modules were initialized
  float bootTime, launchTime, detachTime, decelerationTime, landingTime; //[s]

  //Max, Min
  float maxAltitude; //[m]
};