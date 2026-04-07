#include "MeshNetwork.h"
#include "esp_log.h"

static const char *TAG = "HAL_MESH";

bool MeshNetwork::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

    if (esp_now_init() != ESP_OK)
        return false;

    esp_now_register_recv_cb(onReceiveCallback);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, gateway_mac, 6);
    peerInfo.channel = WIFI_CHANNEL;
    peerInfo.encrypt = false;

    return (esp_now_add_peer(&peerInfo) == ESP_OK);
}

void MeshNetwork::encryptPayload(const uint8_t *in, uint8_t *out, size_t len, const uint8_t *iv)
{
    for (size_t i = 0; i < len; i++)
    {
        out[i] = in[i] ^ iv[i % 16] ^ 0xA5;
    }
}

void MeshNetwork::sendAlert(uint8_t eventCode)
{
    SecurityPacket pkt;
    WiFi.macAddress(pkt.node_mac);

    for (int i = 0; i < 16; i++)
        pkt.iv[i] = esp_random() & 0xFF;

    uint8_t rawPayload[64] = {0};
    rawPayload[0] = eventCode;
    rawPayload[1] = 0x02;

    encryptPayload(rawPayload, pkt.encrypted_payload, 64, pkt.iv);
    esp_now_send(gateway_mac, (uint8_t *)&pkt, sizeof(pkt));
}

void MeshNetwork::onReceiveCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    if (data_len > 0)
    {
        ESP_LOGD(TAG, "Принят пакет от %02X:%02X:%02X...", mac_addr[0], mac_addr[1], mac_addr[2]);
    }
}