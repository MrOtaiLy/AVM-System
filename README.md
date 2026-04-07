# AVM-System: Autonomous Monitoring Core (ESP32-S3)

## English

This project is a professional-grade firmware core for ESP32-S3 based security systems. It utilizes FreeRTOS for task distribution, where vision processing and network communication run on separate CPU cores. The system features motion detection, ESP-NOW mesh networking, and advanced power management with deep sleep support.

### Technical Specifications

- **Framework:** Arduino ESP32 Core
- **OS:** FreeRTOS (Dual-core execution)
- **Storage:** SD_MMC with Mutex protection
- **Networking:** ESP-NOW (Peer-to-peer)
- **Memory:** Optimized for PSRAM (MALLOC_CAP_SPIRAM)

### Hardware Connection

To ensure correct operation, connect the components to the following GPIO pins:

| Component           | Pin (GPIO) | Notes                           |
| :------------------ | :--------- | :------------------------------ |
| **PIR Sensor**      | GPIO 4     | Input with Wakeup support       |
| **Battery Voltage** | GPIO 1     | Analog input (Voltage divider)  |
| **Camera (CSI)**    | Standard   | Configured for ESP32-S3-WROOM-1 |
| **SD Card (DAT0)**  | GPIO 2     | SD_MMC Slot                     |
| **SD Card (CLK)**   | GPIO 14    | SD_MMC Slot                     |
| **SD Card (CMD)**   | GPIO 15    | SD_MMC Slot                     |

### Installation

1. Install VS Code and PlatformIO extension.
2. Clone this repository.
3. Verify PSRAM and Flash settings in `platformio.ini` (e.g., N8R8).
4. Perform Build and Upload.

---

## Русский

Данный проект представляет собой профессиональное программное ядро для систем безопасности на базе ESP32-S3. Используется операционная система FreeRTOS для распределения задач: обработка изображения и сетевой обмен работают на разных ядрах процессора. Система поддерживает детекцию движения, работу в Mesh-сетях через протокол ESP-NOW и расширенное управление питанием с поддержкой глубокого сна.

### Технические характеристики

- **Фреймворк:** Arduino ESP32 Core
- **ОС:** FreeRTOS (Двухъядерное исполнение)
- **Хранение данных:** SD_MMC с защитой через Mutex
- **Сеть:** ESP-NOW (Прямая связь между устройствами)
- **Память:** Оптимизировано под PSRAM (MALLOC_CAP_SPIRAM)

### Подключение периферии

Для корректной работы системы подключите компоненты к следующим выводам (GPIO):

| Компонент          | Pin (GPIO) | Примечания                       |
| :----------------- | :--------- | :------------------------------- |
| **PIR датчик**     | GPIO 4     | Вход с поддержкой пробуждения    |
| **Напряжение АКБ** | GPIO 1     | Аналоговый вход (через делитель) |
| **Камера (CSI)**   | Standard   | Сконфигурировано под стандарт S3 |
| **SD Card (DAT0)** | GPIO 2     | Слот SD_MMC                      |
| **SD Card (CLK)**  | GPIO 14    | Слот SD_MMC                      |
| **SD Card (CMD)**  | GPIO 15    | Слот SD_MMC                      |

### Установка

1. Установите VS Code и расширение PlatformIO.
2. Склонируйте данный репозиторий.
3. Проверьте настройки PSRAM и Flash в `platformio.ini` (например, N8R8).
4. Выполните сборку (Build) и загрузку (Upload).
