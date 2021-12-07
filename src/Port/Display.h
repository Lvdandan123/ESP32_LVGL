/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "lvgl.h"
#include "TFT_eSPI.h"


#define DISP_HOR_RES         240
#define DISP_VER_RES         240
#define DISP_BUF_SIZE        (DISP_HOR_RES * DISP_VER_RES / 2)


typedef TFT_eSPI SCREEN_CLASS;


void lv_port_disp_init(SCREEN_CLASS* scr);
void lv_port_indev_init();
void lvgl_task_creat(void);

// void Port_Init();
// void DisplayFault_Init(SCREEN_CLASS* scr);
// void lv_fs_if_init();

extern TaskHandle_t handleTaskLvgl;

#endif
