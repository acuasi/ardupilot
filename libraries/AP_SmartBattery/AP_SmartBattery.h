#ifndef __AP_SMARTBATTERY_H__
#define __AP_SMARTBATTERY_H__

/*
 *       AP_SmartBattery.cpp - SmartBattery Base Class for Ardupilot Mega
 *       Code by Rayjan Wilson
 *
 *       This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *       Methods:
 *               read   : reads latest value from SmartBattery and
 *                        stores values in voltage, current,  parameter
 *               readVoltge()  : reads a value from the sensor (will be sensor specific)
 *               readCurrent()  : reads a value from the sensor (will be sensor specific)
 *               readStateOfCharge()  : reads a value from the sensor (will be sensor specific)
 *               
 */

#define AP_SMARTBATTERY_NUM_CALLS_FOR_1HZ     1000      // timer process runs at 1khz.  1000 iterations = 1hz
#define AP_SMARTBATTERY_NUM_CALLS_FOR_10HZ     100       // timer process runs at 1khz.  100 iterations = 10hz
#define AP_SMARTBATTERY_NUM_CALLS_FOR_20HZ     50        // timer process runs at 1khz.  50 iterations = 20hz
#define AP_SMARTBATTERY_NUM_CALLS_FOR_50HZ     20        // timer process runs at 1khz.  20 iterations = 50hz

class AP_SmartBattery
{
public:
    // constructor
    AP_SmartBattery() {
        _sensor = this;
    };
    ~AP_SmartBattery() {
        _sensor = NULL;
    };

    
    // called by timer process to read sensor data from all attached sensors
    static void read();

    // read latest values from sensor and fill in voltage, current, and totals.
    virtual void update(uint32_t now);

    // public variables
    

protected:
    // pointer to the last instantiated smart battery sensor.  Will be turned
    // into a table if we ever add support for more than one sensor
    static AP_SmartBattery*  _sensor;
    

private:
    // number of times we have been called by 1khz timer process.
    // We use this to throttle read down to 20hz
    static uint8_t _num_calls;
};

#include "AP_SmartBattery_BQ34Z100.h"

#endif
