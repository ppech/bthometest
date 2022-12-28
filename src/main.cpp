#include <Arduino.h>
#include "datastore.h"
#include "tasks.h"

void setup()
{

  // https://github.com/stumpylog/bthome-weather-station
  ESP_LOGI("main", "Starting up...");

  datastore_init();

  xTaskCreatePinnedToCore(&task_ble_entry,                 // task
                          TASK_NAME_BLE,                   // task name
                          2048,                            // stack size
                          NULL,                            // parameters
                          5,                               // priority
                          &datastore.system.taskBleHandle, // returned task handle
                          0                                // pinned core
  );

  xTaskCreatePinnedToCore(&task_sensors_entry,                 // task
                          TASK_NAME_SENSORS,                   // task name
                          2048,                                // stack size
                          NULL,                                // parameters
                          5,                                   // priority
                          &datastore.system.taskSensorsHandle, // returned task handle
                          1                                    // pinned core
  );

  ESP_LOGI("main", "Start finished");
}

void loop()
{
  // put your main code here, to run repeatedly:
}