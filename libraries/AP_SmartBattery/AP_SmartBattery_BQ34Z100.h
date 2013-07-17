#ifndef __AP_SMARTBATTERY_BQ34Z100_H__
#define __AP_SMARTBATTERY_BQ34Z100_H__

#include <AP_HAL.h>
#include "AP_SmartBattery.h"

// Register Map for the BQ34Z100 Smart Battery Sensor
#define BQ34Z100_PRODUCT_ID            0x00
#define BQ34Z100_REVISION_ID           0x01
#define BQ34Z100_MOTION                0x02
#define BQ34Z100_DELTA_X               0x03
#define BQ34Z100_DELTA_Y               0x04
#define BQ34Z100_SQUAL                 0x05
#define BQ34Z100_PIXEL_SUM             0x06
#define BQ34Z100_MAXIMUM_PIXEL         0x07
#define BQ34Z100_CONFIGURATION_BITS    0x0a
#define BQ34Z100_EXTENDED_CONFIG       0x0b
#define BQ34Z100_DATA_OUT_LOWER        0x0c
#define BQ34Z100_DATA_OUT_UPPER        0x0d
#define BQ34Z100_SHUTTER_LOWER         0x0e
#define BQ34Z100_SHUTTER_UPPER         0x0f
#define BQ34Z100_FRAME_PERIOD_LOWER    0x10
#define BQ34Z100_FRAME_PERIOD_UPPER    0x11
#define BQ34Z100_MOTION_CLEAR          0x12
#define BQ34Z100_FRAME_CAPTURE         0x13
#define BQ34Z100_SROM_ENABLE           0x14
#define BQ34Z100_FRAME_PERIOD_MAX_BOUND_LOWER      0x19
#define BQ34Z100_FRAME_PERIOD_MAX_BOUND_UPPER      0x1a
#define BQ34Z100_FRAME_PERIOD_MIN_BOUND_LOWER      0x1b
#define BQ34Z100_FRAME_PERIOD_MIN_BOUND_UPPER      0x1c
#define BQ34Z100_SHUTTER_MAX_BOUND_LOWER           0x1e
#define BQ34Z100_SHUTTER_MAX_BOUND_UPPER           0x1e
#define BQ34Z100_SROM_ID               0x1f
#define BQ34Z100_OBSERVATION           0x3d
#define BQ34Z100_INVERSE_PRODUCT_ID    0x3f
#define BQ34Z100_PIXEL_BURST           0x40
#define BQ34Z100_MOTION_BURST          0x50
#define BQ34Z100_SROM_LOAD             0x60

// Configuration Bits
#define BQ34Z100_LED_MODE_ALWAYS_ON        0x00
#define BQ34Z100_LED_MODE_WHEN_REQUIRED    0x01

#define BQ34Z100_RESOLUTION_400                 400
#define BQ34Z100_RESOLUTION_1600                1600

// Extended Configuration bits
#define BQ34Z100_SERIALNPU_OFF  0x02

#define BQ34Z100_FRAME_RATE_MAX         6469
#define BQ34Z100_FRAME_RATE_MIN         2000

// SPI bus definitions
#define BQ34Z100_SPI_UNKNOWN         0
#define BQ34Z100_SPIBUS_1            1  // standard SPI bus
#define BQ34Z100_SPIBUS_3            3  // SPI3

class AP_SmartBattery_BQ34Z100 : public AP_SmartBattery
{
public:

    // constructor
    AP_SmartBattery_BQ34Z100(uint8_t reset_pin = 0);

    // initialise the sensor
    bool    init();

    uint8_t read_register(uint8_t address);
    void    write_register(uint8_t address, uint8_t value);

    // reset sensor by holding a pin high (or is it low?) for 10us.
    void    reset();

    // read latest values from sensor and fill in x,y and totals,
    // returns true on successful read
    void    update(uint32_t now);

    // BQ34Z100 specific features

    // return true if there has been motion since the last time this was called
    bool motion() { 
        if( _motion ) {
            _motion = false;
            return true;
        } else {
            return false;
        }
    }                                                                       

    // true if there has been an overflow
    bool overflow() { return _overflow; }

    void disable_serial_pullup();

    // returns true if LED is always on, false if only on when required
    bool get_led_always_on();
    // set parameter to true if you want LED always on, otherwise false
    // for only when required
    void set_led_always_on( bool alwaysOn );

    // returns resolution (either 400 or 1600 counts per inch)
    int16_t get_resolution();
    // set parameter to 400 or 1600 counts per inch
    void set_resolution(uint16_t resolution);

    // get_frame_rate_auto - return true if frame rate is set to "auto",
    // false if manual
    bool get_frame_rate_auto();
    // set_frame_rate_auto(bool) - set frame rate to auto (true),
    // or manual (false)
    void set_frame_rate_auto(bool auto_frame_rate); 

    // get_frame_period
    uint16_t get_frame_period();
    void     set_frame_period(uint16_t period);

    uint16_t get_frame_rate();
    void     set_frame_rate(uint16_t rate);

    // get_shutter_speed_auto - returns true if shutter speed is adjusted
    // automatically, false if manual
    bool     get_shutter_speed_auto();
    // set_shutter_speed_auto - set shutter speed to auto (true),
    // or manual (false)
    void     set_shutter_speed_auto(bool auto_shutter_speed);

    uint16_t get_shutter_speed();
    void     set_shutter_speed(uint16_t shutter_speed);

    // will cause the x,y, dx, dy, and the sensor's motion registers to
    // be cleared
    void     clear_motion();

    // dumps a 30x30 image to the Serial port
    void     print_pixel_data();

private:
    // pin used for chip reset
    uint8_t _reset_pin;
    // true if there has been motion
    bool    _motion;
    // true if the x or y data buffers overflowed
    bool    _overflow;

    // SPI device
    AP_HAL::SPIDeviceDriver *_spi;
    AP_HAL::Semaphore *_spi_sem;
};

#endif
