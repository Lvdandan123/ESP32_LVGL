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
    }Event_t;

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
    const char* GetEventString(uint16_t event)
    {
        const char* eventStr[_EVENT_LAST] = 
        {
            # define EVENT_DEF(evt) #evt
            # include "EventType.inc"
            # undef EVENT_DEF
        };

        return (event < _EVENT_LAST) ? eventStr[event] : "EVENT_NOT_FOUND";
    }

    inline uint16_t GetClickCnt()
    {
        uint16_t cnt = priv.clickCnt + 1;
        priv.clickCnt = 0;
        return cnt;
    }

    inline bool GetClicked()
    {
        bool n = IsClicked;
        IsClicked = false;
        return n;
    }

    inline bool GetPressed()
    {
        bool n = IsPressed;
        IsPressed = false;
        return n;
    }

    inline bool GetLongPressed()
    {
        bool n = IsLongPressed;
        IsLongPressed = false;
        return n;
    }

    operator uint8_t()      //运算符重载为类的成员函数
    {
        return priv.nowState;
    }


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
        
        uint16_t clickCnt;
        bool isLongPressed;
        FuncCallback_t eventCallback;

    }priv;

private:
    uint32_t GetTickElaps(uint32_t prevTick);
};

#endif


