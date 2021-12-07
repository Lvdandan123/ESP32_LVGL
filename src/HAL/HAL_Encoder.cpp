#include "HAL/HAL.h"

void HAL::Encoder_Init()
{
    pinMode(CONFIG_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_PUSH_PIN, INPUT_PULLUP);
}

bool HAL::Encoder_GetIsPush()
{
    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
}