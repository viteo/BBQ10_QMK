// Copyright 2024 viteo (viteo@tut.by)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include_next <mcuconf.h>

#undef STM32_PWM_USE_TIM3
#define STM32_PWM_USE_TIM3 TRUE
