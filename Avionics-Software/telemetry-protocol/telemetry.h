#ifndef INCLUDE_TELEMETRY_H
#define INCLUDE_TELEMETRY_H

//---------------------------------------------------------------------------
// Received Command
//---------------------------------------------------------------------------

enum TelemetryCommand{
    CmdReboot,
    CmdEscape,
    CmdCheckSensors,
    CmdCloseParachute,
    CmdOpenParachute,
    CmdUnknown
};

enum TelemetryCommand TelemetryJudgeCommand(const char* recv);


//---------------------------------------------------------------------------
// Send Message
//---------------------------------------------------------------------------

//Telemetry Format A
//-Get {time, 3-axis-accel, 3-axis-gyro} formatted message
void TelemetryGetFormatA(char* buf, int sequence, float time, float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ);

//Telemetry Format B
//-Get {time, temperature, pressure} formatted message
void TelemetryGetFormatB(char* buf, int sequence, float time, float temperature, float pressure);

//Telemetry Format C
//-Get {time, longitude, latitude} formatted message
void TelemetryGetFormatC(char* buf, int sequence, float time, float longitude, float latitude);

//Telemetry Format K
//-Message on launched
//-Get {time, 3-axis-absolute-accel} formatted message
void TelemetryGetFormatK(char* buf, int sequence, float time, float accelX, float accelY, float accelZ);

//Telemetry Format L
//-Message on parachute opened
//-Get {time} formatted message
void TelemetryGetFormatL(char* buf, int sequence, float time);

//Telemetry Format M
//-Message on detached
//-Get {time} formatted message
void TelemetryGetFormatM(char* buf, int sequence, float time);

//Telemetry Format Z
//-Free message (61 bytes or less)
//-Get {free-message} formatted message
void TelemetryGetFormatZ(char* buf, int sequence, const char* msg);

#endif