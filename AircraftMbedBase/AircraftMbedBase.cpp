#include "AircraftMbedBase.h"
#include "Utils.h"

bool AircraftMbedBase::initialize()
{

  // IM920
  receiver_->initialize();
  transmitter_->initialize();
  if (receiver_->isAvailable())
  {
    receiver_->attach(this, &AircraftMbedBase::onReceive);
  }

  // LSM9DS1
  lsm_->initialize();

  // LPS331
  lps_->initialize();

  // SD
  /*fp = fopen("/sd/data.csv", "w");
  if(fp != NULL) {
      fprintf(fp,"time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,longitude,latitude\r\n");
  }*/

  // GPS

  // setting
  lps_->setResolution(LPS331_I2C_PRESSURE_AVG_512, LPS331_I2C_TEMP_AVG_128);
  lps_->setDataRate(LPS331_I2C_DATARATE_25HZ);

  // datas
  datas.maxAltitude = -1000;

  transmitter_->transmit("Initialized");

  timer_.start();

  return isReady();
}

bool AircraftMbedBase::isReady(bool showDetail)
{
  bool allModulesAvailable = true;

  allModulesAvailable &= transmitter_->isAvailable();
  allModulesAvailable &= receiver_->isAvailable();
  allModulesAvailable &= lps_->isAvailable();
  allModulesAvailable &= lsm_->isAvailable();

  if (showDetail)
  {
    transmitter_->transmit(transmitter_->status());
    transmitter_->transmit(receiver_->status());
    transmitter_->transmit(lps_->status());
    transmitter_->transmit(lsm_->status());
  }
  transmitter_->transmit("Modules: " +
                         std::string(allModulesAvailable ? "OK" : "NG"));

  return allModulesAvailable;
}

void AircraftMbedBase::end()
{
  // end processing
  timer_.stop();
}

void AircraftMbedBase::waiting() {}

void AircraftMbedBase::waitingLaunch()
{
  if (datas.accel.length() > launchThreshold)
  {
    beginRecord();
    scene = Scene::InFlight;
  }
}

void AircraftMbedBase::inFlight()
{

  writeDatas();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(
          timer_.elapsed_time())
          .count() > landingTime)
  {
    endRecord();
    scene = Scene::Landing;
  }

  if (detachAircraft())
  {
    // detach
  }

  if (openParachute())
  {
    // open parachute
  }
}

void AircraftMbedBase::landing()
{
  // if(gps.isAvailable)
  // transmit gps info
}

void AircraftMbedBase::getDatas()
{
  lsm_->readAccel();
  lsm_->readGyro();
  lsm_->readMag();

  datas.time = std::chrono::duration_cast<std::chrono::milliseconds>(
                   timer_.elapsed_time())
                   .count();

  datas.pressure = lps_->getPressure();
  datas.temperature = lps_->getTemperature();

  datas.accel = Vec3(lsm_->ax, lsm_->ay, lsm_->az);
  datas.gyro = Vec3(lsm_->gx, lsm_->gy, lsm_->gz);
  datas.magn = Vec3(lsm_->mx, lsm_->my, lsm_->mz);

  datas.altitude =
      Utils::calcAltitude(basePressure, datas.pressure, datas.temperature);

  if (datas.altitude > datas.maxAltitude)
  {
    datas.maxAltitude = datas.altitude;
  }

  // datas.longitude;
  // datas.latitude;
}

void AircraftMbedBase::writeDatas()
{
  // sd write
  transmitter_->transmit("Writing");
}

void AircraftMbedBase::onReceive()
{
  switch (checkCommand(receiver_->receive()))
  {
  case Commands::ResetMbed:
    __NVIC_SystemReset();
    break;

  case Commands::EscapePreparing:
    scene = Scene::ReadyToLaunch;
    break;

  case Commands::CheckSensors:
    isReady(true);
    break;

  case Commands::ClosingServo:
    // Close servo
    break;

  case Commands::None:
    transmitter_->transmit("Invalid command");
    break;
  }
}

AircraftMbedBase::Commands AircraftMbedBase::checkCommand(std::string recv)
{

  if (recv == "reset")
  {
    return Commands::ResetMbed;
  }
  if (recv == "escape")
  {
    return Commands::EscapePreparing;
  }
  if (recv == "check")
  {
    return Commands::CheckSensors;
  }
  if (recv == "svclose")
  {
    return Commands::ClosingServo;
  }
  return Commands::None;
}