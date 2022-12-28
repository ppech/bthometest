#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>
#include <Wire.h>
#include "datastore.h"
#include "tasks.h"

#define I2C_MASTER_SCL_IO GPIO_NUM_22 /*!<gpio number for i2c master clock  */
#define I2C_MASTER_SDA_IO GPIO_NUM_21 /*!<gpio number for i2c master data */

Adafruit_BME280 bme;
unsigned bme_status;

BH1750 bh(0x23);
bool bh_status;

bool state = 0;

void task_sensors_entry(void *params)
{
    vTaskDelay(500 / portTICK_PERIOD_MS);

    ESP_LOGI(TASK_NAME_SENSORS, "Task SENSORS entry");

    Wire.begin();
    // byte error;
    // for (byte address = 1; address < 127; address++)
    // {
    //     Wire.beginTransmission(address);
    //     error = Wire.endTransmission();
    //     if (error == 0)
    //     {
    //         ESP_LOGI(TASK_NAME_SENSORS, "I2C device found at address 0x%2X", address);
    //     }
    // }

    // status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
    bme_status = bme.begin(0x76);
    if (bme_status)
    {
        ESP_LOGW(TASK_NAME_SENSORS, "BME280 sensor initialized");
    }
    else
    {
        ESP_LOGW(TASK_NAME_SENSORS, "Could not find a valid BME280 sensor");
        ESP_LOGI(TASK_NAME_SENSORS, "SensorID was: %#X", bme.sensorID());
    }

    bh_status = bh.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    if (bh_status)
    {
        ESP_LOGW(TASK_NAME_SENSORS, "BH1750 sensor initialized");
    }
    else
    {
        ESP_LOGW(TASK_NAME_SENSORS, "Could not find a valid BH1750 sensor");
    }

    pinMode(LED_BUILTIN, OUTPUT);

    for (;;)
    {
        if (bme_status)
        {
            datastore.sensors.temperature = bme.readTemperature();
            ESP_LOGI(TASK_NAME_SENSORS, "temperature: %f", datastore.sensors.temperature);
            datastore.sensors.humidity = bme.readHumidity();
            ESP_LOGI(TASK_NAME_SENSORS, "humidity: %f", datastore.sensors.humidity);
            datastore.sensors.pressure = bme.readPressure() / 100.0F;
            ESP_LOGI(TASK_NAME_SENSORS, "pressure: %f", datastore.sensors.pressure);
        }

        if(bh_status)
        {
            datastore.sensors.illuminance = bh.readLightLevel();
            ESP_LOGI(TASK_NAME_SENSORS, "illuminance: %f", datastore.sensors.illuminance);
        }

        state = !state;
        if (state)
            digitalWrite(LED_BUILTIN, HIGH);
        else
            digitalWrite(LED_BUILTIN, LOW);

        xTaskNotifyGive(datastore.system.taskBleHandle);

        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}