#include "Display.h"


TaskHandle_t handleTaskLvgl;


void TaskLvglUpdate(void* parameter)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    while(1)
    {
        lv_task_handler();
        
        delay(10);
    }
}


void lvgl_task_creat(void)
{
    // Update display in parallel thread.
    if (pdPASS == xTaskCreate(
        TaskLvglUpdate,
        "LvglThread",
        20000,
        nullptr,
        configMAX_PRIORITIES - 1,
        &handleTaskLvgl))
    {
        Serial.println("Lvgl task create success!");
    }
    else
    {
        Serial.println("Lvgl task create failed!");

    }
}
