#include "telemetry.h"

#include <stdio.h>
#include <math.h>
#include <string.h>


enum TelemetryCommand TelemetryJudgeCommand(const char* recv){
    if(strcmp(recv, "rebt") == 0){
        return CmdReboot;
    }
    if(strcmp(recv, "escp") == 0){
        return CmdEscape;
    }
    if(strcmp(recv, "chck") == 0){
        return CmdCheckSensors;
    }
    if(strcmp(recv, "clpr") == 0){
        return CmdCloseParachute;
    }
    if(strcmp(recv, "oppr") == 0){
        return CmdOpenParachute;
    }

    return CmdUnknown;
}


void ExceptionFormatter(float *val, int limit)
{
    if (*val >= (float)limit)
    {
        *val = 0.0f;
    }
}

void TelemetryGetFormatA(char* buf, int sequence, float time, float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ)
{
    ExceptionFormatter(&time, 1000);
    ExceptionFormatter(&accelX, 100);
    ExceptionFormatter(&accelY, 100);
    ExceptionFormatter(&accelZ, 100);
    ExceptionFormatter(&gyroX, 100);
    ExceptionFormatter(&gyroY, 100);
    ExceptionFormatter(&gyroZ, 100);

    sprintf(buf, "A%d:%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f;", sequence, time, accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
}

void TelemetryGetFormatB(char* buf, int sequence, float time, float temperature, float pressure)
{
    ExceptionFormatter(&time, 1000);
    ExceptionFormatter(&temperature, 100);
    ExceptionFormatter(&pressure, 10000);

    sprintf(buf, "B%d:%.4f,%.4f,%.4f;", sequence, time, temperature, pressure);
}

void TelemetryGetFormatC(char* buf, int sequence, float time, float longitude, float latitude)
{
    ExceptionFormatter(&time, 1000);
    ExceptionFormatter(&longitude, 1000);
    ExceptionFormatter(&latitude, 100);

    sprintf(buf, "C%d:%.4f,%.6f,%.6f;", sequence, time, longitude, latitude);
}

void TelemetryGetFormatK(char* buf, int sequence, float time, float accelX, float accelY, float accelZ)
{
    float acc = sqrtf(accelX * accelX + accelY * accelY + accelZ * accelZ);

    ExceptionFormatter(&time, 1000);
    ExceptionFormatter(&acc, 100);

    sprintf(buf, "K%d:%.4f,%.4f;", sequence, time, acc);
}

void TelemetryGetFormatL(char* buf, int sequence, float time)
{
    ExceptionFormatter(&time, 1000);

    sprintf(buf, "L%d:%.4f;", sequence, time);
}

void TelemetryGetFormatM(char* buf, int sequence, float time)
{
    ExceptionFormatter(&time, 1000);

    sprintf(buf, "M%d:%.4f;", sequence, time);
}

void TelemetryGetFormatZ(char* buf, int sequence, const char *msg)
{

    sprintf(buf, "Z%d:%s;", sequence, msg);
}
