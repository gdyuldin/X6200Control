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


static struct gpiod_chip *chip1;
static struct gpiod_line *line_morse_key;
static struct gpiod_line *line_bb_reset;


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

static int gpio_sysfs_open_value(uint16_t pin)
{
    char str[64];
    int fd, len;

    snprintf(str, sizeof(str), "/sys/class/gpio/gpio%i/value", pin);

    return open(str, O_RDWR);
}

static bool gpio_sysfs_export(uint16_t pin)
{
    char str[64];
    int fd, len;

    len = snprintf(str, sizeof(str), "%i\n", pin);
    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (fd < 0)
        return false;

    if (write(fd, str, len) < 0)
    {
        close(fd);
        return false;
    }

    close(fd);

    snprintf(str, sizeof(str), "/sys/class/gpio/gpio%i/direction", pin);
    fd = open(str, O_WRONLY);

    if (fd < 0)
        return false;

    if (write(fd, "out\n", 4) < 0)
    {
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

static int gpio_sysfs_open(uint16_t pin)
{
    int fd = gpio_sysfs_open_value(pin);

    if (fd < 0)
    {
        if (gpio_sysfs_export(pin))
        {
            fd = gpio_sysfs_open_value(pin);
        }
        else
        {
            fd = -1;
        }
    }

    return fd;
}

static int gpio_sysfs_write(int pin, int value)
{
    char str[8];

    int len = snprintf(str, sizeof(str), "%i\n", value);
    len = write(pin, str, len);
}

bool x6100_gpio_init()
{
    EXIT_ON_FALSE(gpio_chip_open("gpiochip1", &chip1), "Can't open gpio chip 1");

    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_MORSE_KEY, "X6100_morse_key", chip1, 1, &line_morse_key), "Can't open GPIO morse line");
    EXIT_ON_FALSE(gpio_line_open(X6100_PIN_BB_RESET, "X6100_bb_reset_key", chip1, 0, &line_bb_reset), "Can't open GPIO bb reset line");

    x6100_pin_wifi = gpio_sysfs_open(X6100_PIN_WIFI);
    x6100_pin_usb = gpio_sysfs_open(X6100_PIN_USB);
    x6100_pin_light = gpio_sysfs_open(X6100_PIN_LIGHT);
    return true;
}

void x6100_gpio_set(int pin, int value)
{
    bool use_sysfs = false;
    switch (pin)
    {
    case X6100_PIN_MORSE_KEY:
        gpiod_line_set_value(line_morse_key, value);
        break;
    case X6100_PIN_BB_RESET:
        gpiod_line_set_value(line_bb_reset, value);
        break;

    default:
        use_sysfs = true;
        break;
    }
    if (use_sysfs)
        gpio_sysfs_write(pin, value);
}
