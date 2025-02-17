/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6100 Control
 *
 *  Copyright (c) 2022 Belousov Oleg aka R1CBU
 *  Copyright (c) 2022 Rui Oliveira aka CT7ALW
 */

#include "aether_radio/x6100_control/low/gpio.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gpiod.h>

#define EXIT_ON_FALSE(val, msg) ({if (!val) {fprintf(stderr, "%s\n", msg); return false;}})
#define X6100_PIN_WIFI 357
#define X6100_PIN_USB 138
#define X6100_PIN_LIGHT 143
#define X6100_PIN_MORSE_KEY 203
#define X6100_PIN_BB_RESET 204


static struct gpiod_chip *chip0;
static struct gpiod_chip *chip1;
static struct gpiod_line *line_morse_key;
static struct gpiod_line *line_bb_reset;
static struct gpiod_line *line_usb;
static struct gpiod_line *line_light;
static struct gpiod_line *line_wifi;


int x6100_pin_wifi = X6100_PIN_WIFI;
int x6100_pin_usb = X6100_PIN_USB;
int x6100_pin_light = X6100_PIN_LIGHT;
int x6100_pin_morse_key = X6100_PIN_MORSE_KEY;
int x6100_pin_bb_reset = X6100_PIN_BB_RESET;


static bool gpio_chip_open(const char * name, struct gpiod_chip **chip) {
    *chip = gpiod_chip_open_by_name(name);
    if (*chip == NULL) {
        return false;
    }
    return true;
}

static bool gpio_line_open(unsigned int pin, const char * consumer, struct gpiod_chip *chip, int val, struct gpiod_line **line) {
    *line = gpiod_chip_get_line(chip, pin);
    if (*line == NULL) {
        return false;
    }
    if (gpiod_line_request_output(*line, consumer, val) != 0) {
        return false;
    }
    return true;
}

bool x6100_gpio_init()
{
    EXIT_ON_FALSE(gpio_chip_open("gpiochip0", &chip0), "Can't open gpio chip 0");
    EXIT_ON_FALSE(gpio_chip_open("gpiochip1", &chip1), "Can't open gpio chip 1");

    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_MORSE_KEY, "X6100_morse_key", chip1, 1, &line_morse_key), "Can't open GPIO morse line");
    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_BB_RESET, "X6100_bb_reset", chip1, 0, &line_bb_reset), "Can't open GPIO bb reset line");
    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_USB, "X6100_usb", chip1, 0, &line_usb), "Can't open GPIO usb line");
    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_LIGHT, "X6100_light", chip1, 0, &line_light), "Can't open GPIO light line");
    EXIT_ON_FALSE(gpio_line_open(5, "X6100_wifi", chip0, 0, &line_wifi), "Can't open GPIO wifi line");
    return true;
}

void x6100_gpio_set(int pin, int value)
{
    switch (pin)
    {
    case X6100_PIN_MORSE_KEY:
        gpiod_line_set_value(line_morse_key, value);
        break;
    case X6100_PIN_BB_RESET:
        gpiod_line_set_value(line_bb_reset, value);
        break;
    case X6100_PIN_USB:
        gpiod_line_set_value(line_usb, value);
        break;
    case X6100_PIN_LIGHT:
        gpiod_line_set_value(line_light, value);
        break;
    case X6100_PIN_WIFI:
        gpiod_line_set_value(line_wifi, value);
        break;

    default:
        printf("Unknown pin: %i\n", pin);
        break;
    }
}
