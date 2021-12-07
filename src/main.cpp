#include <Arduino.h>
#if 1
#include "Port/Display.h"
#include "HAL/HAL.h"
#include "APP/Configs/Config.h"


// Use hardware SPI
static SCREEN_CLASS Screen;
extern lv_color_t* lv_disp_buf_p;


static void event_cb(lv_event_t * e)
{
    LV_LOG_USER("Clicked");

    static unsigned int cnt = 1;
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "%u", cnt);
    cnt++;
}

/**
 * Add click event to a button
 */
void lv_example_event_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 100, 50);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Click me!");
    lv_obj_center(label);
}

/**
 * @brief  ��ʼ���ӿ�
 */
void setup() {
    lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");
    
    /* ��ʼ�����ڲ�����Ϊ115200 */
    Serial.begin(115200);     
    Serial.println("In Application.......");

    /* ��Դ����ʹ�� */
    Serial.println("Power: Wating......");
    pinMode(CONFIG_POWER_EN_PIN, OUTPUT);
    digitalWrite(CONFIG_POWER_EN_PIN, HIGH);
    Serial.println("Power: ON!");
    
    /* SD ����ʼ�� */
    // HAL::SD_Init();
    /* Encoder ��ʼ�� */
    HAL::Encoder_Init();
    
    /* ��ʼ��spi lcd Ӳ���ӿ� */
    Screen.begin();              
    Screen.setRotation(0);

    pinMode(CONFIG_SCREEN_BLK_PIN, OUTPUT);
    digitalWrite(CONFIG_SCREEN_BLK_PIN, HIGH);

    /* lvgl���ʼ�� */
    lv_init();
    lv_port_disp_init(&Screen);
    lv_port_indev_init();

    /* ���� */
    lv_example_event_1();

    lvgl_task_creat();

    xTaskNotifyGive(handleTaskLvgl);
}


/**
 * @brief ѭ���ӿ�
 */
void loop() {
    static uint32_t cnt = 0;

    if (HAL::Encoder_GetIsPush() && (++cnt > 300))
    {
        digitalWrite(CONFIG_POWER_EN_PIN, LOW);
    }
    
    delay(20);
}
#endif

