/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "../Display.h"
#include "HAL/HAL.h"

// static void encoder_init(void);
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
// static void encoder_handler(void);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_encoder;
// static int32_t encoder_diff;
// static lv_indev_state_t encoder_state;


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_port_indev_init(void)
{
    /*------------------
     * Encoder
     * -----------------*/

    static lv_indev_drv_t indev_drv;

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    data->enc_diff = HAL::Encoder_GetDiff();  //自上次读数以来的步数增减
    data->state = HAL::Encoder_GetIsPush() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    // data->enc_diff = encoder_diff;
    // data->state = encoder_state;
}

/*Call this function in an interrupt to process encoder events (turn, press)*/
// void encoder_handler(void)
// {
//     static uint8_t state = 0;

//     state = !state;

//     encoder_diff += 1;
//     encoder_state = (state ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
// }

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
