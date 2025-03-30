/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6200 Control
 *
 *  Copyright (c) 2022 Belousov Oleg a.k.a. R1CBU
 *  Copyright (c) 2022 Rui Oliveira a.k.a. CT7ALW
 */

#include <unistd.h>
#include <stdio.h>

#include <aether_radio/x6200_control/control.h>
#include <aether_radio/x6200_control/low/flow.h>
#include <aether_radio/x6200_control/low/gpio.h>

typedef enum {
    ATU_IDLE = 0,
    ATU_START,
    ATU_RUN,
    ATU_DONE
} atu_status_t;

static x6200_flow_t pack;

int main() {
    atu_status_t atu = ATU_IDLE;

    if (!x6200_control_init())
        return 1;

    if (!x6200_flow_init())
        return 1;

    if (!x6200_gpio_init())
        return 1;

    x6200_control_vfo_freq_set(X6200_VFO_A, 7135000);

    while (atu != ATU_DONE) {
        if (!x6200_flow_read(&pack)) {
            usleep(25000);
            continue;
        }

        printf("tx=%d "
               "txpwr=%.1f swr=%.1f alc=%.1f vext=%.1f vbat=%.1f bat=%d atu_params=%08X\n",
               pack.flag.tx, pack.tx_power * 0.1, pack.vswr * 0.1f, pack.alc_level * 0.1,
               pack.vext * 0.1f, pack.vbat * 0.1f, pack.batcap, pack.atu_params);

        switch (atu)
        {
        case ATU_IDLE:
            x6200_control_atu_tune(true);
            x6200_gpio_set(x6200_pin_light, 1);
            atu = ATU_START;
            break;

        case ATU_START:
            if (pack.flag.tx)
            {
                atu = ATU_RUN;
            }
            break;

        case ATU_RUN:
            if (!pack.flag.tx)
            {
                x6200_control_atu_tune(false);
                x6200_gpio_set(x6200_pin_light, 0);
                atu = ATU_DONE;
            }
            break;
        }
    }
}
