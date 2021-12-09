#include <cstdint>
#include <cstring>
#include <Arduino.h>
#include "ButtonEvent.h"


#define GET_TICK()  millis()    //arduino Number of milliseconds passed since the program started. Data type: unsigned long.


/**
 * @brief   ����ʱ�乹�캯��
 *          �Գ�Ա�������г�ʼ��
 * @param   longPressTime ��������������ʱʱ��
 * @param   longPressTimeRepeat ���������ظ�����ʱ����
 * @param   doubleClickTime ˫��ʱ����
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
 * @brief   ��ȡ��ǰϵͳʱ������ڲ���ʱ��Ĳ�
 * @param   prevTick �ϴμ�¼��ʱ���
 * @retval  ʱ���
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
 * @brief   ����״̬�¼���
 * @param   function �¼��ص�����ָ��
 * @retval  none
 */
void ButtonEvent::EventAttach(FuncCallback_t function)
{
    priv.eventCallback = function;
}

/**
 * @brief   �����¼���� 10msѭ������
 * @param   isPress ��ǰ����״̬
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
