// Copyright 2024 viteo (viteo@tut.by)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* enter bootloader on '$' button */
#define BOOTMAGIC_LITE_ROW 4
#define BOOTMAGIC_LITE_COLUMN 4

/* backlight pwm pins */
#define PWM_PORT IOPORT1  // GPIOA on STM32 Bluepill
#define PWM_PIN1 6        // PA6 TIM3 Channel 1
#define PWM_PIN2 7        // PA7 TIM3 Channel 2

/* backlight settings */
#define BACKLIGHT_DEFAULT_ON 1
#define BACKLIGHT_LEVELS 7
#define BACKLIGHT_DEFAULT_LEVEL (BACKLIGHT_LEVELS / 2)

/* backlight breathing settings */
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 4 //seconds
