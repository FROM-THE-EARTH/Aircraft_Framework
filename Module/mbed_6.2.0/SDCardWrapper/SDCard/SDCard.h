/****************************************************************************/
/*  SDCard - Library for Nucleo                                             */
/****************************************************************************/
/*  LEnsE / Julien VILLEMEJANE       /   Institut d'Optique Graduate School */
/****************************************************************************/
/*  Pins                                                                    */
/*      USBTX, USBRX    RS232 communication with computer                   */
/*      D11, D12, D13   SPI communication with SD Card                      */
/*      D4              CS of SD Card                                       */
/****************************************************************************/
/*  Tested on L476RG                                                        */
/****************************************************************************/
#ifndef RPLIDAR_H
#define RPLIDAR_H
 
#include "mbed.h"
// Block devices
#include "SDBlockDevice.h"
// File systems
#include "FATFileSystem.h"
 
class SDCard
{
public:
    /**
    *   Constructor
    *
    * @param miso   SPI communication for SD Card / MISO pin
    * @param mosi   SPI communication for SD Card / MOSI pin
    * @param sck    SPI communication for SD Card / SCK pin
    * @param cs     SPI communication for SD Card / CS pin
    */
    SDCard(PinName miso, PinName mosi, PinName sck, PinName cs);
 
    /**
    *   Mounting SD Card and FileSystem
    *
    * @return error   1 if OK, -1 if not OK
    */
    int mounting(void);
    /**
    *   Opening file on SD Card
    *
    * @param path   directory of the file
    * @param file   name of the file with extension
    * @return error 1 if OK, -1 if not OK
    */
    int opening_file(char *path, const char* filename, const char* mode);
    /**
    *   Closing file on SD Card
    *
    * @return error   1 if OK, -1 if not OK
    */
    int closing_file(void);
    /**
    *   Unmounting SD Card and FileSystem
    *
    * @return error   1 if OK, -1 if not OK
    */
    int unmounting(void);
    
    FILE *getFile(void);
 
private:
    /* CS Pin of SD Card */
    PinName     __cs;
    /* Block device of SD Card */
    SDBlockDevice   __blockDevice;
    /* File system of the SD Card */
    FATFileSystem   __fileSystem;
    /* File to process */
    FILE *__f;
    /* Error during process */
    int __err;
};
#endif
            