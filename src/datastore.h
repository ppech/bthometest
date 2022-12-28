#ifndef DATASTORE_H
#define DATASTORE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct SensorsData
{
    float temperature;
    float humidity;
    float pressure;
    float illuminance;
} SensorsData;

typedef struct SystemData
{
    uint8_t bootCount;
    uint8_t packetId;
    TaskHandle_t taskBleHandle;
    TaskHandle_t taskSensorsHandle;
} SystemData;

typedef struct DataStore
{
    SystemData system;
    SensorsData sensors;
} DataStore;

extern DataStore datastore;

void datastore_init(void);

#endif