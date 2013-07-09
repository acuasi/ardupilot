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
 *               init() : initializate sensor and library.
 *               read   : reads latest value from SmartBattery and
 *                        stores values in voltage, current,  parameter
 *               read_register()  : reads a value from the sensor (will be
 *                                  sensor specific)
 *               write_register() : writes a value to one of the sensor's
 *                                  register (will be sensor specific)
 */

#include <AP_Math.h>

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

    virtual bool init(); 

    virtual uint8_t read_register(uint8_t address);
    virtual void    write_register(uint8_t address, uint8_t value);

    // called by timer process to read sensor data from all attached sensors
    static void     read(uint32_t now);

    // read latest values from sensor and fill in voltage, current, and totals.
    virtual void    update(uint32_t now);

    // public variables
    int16_t  raw_dx;            // raw sensor change in x and y position (i.e. unrotated)
    int16_t  raw_dy;            // raw sensor change in x and y position (i.e. unrotated)
    uint8_t  surface_quality;   // image quality (below 15 you really can't trust the x,y values returned)
    int16_t  x,y;               // total x,y position
    int16_t  dx,dy;             // rotated change in x and y position
    float    vlon, vlat;        // position as offsets from original position
    uint32_t last_update;       // millis() time of last update
    float    field_of_view;     // field of view in Radians
    float    scaler;            // number returned from sensor when moved one pixel
    int16_t  num_pixels;        // number of pixels of resolution in the sensor

    // public variables for reporting purposes
    float    exp_change_x, exp_change_y;    // expected change in x, y coordinates
    float    change_x, change_y;            // actual change in x, y coordinates
    float    x_cm, y_cm;                    // x,y position in cm

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
