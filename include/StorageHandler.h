#pragma once

#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class StorageHandler
{
public:
    static StorageHandler &getInstance()
    {
        static StorageHandler instance;
        return instance;
    }

    bool begin();
    void writeLog(const char *msg);

private:
    StorageHandler();
    ~StorageHandler() = default;
    StorageHandler(const StorageHandler &) = delete;
    StorageHandler &operator=(const StorageHandler &) = delete;

    SemaphoreHandle_t sdMutex;
    const uint32_t MAX_LOG_SIZE = 5 * 1024 * 1024;
    const char *LOG_PATH = "/system_log.txt";
    const char *LOG_BACKUP_PATH = "/system_log_old.txt";

    void rotateLogsIfNeeded();
};