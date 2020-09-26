#pragma once

#include "Vec3.h"

struct Datas
{
  //Current values
  float time;
  float temperature;
  float pressure;
  Vec3 accel, gyro, magn;
  float latitude, longitude;
  float altitude;

  //Special values
  float launchTime, detachTime, decelerationTime, landingTime;

  //Max, Min
  float maxAltitude;
};