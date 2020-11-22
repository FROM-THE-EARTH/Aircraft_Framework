#include "../Avionics.h"
#include <cstdio>

#ifdef PLATFORM_MBED

#include "Utils.h"

bool Avionics::initialize()
{
  // IM920
  transmitter_.initialize();
  transmitter_.transmit("Initializing");

  receiver_.initialize();
  receiver_.attach(this, &Avionics::onReceive);

  // LSM9DS1
  lsm_.initialize();
  lsm_.setAccelScale(LSM9DS1::accel_scale::A_SCALE_4G);
  lsm_.setGyroScale(LSM9DS1::gyro_scale::G_SCALE_500DPS);
  lsm_.setMagScale(LSM9DS1::mag_scale::M_SCALE_4GS);
  lsm_.setAccelODR(LSM9DS1::accel_odr::A_ODR_952);
  lsm_.setGyroODR(LSM9DS1::gyro_odr::G_ODR_952_BW_100);
  lsm_.setMagODR(LSM9DS1::mag_odr::M_ODR_80);

  // LPS33HW
  lps_.set_odr(LPS22HB::lps22hb_odr::LPS22HB_ODR_25HZ);

  // ADXL345
  adxl_.initialize();

  //GPS
  NVIC_SetPriority(UART2_IRQn, 2);

  //SD
  sd_.initialize();
  if (sd_.isInitialized())
  {
    sd_.open("data.csv");
    sd_.write(csvHeader + "\n");
  }
  else
  {
    transmitter_.transmit("Failed to initialize SD Card");
  }

  //end initialization
  transmitter_.transmit("Initialized");

  //initialize datas
  datas.bootTime = 0.0f;

  //initialize time
  timer_.start();

  return isReady();
}

void Avionics::update()
{
  receiver_.update();

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
  allModulesAvailable &= adxl_.isAvailable();
  allModulesAvailable &= sd_.isAvailable();

  if (showDetail)
  {
    transmitter_.transmit(transmitter_.status());
    transmitter_.transmit(receiver_.status());
    transmitter_.transmit(lps_.status());
    transmitter_.transmit(lsm_.status());
    transmitter_.transmit(adxl_.status());
    transmitter_.transmit(sd_.status());
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

  lps_.get();
  datas.pressure = lps_.pressure();
  datas.temperature = lps_.temperature();

  datas.accel = {lsm_.ax, lsm_.ay, lsm_.az};
  datas.gyro = {lsm_.gx, lsm_.gy, lsm_.gz};
  datas.magn = {lsm_.mx, lsm_.my, lsm_.mz};

  datas.largeAcc = adxl_.getOutput();

  datas.altitude =
      Utils::calcAltitude(basePressure, datas.pressure, datas.temperature);

  if (datas.altitude > datas.maxAltitude)
  {
    datas.maxAltitude = datas.altitude;
  }

  gps_.getgps();
  if (gps_.result)
  {
    datas.longitude = gps_.longitude;
    datas.latitude = gps_.latitude;
  }
}

void Avionics::writeDatas()
{
  sd_.write(getCSVFormattedData() + "\n");
}

#endif