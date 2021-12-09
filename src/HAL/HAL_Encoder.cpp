#include "HAL/HAL.h"
#include "App/Utils/ButtonEvent/ButtonEvent.h"
#include <Arduino.h>

/* 定义一个ButtonEvent静态对象 */
static ButtonEvent EncoderPush(5000);

static bool EncoderEnable = true;
static bool EncoderDiffDisable = false;
static volatile int16_t EncoderDiff = 0;


/**
 * @brief   编码器中断回调
 * TODO 有时间修改下这驱动
 */
static void Encoder_IrqHandler()
{
    if (!EncoderEnable || EncoderDiffDisable)
    {
        return;
    }

    static volatile int count, countLast;
    static volatile uint8_t a0, b0;
    static volatile uint8_t ab0;
    uint8_t a = digitalRead(CONFIG_ENCODER_A_PIN);
    uint8_t b = digitalRead(CONFIG_ENCODER_B_PIN);

    if (a != a0)    //A端电平发生了变化
    {
        a0 = a;
        if (b != b0)
        {
            b0 = b;
            count += ((a == b) ? 1 : -1);
            if ((a == b) != ab0)
            {
                count += ((a == b) ? 1 : -1);
            }
            ab0 = (a == b);
        }
    }

    if (count != countLast)
    {
        EncoderDiff += (count - countLast) > 0 ? 1 : -1;
        countLast = count;
    }
}

/**
 * @brief   编码器按下处理
 */
static void Encoder_PushHandler(ButtonEvent* bth, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        // HAL::Buzz_Tone(500, 20);
        EncoderDiffDisable = true;      //按键按下后禁用滚轮操作
    }
    else if (event == ButtonEvent::EVENT_RELEASED)
    {
        // HAL::Buzz_Tone(700, 20);
        EncoderDiffDisable = false; 
    }
    else if (event == ButtonEvent::EVENT_RELEASED)       //长按关机
    {
        // HAL::Audio_PlayMusic("Shutdown");
        // HAL::Power_Shutdown();
    }
}

/**
 * @brief   编码器旋转处理
 */
static void Encoder_RotateHandler(int16_t diff)
{
    // HAL::Buzz_Tone(300, 5);

    // actEncoder->Commit((const void*) &diff, sizeof(int16_t));
    // actEncoder->Publish();
}

/**
 * @brief   编码器初始化
 */
void HAL::Encoder_Init()
{
    //GPIO初始化
    pinMode(CONFIG_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_PUSH_PIN, INPUT_PULLUP);

    attachInterrupt(CONFIG_ENCODER_A_PIN, Encoder_IrqHandler, CHANGE);

    EncoderPush.EventAttach(Encoder_PushHandler);

    // actEncoder = new Account("Encoder", AccountSystem::Broker(), sizeof(int16_t), nullptr);
}

/**
 * @brief   需要循环执行
 */
void HAL::Encoder_Update()
{
    EncoderPush.EventMonitor(Encoder_GetIsPush());
}

int16_t HAL::Encoder_GetDiff()
{
    int16_t diff = -EncoderDiff / 2;

    if (diff != 0)
    {
        EncoderDiff = 0;
        Encoder_RotateHandler(diff);
    }

    return diff;
}

bool HAL::Encoder_GetIsPush()
{
    if (!EncoderEnable)
    {
        return false;
    }

    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
}

void HAL::Encoder_SetEnable(bool en)
{
    EncoderEnable = en;
}
