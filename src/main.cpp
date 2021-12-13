#include <Arduino.h>
#if 1
#include "Port/Display.h"
#include "HAL/HAL.h"
#include "APP/Configs/Config.h"


// Use hardware SPI
static SCREEN_CLASS Screen;
extern lv_color_t* lv_disp_buf_p;

/* example */
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());           /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);                /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);

    lv_group_t * group = lv_group_create();
    lv_group_add_obj(group, btn);
    lv_indev_set_group(indev_encoder, group);
}


/**
 * @brief  初始化接口
 */
void setup() {
    lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");
    
    /* 初始化串口波特率为115200 */
    Serial.begin(115200);     
    Serial.println("In Application.......");

    /* 电源保持使能 */
    Serial.println("Power: Wating......");
    pinMode(CONFIG_POWER_EN_PIN, OUTPUT);
    digitalWrite(CONFIG_POWER_EN_PIN, HIGH);
    Serial.println("Power: ON!");
    
    /* SD 卡初始化 */
    // HAL::SD_Init();
    
    /* Encoder 初始化 */
    HAL::Encoder_Init();
    
    /* 初始化spi lcd 硬件接口 */
    Screen.begin();              
    Screen.setRotation(0);

    pinMode(CONFIG_SCREEN_BLK_PIN, OUTPUT);
    digitalWrite(CONFIG_SCREEN_BLK_PIN, HIGH);

    /* lvgl库初始化 */
    lv_init();
    lv_port_disp_init(&Screen);
    lv_port_indev_init();

    /* GUI线程 */
    lvgl_task_creat();
    xTaskNotifyGive(handleTaskLvgl);

    /* 例子 */
    lv_example_get_started_1();

}


/**
 * @brief 循环接口
 */
void loop() {
    HAL::Update();
    delay(20);
}
#endif

