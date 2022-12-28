#ifndef TASKS_H
#define TASKS_H

#define TASK_NAME_BLE    "task_ble"
#define TASK_NAME_SENSORS "task_sensors"

void task_ble_entry(void* params);

void task_sensors_entry(void* params);

#endif