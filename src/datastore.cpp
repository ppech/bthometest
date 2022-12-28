#include "datastore.h"
#include "esp32-hal-log.h"

DataStore datastore;

static RTC_DATA_ATTR uint8_t bootCount{0};

void datastore_init(void)
{
    ESP_LOGI("datastore", "boot count: %d", bootCount);
    datastore.system.bootCount = bootCount;
    datastore.system.packetId = 0;
    bootCount++;
}