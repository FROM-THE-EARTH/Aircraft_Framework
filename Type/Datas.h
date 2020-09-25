#pragma once

#include "Vec3.h"

struct Datas {
  float time;
  float temperature;
  float pressure;
  Vec3 accel;
  Vec3 gyro;
  Vec3 magn;
  float latitude;
  float longitude;
  float altitude;
  float maxAltitude;
};