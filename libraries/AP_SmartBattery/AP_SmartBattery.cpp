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

// pointer to the last instantiated battery sensor.  Will be turned into
// a table if we ever add support for more than one sensor
AP_SmartBattery* AP_SmartBattery::_sensor = NULL;

// number of times we have been called by 1khz timer process.
// We use this to throttle read down to 20hz
uint8_t AP_SmartBattery::_num_calls;

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
virtual void AP_SmartBattery::update(uint32_t now){ }
virtual unsigned int AP_SmartBattery::readVoltage(void){ };
virtual unsigned int AP_SmartBattery::readCurrent(void){ };
virtual unsigned int AP_SmartBattery::readStateOfCharge(void){ };
