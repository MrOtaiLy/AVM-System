#pragma once

#include <Arduino.h>
#include "esp_camera.h"

class VisionSystem
{
public:
    static VisionSystem &getInstance()
    {
        static VisionSystem instance;
        return instance;
    }

    bool begin();
    bool detectMotion();
    camera_fb_t *captureFrame();
    void releaseFrame(camera_fb_t *fb);

private:
    VisionSystem() = default;

    uint8_t *prevFrameBuf = nullptr;
    size_t frameBufSize = 0;
    const uint8_t DIFF_THRESHOLD = 25;
    const uint32_t PIXEL_CHANGE_TRIGGER_COUNT = 500;
};