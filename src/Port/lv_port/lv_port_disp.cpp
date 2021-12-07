/*********************
 *      INCLUDES
 *********************/
#include "../Display.h"




lv_color_t* lv_disp_buf_p;
static lv_disp_draw_buf_t disp_buf;
static lv_disp_drv_t disp_drv; 


#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc )
{
   Serial.printf( "%s(%s)@%d->%s\r\n", file, fn_name, line, dsc );
   Serial.flush();
}
#endif

/**
 * @brief   lvgl数据刷新接口
 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    SCREEN_CLASS* p_Screen = (SCREEN_CLASS*) disp_drv->user_data;

    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;

    p_Screen->startWrite();
    p_Screen->setAddrWindow(area->x1, area->y1, w, h);
    p_Screen->pushColors (( uint16_t * )&color_p->full, w*h, true);
    p_Screen->endWrite();

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

static void disp_wait_cb(lv_disp_drv_t* disp_drv)
{
//    __wfi();
}

/**
 * @brief lvgl初始化接口
 */
void lv_port_disp_init(SCREEN_CLASS* scr)
{
#if LV_USE_LOG != 0
   lv_log_register_print_cb(reinterpret_cast<lv_log_print_g_cb_t>(my_print)); /* register print function for debugging */
#endif

    /* Move the malloc process to Init() to make sure that the largest heap can be used for this buffer.*/
  /*   lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n"); */
    lv_disp_draw_buf_init(&disp_buf, lv_disp_buf_p, nullptr, DISP_BUF_SIZE);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.wait_cb = disp_wait_cb;
    disp_drv.user_data = scr;
    lv_disp_drv_register(&disp_drv);
}


