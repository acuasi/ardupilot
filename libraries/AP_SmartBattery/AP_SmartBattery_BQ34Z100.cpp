/*
 *       AP_SmartBattery_BQ34Z100.cpp - BQ34Z100 SmartBattery Library for Ardupilot Mega
 *       Code by Rayjan Wilson
 *
 *       This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 */

#include <AP_HAL.h>
#include "AP_SmartBattery_BQ34Z100.h"

extern const AP_HAL::HAL& hal;

// Register Map for the BQ34Z100 Smart Battery Sensor
#define BQ34Z100                      0x55  //register or address
#define BQ34Z100_Voltage    0x08 //, 0x09
#define BQ34Z100_AverageCurrent 0x0a //, 0x0b
#define BQ34Z100_StateOfCharge 0x02 //, 0x03
#define BQ34Z100_Temperature 0x0c //, 0x0d
//InstantCurrent 0x00, 0x01 then 0x0018

// Constructors
AP_SmartBattery_BQ34Z100::AP_SmartBattery_BQ34Z100(void){ }

void update(void){
    battery.voltage = readVoltage();
    battery.current_amps = readCurrent();
    battery.StateOfCharge = readStateOfCharge();
}

unsigned int readVoltage(void){
    uint8_t data[2];
    if(hal.i2c->readRegisters(BQ34Z100, BQ34Z100_Voltage, len, data) != 0){
        healthy = false;
    }
    else{
        // combine results into voltage
        voltage = buff[0] << 8 | buff[1];
        healthy = true;
    }
    return voltage;
}

unsigned int readCurrent(void){
    uint8_t data[2];
    if(hal.i2c->readRegisters(BQ34Z100, BQ34Z100_AverageCurrent, len, data) != 0){
        healthy = false;
    }
    else{
        // combine results into voltage
        current = buff[0] << 8 | buff[1];
        healthy = true;
    }
    return current;
}

unsigned int readStateOfCharge(void){
    uint8_t data[2];
    if(hal.i2c->readRegisters(BQ34Z100, BQ34Z100_StateOfCharge, len, data) != 0){
        healthy = false;
    }
    else{
        // combine results into voltage
        soc = buff[0] << 8 | buff[1];
        healthy = true;
    }
    return soc;
}
