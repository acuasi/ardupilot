/*
 *       AP_SmartBattery.cpp - Smart Battery Base Class for Ardupilot Mega
 *       Code by Rayjan Wilson
 *
 *       This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 */

#include "AP_SmartBattery.h"

// pointer to the last instantiated optical flow sensor.  Will be turned into
// a table if we ever add support for more than one sensor
AP_SmartBattery* AP_SmartBattery::_sensor = NULL;

// number of times we have been called by 1khz timer process.
// We use this to throttle read down to 20hz
uint8_t AP_SmartBattery::_num_calls;

bool AP_SmartBattery::init()
{
    return true;      // just return true by default
}


// parent method called at 1khz by periodic process
// this is slowed down to 1hz and each instance's update function is called
// (only one instance is supported at the moment)
void AP_SmartBattery::read(uint32_t now)
{
    _num_calls++;

    if( _num_calls >= AP_SMARTBATTERY_NUM_CALLS_FOR_1HZ ) {
        _num_calls = 0;
        // call to update all attached sensors
        if( _sensor != NULL ) {
            _sensor->update(now);
        }
    }
};

// read value from the sensor.  Should be overridden by derived class
void AP_SmartBattery::update(uint32_t now){ }

// reads a value from the sensor (will be sensor specific)
uint8_t AP_SmartBattery::read_register(uint8_t address){ return 0; }

// writes a value to one of the sensor's register (will be sensor specific)
void AP_SmartBattery::write_register(uint8_t address, uint8_t value) {}


