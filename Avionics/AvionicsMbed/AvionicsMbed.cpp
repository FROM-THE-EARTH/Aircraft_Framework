#include "../Avionics.h"

#ifdef PLATFORM_MBED

#include "Utils.h"
#include "Nature.h"

bool Avionics::initialize()
{
  // IM920
  transmitter_.initialize();
  transmitter_.transmit("Initializing");

  receiver_.initialize();
  if (receiver_.isAvailable())
  {
    receiver_.attach(this, &Avionics::onReceive);
  }

  // LSM9DS1
  lsm_.initialize();
  lsm_.setAccelScale(LSM9DS1::accel_scale::A_SCALE_4G);
  lsm_.setGyroScale(LSM9DS1::gyro_scale::G_SCALE_500DPS);
  lsm_.setMagScale(LSM9DS1::mag_scale::M_SCALE_4GS);
  lsm_.setAccelODR(LSM9DS1::accel_odr::A_ODR_952);
  lsm_.setGyroODR(LSM9DS1::gyro_odr::G_ODR_952_BW_100);
  lsm_.setMagODR(LSM9DS1::mag_odr::M_ODR_80);

  // LPS331
  lps_.initialize();
  lps_.setResolution(LPS331_I2C_PRESSURE_AVG_384, LPS331_I2C_TEMP_AVG_64);
  lps_.setDataRate(LPS331_I2C_DATARATE_25HZ);

  // SD
  /*fp = fopen("/sd/data.csv", "w");
  if(fp != NULL) {
      fprintf(fp,csvHeader + "\r\n");
  }*/

  //GPS
  NVIC_SetPriority(UART3_IRQn, 2);

  transmitter_.transmit("Initialized");

  //initialize datas
  datas.bootTime = 0.0f;

  //initialize time
  timer_.start();

  return isReady();
}

void Avionics::update()
{
  receiver_.poll();

  timer_.update();

  datas.time = timer_.now();

  datas.deltaTime = timer_.delta();
}

bool Avionics::isReady(bool showDetail)
{
  bool allModulesAvailable = true;

  allModulesAvailable &= transmitter_.isAvailable();
  allModulesAvailable &= receiver_.isAvailable();
  allModulesAvailable &= lps_.isAvailable();
  allModulesAvailable &= lsm_.isAvailable();

  if (showDetail)
  {
    transmitter_.transmit(transmitter_.status());
    transmitter_.transmit(receiver_.status());
    transmitter_.transmit(lps_.status());
    transmitter_.transmit(lsm_.status());
  }
  transmitter_.transmit("Modules: " + xString(allModulesAvailable ? "OK" : "NG"));

  return allModulesAvailable;
}

void Avionics::end()
{
  // end processing
}

void Avionics::getDatas()
{
  lsm_.readAccel();
  lsm_.readGyro();
  lsm_.readMag();

  datas.pressure = lps_.getPressure();
  datas.temperature = lps_.getTemperature();

  datas.accel = Vec3(lsm_.ax, lsm_.ay, lsm_.az);
  datas.gyro = Vec3(lsm_.gx, lsm_.gy, lsm_.gz);
  datas.magn = Vec3(lsm_.mx, lsm_.my, lsm_.mz);

  datas.altitude =
      Utils::calcAltitude(basePressure, datas.pressure, datas.temperature);

  if (datas.altitude > datas.maxAltitude)
  {
    datas.maxAltitude = datas.altitude;
  }

  if (gps_.result)
  {
    gps_.getgps();
    datas.longitude = gps_.longitude;
    datas.latitude = gps_.latitude;
  }

  transmit(std::to_string((int)datas.longitude) + ", " + std::to_string((int)datas.longitude));
}

void Avionics::writeDatas()
{
  transmitter_.transmit("(" + to_XString(datas.roll) + ", " + to_XString(datas.pitch) + ", " + to_XString(datas.yaw) + ")");
  transmitter_.transmit(to_XString(datas.pressure) + ", " + to_XString(datas.temperature));

  //sd.write(getCSVFormattedData());
}

#endif