#include "rtc.h"

#define RTC_I2C_ADDRESS 0xD0 // DS1307 or DS3231 I2C address

// Convert BCD to Decimal
static uint8_t bcd_to_dec(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Convert Decimal to BCD
static uint8_t dec_to_bcd(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

// Initialize the RTC
void rtc_initialize(void)
{
    // Nothing specific to initialize for DS1307/DS3231
    // Ensure I2C communication is ready
    i2c_start();
    i2c_write(RTC_I2C_ADDRESS); // Access the RTC
    i2c_stop();
}

// Get Time from RTC
void rtc_get_time(custom_rtc_time_t *time)
{
    uint8_t data[7];

    // Request time data from RTC
    i2c_start();
    i2c_write(RTC_I2C_ADDRESS); // Write mode
    i2c_write(0x00);            // Start from register 0x00
    i2c_stop();

    // Read 7 bytes (seconds, minutes, hours, day, date, month, year)
    i2c_start();
    i2c_write(RTC_I2C_ADDRESS | 0x01); // Read mode
    for (uint8_t i = 0; i < 6; i++)
    {
        data[i] = i2c_read(1); // Read with ACK
    }
    data[6] = i2c_read(0); // Last byte with NACK
    i2c_stop();

    // Parse the time
    time->seconds = bcd_to_dec(data[0]);
    time->minutes = bcd_to_dec(data[1]);
    time->hours = bcd_to_dec(data[2]);
    time->day = bcd_to_dec(data[4]);
    time->month = bcd_to_dec(data[5]);
    time->year = bcd_to_dec(data[6]);
}

