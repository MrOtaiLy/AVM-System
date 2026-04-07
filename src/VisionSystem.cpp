#include "VisionSystem.h"
#include "esp_log.h"
#include "esp_heap_caps.h"

static const char *TAG = "HAL_VISION";

bool VisionSystem::begin()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 11;
    config.pin_d1 = 9;
    config.pin_d2 = 8;
    config.pin_d3 = 10;
    config.pin_d4 = 12;
    config.pin_d5 = 18;
    config.pin_d6 = 17;
    config.pin_d7 = 16;
    config.pin_xclk = 15;
    config.pin_pclk = 13;
    config.pin_vsync = 6;
    config.pin_href = 7;
    config.pin_sccb_sda = 4;
    config.pin_sccb_scl = 5;
    config.pin_pwdn = -1;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_GRAYSCALE;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;

    if (esp_camera_init(&config) != ESP_OK)
        return false;

    frameBufSize = 320 * 240;
    prevFrameBuf = (uint8_t *)heap_caps_calloc(1, frameBufSize, MALLOC_CAP_SPIRAM);

    return prevFrameBuf != nullptr;
}

bool VisionSystem::detectMotion()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
        return false;

    uint32_t changedPixels = 0;
    for (size_t i = 0; i < frameBufSize; i++)
    {
        if (std::abs((int)fb->buf[i] - (int)prevFrameBuf[i]) > DIFF_THRESHOLD)
        {
            changedPixels++;
        }
    }

    memcpy(prevFrameBuf, fb->buf, frameBufSize);
    esp_camera_fb_return(fb);

    return (changedPixels > PIXEL_CHANGE_TRIGGER_COUNT);
}

camera_fb_t *VisionSystem::captureFrame()
{
    return esp_camera_fb_get();
}

void VisionSystem::releaseFrame(camera_fb_t *fb)
{
    if (fb)
        esp_camera_fb_return(fb);
}