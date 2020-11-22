#pragma once

#include "Vec3.h"

struct Datas
{
  //Current values
  float time, deltaTime;            //[s]
  float temperature;                //[C]
  float pressure;                   //[Pa]
  Vec3 accel;                       //[G]
  Vec3 gyro;                        //[dps]
  Vec3 magn;                        //[gauss]
  Vec3 largeAcc;                    //[G] for openingshock
  float latitude, longitude = 0.0f; //[degree]
  float altitude;                   //[m]
  float roll, pitch, yaw;           //[degree]

  //Special values
  //bootTime is the time when modules were initialized
  float bootTime, launchTime, detachTime, decelerationTime, landingTime = 0.0f; //[s]

  //Max, Min
  float maxAltitude = -1000.0f; //[m]
};