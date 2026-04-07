#include "StorageHandler.h"
#include "esp_log.h"

static const char *TAG = "HAL_STORAGE";

StorageHandler::StorageHandler()
{
    sdMutex = xSemaphoreCreateMutex();
}

bool StorageHandler::begin()
{
    SD_MMC.setPins(39, 38, 40, 41, 42, 2);
    if (!SD_MMC.begin("/sdcard", true))
    {
        ESP_LOGE(TAG, "SD mount failed");
        return false;
    }
    if (SD_MMC.cardType() == CARD_NONE)
    {
        ESP_LOGE(TAG, "No SD card");
        return false;
    }
    return true;
}

void StorageHandler::writeLog(const char *msg)
{
    if (xSemaphoreTake(sdMutex, portMAX_DELAY) == pdTRUE)
    {
        rotateLogsIfNeeded();
        File file = SD_MMC.open(LOG_PATH, FILE_APPEND);
        if (file)
        {
            file.println(msg);
            file.close();
        }
        xSemaphoreGive(sdMutex);
    }
}

void StorageHandler::rotateLogsIfNeeded()
{
    File file = SD_MMC.open(LOG_PATH, FILE_READ);
    if (!file)
        return;

    size_t size = file.size();
    file.close();

    if (size >= MAX_LOG_SIZE)
    {
        SD_MMC.remove(LOG_BACKUP_PATH);
        SD_MMC.rename(LOG_PATH, LOG_BACKUP_PATH);
    }
}