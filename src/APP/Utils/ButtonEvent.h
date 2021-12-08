#ifndef BUTTON_EVENT_H_
#define BUTTON_EVENT_H_

class ButtonEvent
{
private:
    typedef void(*FuncCallback_t)(ButtonEvent *btn, int event);

public:
    typedef enum
    {
        #define EVENT_DEF(evt) evt
        #include "EventType.inc"
        #undef EVENT_DEF
        _EVENT_LAST
    };

public:
    bool IsPressed;
    bool IsClicked;
    bool IsLongPressed;

public:
    ButtonEvent(
        uint16_t longPressTime = 500,
        uint16_t longPressTimeRepeat = 200,
        uint16_t doubleClickTime = 200);          //定义类的构造函数

    void EventAttach(FuncCallback_t function);
    void EventMonitor(bool isPress);


private:
    typedef enum
    {
        STATE_NO_PRESS,
        STATE_PRESS,
        STATE_LONG_PRESS,

    }State_t;

private:
    struct
    {
        State_t nowState;                       //按键当前状态
        uint16_t longPressTimeCfg;              //长按时间
        uint16_t longPressRepeatTimeCfg;
        uint16_t doubleClickTimeCfg;

        uint32_t lastLongPressTime;
        uint32_t lastPressTime;
        uint32_t lastClickTime;
        

        bool isLongPressed;
        FuncCallback_t eventCallback;

    }priv;

private:
    uint32_t GetTickElaps(uint32_t prevTick);
};

#endif


