#pragma once

#include "Vec3.h"
#include "Quaternion.h"


struct Datas
{
  //Current values
  float time, deltaTime;//[s]
  float temperature;//[C]
  float pressure;//[Pa]
  Vec3 accel, gyro, magn;//[G, dps, gauss]
  float latitude, longitude;//
  float altitude;//[m]
  Quaternion quaternion;//(x, y, z, w)

  //Special values
  //bootTime is the time when modules were initialized
  float bootTime, launchTime, detachTime, decelerationTime, landingTime;//[s]

  //Max, Min
  float maxAltitude;//[m]
};