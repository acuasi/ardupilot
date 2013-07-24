#ifndef __AP_SMARTBATTERY_BQ34Z100_H__
#define __AP_SMARTBATTERY_BQ34Z100_H__

#include <AP_HAL.h>
#include "AP_SmartBattery.h"

class AP_SmartBattery_BQ34Z100 : public AP_SmartBattery
{
public:

    // constructor
    AP_SmartBattery_BQ34Z100(void);


    // read latest values from sensor and fill in x,y and totals,
    // returns true on successful read
    void    update(uint32_t now);

    // BQ34Z100 specific features
    unsigned int readVoltage(void);
    unsigned int readCurrent(void);
    unsigned int readStateOfCharge(void);
    

private:
    // I2C device
    AP_HAL::I2CDeviceDriver *_i2c;
    AP_HAL::Semaphore *_i2c_sem;

    uint8_t voltage = 0;
    uint8_t current = 0;
    uint8_t soc = 0;

    uint8_t len = 2;
    
    bool healthy = false;

};

#endif
