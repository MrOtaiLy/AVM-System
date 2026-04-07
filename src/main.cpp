#include <Arduino.h>
#include "esp_log.h"
#include "StorageHandler.h"
#include "PowerManager.h"
#include "VisionSystem.h"
#include "MeshNetwork.h"

static const char *TAG = "MAIN_CORE";

TaskHandle_t cameraTaskHandle = NULL;
TaskHandle_t meshTaskHandle = NULL;

void taskCameraLogic(void *pvParameters)
{
  VisionSystem &vision = VisionSystem::getInstance();
  MeshNetwork &mesh = MeshNetwork::getInstance();
  StorageHandler &storage = StorageHandler::getInstance();

  for (;;)
  {
    if (vision.detectMotion())
    {
      storage.writeLog("[WARN] ALARM_TRIGGER");
      mesh.sendAlert(0x0A);
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void taskMeshMaintenance(void *pvParameters)
{
  PowerManager &power = PowerManager::getInstance();
  StorageHandler &storage = StorageHandler::getInstance();

  for (;;)
  {
    uint8_t batLvl = power.getBatteryLevel();

    char logBuffer[64];
    snprintf(logBuffer, sizeof(logBuffer), "[INFO] BAT:%d%%", batLvl);
    storage.writeLog(logBuffer);

    if (batLvl < 5)
    {
      MeshNetwork::getInstance().sendAlert(0xBB);
      vTaskDelay(pdMS_TO_TICKS(1000));
      power.enterDeepSleep();
    }
    vTaskDelay(pdMS_TO_TICKS(60000));
  }
}

void setup()
{
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));

  PowerManager::getInstance().begin();
  StorageHandler::getInstance().begin();

  if (!VisionSystem::getInstance().begin())
  {
    esp_restart();
  }

  MeshNetwork::getInstance().begin();

  xTaskCreatePinnedToCore(
      taskCameraLogic,
      "CamTask",
      8192,
      NULL,
      5,
      &cameraTaskHandle,
      1);

  xTaskCreatePinnedToCore(
      taskMeshMaintenance,
      "MeshTask",
      4096,
      NULL,
      3,
      &meshTaskHandle,
      0);

  vTaskDelete(NULL);
}

void loop() {}