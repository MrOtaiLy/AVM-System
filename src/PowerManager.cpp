#include "PowerManager.h"
#include "esp_log.h"
#include "esp_sleep.h"

static const char *TAG = "HAL_POWER";

const BatteryPoint PowerManager::dischargeCurve[] = {
    {4200, 100}, {4060, 90}, {3980, 80}, {3920, 70}, {3870, 60}, {3820, 50}, {3790, 40}, {3770, 30}, {3740, 20}, {3680, 10}, {3200, 0}};

void PowerManager::begin()
{
    pinMode(PIR_PIN, INPUT_PULLDOWN);
    esp_sleep_enable_ext1_wakeup((1ULL << PIR_PIN), ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_enable_timer_wakeup(TIMER_WAKEUP_US);
}

uint8_t PowerManager::getBatteryLevel()
{
    uint32_t voltage_mv = analogReadMilliVolts(BAT_ADC_PIN) * 2;

    if (voltage_mv >= dischargeCurve[0].voltage_mv)
        return 100;
    if (voltage_mv <= dischargeCurve[CURVE_POINTS - 1].voltage_mv)
        return 0;

    for (int i = 0; i < CURVE_POINTS - 1; i++)
    {
        if (voltage_mv <= dischargeCurve[i].voltage_mv && voltage_mv > dischargeCurve[i + 1].voltage_mv)
        {
            float vDiff = dischargeCurve[i].voltage_mv - dischargeCurve[i + 1].voltage_mv;
            float pDiff = dischargeCurve[i].percentage - dischargeCurve[i + 1].percentage;
            float factor = (voltage_mv - dischargeCurve[i + 1].voltage_mv) / vDiff;
            return dischargeCurve[i + 1].percentage + (pDiff * factor);
        }
    }
    return 0;
}

void PowerManager::enterDeepSleep()
{
    Serial.flush();
    esp_deep_sleep_start();
}