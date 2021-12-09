#include <cstdint>
#include <cstring>
#include <Arduino.h>
#include "ButtonEvent.h"


#define GET_TICK()  millis()    //arduino Number of milliseconds passed since the program started. Data type: unsigned long.


/**
 * @brief   按键时间构造函数
 *          对成员变量进行初始化
 * @param   longPressTime 按键长按触发超时时间
 * @param   longPressTimeRepeat 长按按键重复触发时间间隔
 * @param   doubleClickTime 双击时间间隔
 * @retval  none
 */
ButtonEvent::ButtonEvent(
    uint16_t longPressTime, 
    uint16_t longPressTimeRepeat,
    uint16_t doubleClickTime)
{
    memset(&this->priv, 0, sizeof(this->priv));

    priv.longPressTimeCfg = longPressTime;
    priv.longPressRepeatTimeCfg = longPressTimeRepeat;
    priv.doubleClickTimeCfg = doubleClickTime;

    priv.lastLongPressTime = priv.lastClickTime = priv.lastPressTime = 0;
    priv.isLongPressed = false;
    priv.nowState = STATE_NO_PRESS;
    
    priv.eventCallback = nullptr;
}

/**
 * @brief   获取当前系统时间与入口参数时间的差
 * @param   prevTick 上次记录的时间戳
 * @retval  时间差
 */
uint32_t ButtonEvent::GetTickElaps(uint32_t prevTick)
{
    uint32_t curTime = GET_TICK();

    if (curTime > prevTick)
    {
        curTime -= prevTick;
    }
    else
    {
        curTime += 0xFFFFFFFF - prevTick +1;
    }

    return curTime;
}

/**
 * @brief   按键状态事件绑定
 * @param   function 事件回调函数指针
 * @retval  none
 */
void ButtonEvent::EventAttach(FuncCallback_t function)
{
    priv.eventCallback = function;
}

/**
 * @brief   按键事件监控 10ms循环调用
 * @param   isPress 当前按键状态
 * @retval  none
 */
void ButtonEvent::EventMonitor(bool isPress)
{
    if (nullptr == priv.eventCallback)  return;

    if (isPress && (priv.nowState == STATE_NO_PRESS))
    {
        priv.nowState = STATE_PRESS;

        IsPressed = true;
        priv.lastPressTime = GET_TICK();

        priv.eventCallback(this, EVENT_PRESSED);
        priv.eventCallback(this, EVENT_CHANGED);
    }

    if (priv.nowState == STATE_NO_PRESS) return;

    if (isPress && GetTickElaps(priv.lastPressTime) >= priv.longPressTimeCfg)
    {
        priv.nowState = STATE_LONG_PRESS;

        if(!priv.isLongPressed)
        {
            priv.eventCallback(this, EVENT_LONG_PRESSED);
            priv.lastLongPressTime = GET_TICK();
            IsLongPressed = priv.isLongPressed = true;
        }
        else if(GetTickElaps(priv.lastLongPressTime) >= priv.longPressRepeatTimeCfg)
        {
            priv.lastLongPressTime = GET_TICK();
            priv.eventCallback(this, EVENT_LONG_PRESSED_REPEAT);
        }
    }
    else if (!isPress)
    {
        priv.nowState = STATE_NO_PRESS;

        if(GetTickElaps(priv.lastClickTime) < priv.doubleClickTimeCfg)
        {
            priv.clickCnt++;
            priv.eventCallback(this, EVENT_DOUBLE_CLICKED);
        }

        if(priv.isLongPressed)
        {
            priv.eventCallback(this, EVENT_LONG_PRESSED_RELEASED);
        }

        priv.isLongPressed = false;
        IsClicked = true;
        priv.lastClickTime = GET_TICK();

        if(GetTickElaps(priv.lastPressTime) < priv.longPressTimeCfg)
        {
            priv.eventCallback(this, EVENT_SHORT_CLICKED);
        }

        priv.eventCallback(this, EVENT_CLICKED);
        priv.eventCallback(this, EVENT_RELEASED);
        priv.eventCallback(this, EVENT_CHANGED);
    }

}
