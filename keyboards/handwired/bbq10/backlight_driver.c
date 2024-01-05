// Copyright 2024 viteo (viteo@tut.by)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>

#ifdef BACKLIGHT_ENABLE
#include "backlight.h"

#define PWM_WIDTH 0xFFFFUL
#define PWM_PERIOD 256

static uint8_t backlight_userlevel = BACKLIGHT_DEFAULT_LEVEL;
static PWMConfig pwmcfg_backlight = {
    PWM_WIDTH,  /* PWM clock frequency     */
    PWM_PERIOD, /* PWM period (in ticks)   */
    NULL,       /* Period callback         */
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH1 mode and callback    */
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH2 mode and callback    */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH3 mode and callback    */
        {PWM_OUTPUT_DISABLED, NULL}     /* CH4 mode and callback    */
    },
    0, /* TIMx_CR2 register     */
    0  /* TIMx_DIER register    */
};

// See https://jared.geek.nz/2013/02/linear-led-pwm/
static uint16_t cie_lightness(uint16_t width)
{
    if (width <= (PWM_WIDTH * 8 / 100)) // if below 8% of max
        return width / 9;               // same as dividing by 900%
    else
    {
        uint32_t y = (((uint32_t)width + (PWM_WIDTH * 16 / 100)) << 8) / ((PWM_WIDTH * 16 / 100) + PWM_WIDTH); // add 16% of max and compare
        // to get a useful result with integer division, we shift left in the expression above
        // and revert what we've done again after squaring.
        y = y * y * y >> 8;
        if (y > PWM_WIDTH) // prevent overflow
            return PWM_WIDTH;
        else
            return (uint16_t)y;
    }
}

void backlight_init_ports(void)
{
    palSetPadMode(PWM_PORT, PWM_PIN1, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    palSetPadMode(PWM_PORT, PWM_PIN2, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    pwmStart(&PWMD3, &pwmcfg_backlight);
    backlight_set(0);
}

void backlight_set(uint8_t level)
{
    backlight_userlevel = level;
    if (level == 0)
    {
        // Turn backlight off
        pwmDisableChannel(&PWMD3, 0);
        pwmDisableChannel(&PWMD3, 1);
    }
    else
    {
        uint32_t duty = (uint32_t)(cie_lightness(0xFFFFUL * level / BACKLIGHT_LEVELS));
        pwmEnableChannelI(&PWMD3, 0, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_WIDTH, duty));
        pwmEnableChannelI(&PWMD3, 1, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_WIDTH, duty));
    }
}

#ifdef BACKLIGHT_BREATHING

static void breathing_callback(PWMDriver *pwmp);
static PWMConfig pwmcfg_breathing = {
    PWM_WIDTH,          /* PWM clock frequency     */
    PWM_PERIOD,         /* PWM period (in ticks)   */
    breathing_callback, /* Period callback         */
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH1 mode and callback    */
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH2 mode and callback    */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH3 mode and callback    */
        {PWM_OUTPUT_DISABLED, NULL}     /* CH4 mode and callback    */
    },
    0, /* TIMx_CR2 register     */
    0  /* TIMx_DIER register    */
};

/* To generate breathing curve in python:
 * from math import sin, pi; [int(sin(x/128.0*pi)**4*255) for x in range(128)]
 */
#define BREATHING_STEPS 128
static const uint8_t breathing_table[BREATHING_STEPS] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   2,   3,   4,   5,   6,   8,   10,  12,  15,
    17,  20,  24,  28,  32,  36,  41,  46,  51,  57,  63,  70,  76,  83,  91,  98,  106, 113, 121, 129, 138, 146,
    154, 162, 170, 178, 185, 193, 200, 207, 213, 220, 225, 231, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254,
    253, 252, 250, 247, 244, 240, 235, 231, 225, 220, 213, 207, 200, 193, 185, 178, 170, 162, 154, 146, 138, 129,
    121, 113, 106, 98,  91,  83,  76,  70,  63,  57,  51,  46,  41,  36,  32,  28,  24,  20,  17,  15,  12,  10,
    8,   6,   5,   4,   3,   2,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

// Use this before the cie_lightness function.
static inline uint16_t scale_backlight(uint16_t v)
{
    return v / BACKLIGHT_LEVELS * backlight_userlevel;
}

static void breathing_callback(PWMDriver *pwmp)
{
    (void)pwmp;
    static uint16_t breathing_counter = 0;
    uint16_t interval = (uint16_t)BREATHING_PERIOD * PWM_PERIOD / BREATHING_STEPS;
    
    // resetting after one period to prevent ugly reset at overflow.
    breathing_counter = (breathing_counter + 1) % (BREATHING_PERIOD * PWM_PERIOD);
    uint8_t index = breathing_counter / interval % BREATHING_STEPS;

    uint32_t duty = cie_lightness(scale_backlight(breathing_table[index] * PWM_PERIOD));

    chSysLockFromISR();
    pwmEnableChannelI(&PWMD3, 0, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_WIDTH, duty));
    pwmEnableChannelI(&PWMD3, 1, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_WIDTH, duty));
    chSysUnlockFromISR();
}

void breathing_enable(void)
{
    pwmStop(&PWMD3);
    pwmStart(&PWMD3, &pwmcfg_breathing);
    pwmEnablePeriodicNotification(&PWMD3);
    pwmEnableChannelI(&PWMD3, 0, PWM_WIDTH); // full width or PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_WIDTH, x)
    pwmEnableChannelI(&PWMD3, 1, PWM_WIDTH);
    backlight_set(backlight_userlevel);
}

void breathing_disable(void)
{
    pwmStop(&PWMD3);
    pwmStart(&PWMD3, &pwmcfg_backlight);
    // Restore backlight level
    backlight_set(backlight_userlevel);
}

#endif // BACKLIGHT_BREATHING
#endif // BACKLIGHT_ENABLE