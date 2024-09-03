    /*
    * This file is part of the Pico Keys SDK distribution (https://github.com/polhenarejos/pico-keys-sdk).
    * Copyright (c) 2022 Pol Henarejos.
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, version 3.
    *
    * This program is distributed in the hope that it will be useful, but
    * WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    * General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program. If not, see <http://www.gnu.org/licenses/>.
    */

#include <stdio.h>
#include <stdlib.h>
#include "pico_keys.h"
#ifdef PICO_PLATFORM
#include "bsp/board.h"
#elif defined(ESP_PLATFORM)
#include "esp_compat.h"
#elif defined(ENABLE_EMULATION)
#include "emulation.h"
#endif

extern void led_driver_init();
extern void led_driver_color(uint8_t);

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_set_blink(uint32_t mode) {
    blink_interval_ms = mode;
}

void led_blinking_task() {
#ifndef ENABLE_EMULATION
    static uint32_t start_ms = 0;
    static uint8_t led_state = false;
    uint8_t state = led_state;
#ifdef PICO_DEFAULT_LED_PIN_INVERTED
    state = !state;
#endif
    uint32_t led_color = (blink_interval_ms & LED_COLOR_MASK) >> LED_COLOR_SHIFT;
    uint32_t led_off = (blink_interval_ms & LED_OFF_MASK) >> LED_OFF_SHIFT;
    uint32_t led_on = (blink_interval_ms & LED_ON_MASK) >> LED_ON_SHIFT;
    uint32_t led_interval = led_state ? led_on : led_off;

    // Blink every interval ms
    if (board_millis() - start_ms < led_interval) {
        return; // not enough time
    }
    start_ms += led_interval;

    if (state == false) {
        led_driver_color(LED_COLOR_OFF);
    }
    else {
        led_driver_color(led_color);
    }
    led_state ^= 1; // toggle
#endif
}

void led_off_all() {
#ifndef ENABLE_EMULATION
    led_driver_color(LED_COLOR_OFF);
#endif
}

void led_init() {
#ifndef ENABLE_EMULATION
    led_driver_init();
    led_set_blink(BLINK_NOT_MOUNTED);
#endif
}