#include <Arduino.h>
#include <NimBLEDevice.h>
#include "datastore.h"
#include "tasks.h"

const NimBLEUUID uuid = NimBLEUUID((uint16_t)0xD2FC);

void task_ble_entry(void *params)
{
    // wait some to time to initialize sensors    
    vTaskDelay(1500 / portTICK_PERIOD_MS);

    BLEDevice::init("");

    ESP_LOGI(TASK_NAME_BLE, "Task BLE entry");

    for (;;)
    {
        // Wait for sensor task notify
        // Clear all bits on exit to reset
        if (pdTRUE == xTaskNotifyWait(0, 0xFFFFFFFF, NULL, 60000 / portTICK_PERIOD_MS))
        {
            ESP_LOGI(TASK_NAME_BLE, "notify");
        }
        else
        {
            ESP_LOGI(TASK_NAME_BLE, "timeout");
        }

        ESP_LOGI(TASK_NAME_BLE, "Packet #%d", datastore.system.packetId);

        BLEAdvertisementData data = BLEAdvertisementData();

        data.setFlags(0x06);

        std::string bth = "";

        // length
        bth += 0x12; //18

        // 16-bit UUID
        bth += 0x16;

        // UUID
        bth += 0xD2;
        bth += 0xFC;

        // BTHome device info
        bth += 0x40;

        // temperature
        bth += 0x02;

        int16_t t = datastore.sensors.temperature * 100;
        bth += (char)(t & 0xFF);          // lsb
        bth += (char)((t & 0xFF00) >> 8); // msb

        // humidity
        bth += 0x03;

        uint16_t h = datastore.sensors.humidity * 100;
        bth += (char)(h & 0xFF);          // lsb
        bth += (char)((h & 0xFF00) >> 8); // msb

        // pressure
        bth += 0x04;

        uint32_t p = datastore.sensors.pressure * 100;
        bth += (char)(p & 0xFF);             // lsb
        bth += (char)((p & 0xFF00) >> 8);   
        bth += (char)((p & 0xFF0000) >> 16); // msb

        // illuminance
        bth += 0x05;

        uint32_t i = datastore.sensors.illuminance * 100;
        bth += (char)(i & 0xFF);             // lsb
        bth += (char)((i & 0xFF00) >> 8);   
        bth += (char)((i & 0xFF0000) >> 16); // msb


        // // count packet
        // bth += 0x09;

        // bth += (char)(datastore.system.packetId & 0xFF);          // lsb
        // bth += (char)((datastore.system.packetId & 0xFF00) >> 8); // msb

        data.addData(bth);

        data.setName("BTHomeTest1");

        // std::string payload = data.getPayload();
        // for (size_t i = 0; i < payload.length(); i++)
        // {
        //     if (payload[i] > 32 && payload[i] < 127)
        //         ESP_LOGD(TASK_NAME_BLE, "payload[%2i]: %2X %3i %c", i, payload[i], payload[i], payload[i]);
        //     else
        //         ESP_LOGD(TASK_NAME_BLE, "payload[%2i]: %2X %3i", i, payload[i], payload[i]);
        // }

        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->setAdvertisementData(data);

        ESP_LOGI(TASK_NAME_BLE, "ble advert start");
        pAdvertising->start();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        pAdvertising->stop();
        ESP_LOGI(TASK_NAME_BLE, "ble advert stop");

        datastore.system.packetId++;
    }
}