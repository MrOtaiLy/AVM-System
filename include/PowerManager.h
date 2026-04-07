#pragma once

#include <Arduino.h>

struct BatteryPoint
{
    uint32_t voltage_mv;
    uint8_t percentage;
};

class PowerManager
{
public:
    static PowerManager &getInstance()
    {
        static PowerManager instance;
        return instance;
    }

    void begin();
    void enterDeepSleep();
    uint8_t getBatteryLevel();

private:
    PowerManager() = default;

    static const BatteryPoint dischargeCurve[];
    static const uint8_t CURVE_POINTS = 11;

    const gpio_num_t PIR_PIN = GPIO_NUM_4;
    const uint8_t BAT_ADC_PIN = 5;
    const uint64_t TIMER_WAKEUP_US = 600000000ULL;
};