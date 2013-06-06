// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

// filter altitude from the barometer with a low pass filter
static LowPassFilterInt32 altitude_filter;


static void init_barometer(void)
{
    gcs_send_text_P(SEVERITY_LOW, PSTR("Calibrating barometer"));    
    barometer.calibrate();

    // filter at 100ms sampling, with 0.7Hz cutoff frequency
    altitude_filter.set_cutoff_frequency(0.1, 0.7);

    gcs_send_text_P(SEVERITY_LOW, PSTR("barometer calibration complete"));
}

// read the barometer and return the updated altitude in centimeters
// above the calibration altitude
static int32_t read_barometer(void)
{
    barometer.read();
    return altitude_filter.apply(barometer.get_altitude() * 100.0);
}

// in M/S * 100
static void read_airspeed(void)
{
    airspeed.read();
    calc_airspeed_errors();
}

static void zero_airspeed(void)
{
    airspeed.calibrate();
    gcs_send_text_P(SEVERITY_LOW,PSTR("zero airspeed calibrated"));
}

// Functions for reading values over I2C from the BQ34Z100 gas gauge chip
// **********************************************************************

unsigned int readSOC(void)
{
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x02);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int low = Wire.read();
  
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x03);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int high_t = Wire.read();

  unsigned int high = high_t << 8;
    
  unsigned int soc = high + low;

  return soc;
}

unsigned int RemainingCapacity(void)
{
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x04);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int low = Wire.read();
  
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x05);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int high = Wire.read();
  
  unsigned int high1 = high<<8;
  
  unsigned int remain_cap = high1 + low;

  return remain_cap;
}

unsigned int readVoltage(void)
{
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x08);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int low = Wire.read();
  
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x09);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int high = Wire.read();
  
  unsigned int high1 = high<<8;
  
  unsigned int voltage = high1 + low;

  return voltage;

}

int readCurrent(void)
{
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x0a);
  Wire.endTransmission();

  Wire.requestFrom(BQ34Z100,1);

  unsigned int low = Wire.read();

  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x0b);
  Wire.endTransmission();

  Wire.requestFrom(BQ34Z100,1);
  
  unsigned int high_t = Wire.read();
  
  unsigned int high = high_t << 8;
  
  unsigned int avg_current = high + low;
  
  return avg_current/10;
}

static void query_bq34z100(void)
{  
    Wire.begin(BQ34Z100);

    state_of_charge = readSOC();
    battery_voltage1 = readVoltage();
    current_amps1 = readCurrent();

}

// End of BQ34Z100 function definitions
// **********************************************************************

static void read_battery(void)
{
    if(g.battery_monitoring == 0) {
        battery_voltage1 = 0;
        return;
    }

    if(g.battery_monitoring == 3 || g.battery_monitoring == 4) {
        // this copes with changing the pin at runtime
        batt_volt_pin->set_pin(g.battery_volt_pin);
        battery_voltage1 = BATTERY_VOLTAGE(batt_volt_pin);
    }
    if(g.battery_monitoring == 4) {
        // this copes with changing the pin at runtime
        batt_curr_pin->set_pin(g.battery_curr_pin);
        current_amps1    = CURRENT_AMPS(batt_curr_pin);
        current_total1   += current_amps1 * (float)delta_ms_medium_loop * 0.0002778;                                    // .0002778 is 1/3600 (conversion to hours)
    }

#if BATTERY_EVENT == ENABLED
    if(battery_voltage1 < LOW_VOLTAGE) low_battery_event();
    if(g.battery_monitoring == 4 && current_total1 > g.pack_capacity) low_battery_event();
#endif
}


// read the receiver RSSI as an 8 bit number for MAVLink
// RC_CHANNELS_SCALED message
void read_receiver_rssi(void)
{
    rssi_analog_source->set_pin(g.rssi_pin);
    float ret = rssi_analog_source->voltage_average() * 50;
    receiver_rssi = constrain_int16(ret, 0, 255);
}


/*
  return current_loc.alt adjusted for ALT_OFFSET
  This is useful during long flights to account for barometer changes
  from the GCS, or to adjust the flying height of a long mission
 */
static int32_t adjusted_altitude_cm(void)
{
    return current_loc.alt - (g.alt_offset*100);
}
