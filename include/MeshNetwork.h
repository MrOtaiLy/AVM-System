#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

struct __attribute__((packed)) SecurityPacket
{
    uint8_t node_mac[6];
    uint8_t iv[16];
    uint8_t encrypted_payload[64];
};

class MeshNetwork
{
public:
    static MeshNetwork &getInstance()
    {
        static MeshNetwork instance;
        return instance;
    }

    bool begin();
    void sendAlert(uint8_t eventCode);
    static void onReceiveCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len);

private:
    MeshNetwork() = default;

    uint8_t gateway_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8_t WIFI_CHANNEL = 1;

    void encryptPayload(const uint8_t *in, uint8_t *out, size_t len, const uint8_t *iv);
};