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

#define AP_SPI_TIMEOUT 1000

union NumericIntType
{
    int16_t intValue;
    uint16_t uintValue;
    uint8_t byteValue[2];
};

// Constructors ////////////////////////////////////////////////////////////////
AP_SmartBattery_BQ34Z100::AP_SmartBattery_BQ34Z100(uint8_t reset_pin) :
    _reset_pin(reset_pin)
{
    num_pixels = ADNS3080_PIXELS_X;
    field_of_view = AP_OPTICALFLOW_ADNS3080_08_FOV;
    scaler = AP_OPTICALFLOW_ADNS3080_SCALER;
}

// Public Methods //////////////////////////////////////////////////////////////
// init - initialise sensor
// assumes SPI bus has been initialised but will attempt to initialise 
// nonstandard SPI3 bus if required
bool
AP_SmartBattery_BQ34Z100::init()
{
    int8_t retry = 0;
    bool retvalue = false;

    // suspend timer while we set-up SPI communication
    hal.scheduler->suspend_timer_procs();

    // reset the device if the reset pin has been defined
    if(_reset_pin != 0) {
        hal.gpio->pinMode(_reset_pin, GPIO_OUTPUT);
    }

    // reset the device
    reset();

    // get pointer to the spi bus
    _spi = hal.spi->device(AP_HAL::SPIDevice_ADNS3080_SPI0);
    if (_spi == NULL) {
        retvalue = false; goto finish;
    }

    // check 3 times for the sensor on standard SPI bus
    while( retvalue == false && retry < 3 ) {
        if( read_register(ADNS3080_PRODUCT_ID) == 0x17 ) {
            retvalue = true;
            goto finish;
        }
        retry++;
    }

    // if not found, get pointer to the SPI3 bus
    _spi = hal.spi->device(AP_HAL::SPIDevice_ADNS3080_SPI3);
    if (_spi == NULL) {
        retvalue = false; goto finish;
    }

    // check 3 times on SPI3
    retry = 0;
    while( retvalue == false && retry < 3 ) {
        if( read_register(ADNS3080_PRODUCT_ID) == 0x17 ) {
            retvalue = true;
        }
        retry++;
    }

    // If we fail to find on SPI3, no connection available.
    retvalue = false;
    _spi = NULL;

finish:
    // if device is working register the global static read function to
    // be called at 1khz
    if( retvalue ) {
        hal.scheduler->register_timer_process( AP_OpticalFlow_ADNS3080::read );
        _spi_sem = _spi->get_semaphore();
        if (_spi_sem == NULL) {
            hal.scheduler->panic(PSTR("PANIC: Got SPI Driver, but did not "
                        "get valid SPI semaphore in AP_Optflow_ADNS3080"));
        }
    }

    // resume timer
    hal.scheduler->resume_timer_procs();

    return retvalue;
}


// Read a register from the sensor
uint8_t AP_SmartBattery_BQ34Z100::read_register(uint8_t address)
{
    if (_spi == NULL) return 0;

    if (!_spi_sem->take_nonblocking()) {
        return 0;
    }

    _spi->cs_assert();
    // send the device the register you want to read:
    _spi->transfer(address);
    hal.scheduler->delay_microseconds(50);
    // send a value of 0 to read the first byte returned:
    uint8_t result = _spi->transfer(0x00);

    _spi->cs_release();

    _spi_sem->give();

    return result;
}

// write a value to one of the sensor's registers
void AP_SmartBattery_BQ34Z100::write_register(uint8_t address, uint8_t value)
{
    if (_spi == NULL) return;

    if (!_spi_sem->take_nonblocking()) {
        return;
    }

    _spi->cs_assert();

    // send register address
    _spi->transfer(address | 0x80 );
    hal.scheduler->delay_microseconds(50);
    // send data
    _spi->transfer(value);

    _spi->cs_release();

    _spi_sem->give();
}

// reset sensor by holding a pin high (or is it low?) for 10us.
void AP_SmartBattery_BQ34Z100::reset()
{
    // return immediately if the reset pin is not defined
    if( _reset_pin == 0)
        return;

    // reset sensor
    hal.gpio->write(_reset_pin, 1);
    hal.scheduler->delay_microseconds(10);
    // return sensor to normal
    hal.gpio->write(_reset_pin, 0);
}

// read latest values from sensor and fill in x,y and totals
void AAP_SmartBattery_BQ34Z100::update(uint32_t now)
{
    uint8_t motion_reg;
    surface_quality = read_register(ADNS3080_SQUAL);
    hal.scheduler->delay_microseconds(50);

    // check for movement, update x,y values
    motion_reg = read_register(ADNS3080_MOTION);
    // check if we've had an overflow
    _overflow = ((motion_reg & 0x10) != 0);
    if( (motion_reg & 0x80) != 0 ) {
        raw_dx = ((int8_t)read_register(ADNS3080_DELTA_X));
        hal.scheduler->delay_microseconds(50);
        raw_dy = ((int8_t)read_register(ADNS3080_DELTA_Y));
        _motion = true;
    }else{
        raw_dx = 0;
        raw_dy = 0;
    }

    last_update = hal.scheduler->millis();

    apply_orientation_matrix();
}


