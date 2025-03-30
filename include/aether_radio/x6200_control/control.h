/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6200 Control
 *
 *  Copyright (c) 2022 Belousov Oleg a.k.a. R1CBU
 *  Copyright (c) 2022 Rui Oliveira a.k.a. CT7ALW
 */

#pragma once

#include <stdint.h>
#include "aether_radio/x6200_control/low/control.h"

typedef enum {
    X6200_VFO_A = 0,
    X6200_VFO_B
} x6200_vfo_t;

/* VFO Settings */

AETHER_X6200CTRL_API void x6200_control_vfo_set(x6200_vfo_t vfo);

AETHER_X6200CTRL_API void x6200_control_vfo_mode_set(x6200_vfo_t vfo, x6200_mode_t mode);
AETHER_X6200CTRL_API void x6200_control_vfo_agc_set(x6200_vfo_t vfo, x6200_agc_t agc);
AETHER_X6200CTRL_API void x6200_control_vfo_freq_set(x6200_vfo_t vfo, uint32_t freq);
AETHER_X6200CTRL_API void x6200_control_vfo_att_set(x6200_vfo_t vfo, x6200_att_t att);
AETHER_X6200CTRL_API void x6200_control_vfo_pre_set(x6200_vfo_t vfo, x6200_pre_t pre);

/* Radio */

AETHER_X6200CTRL_API void x6200_control_rfg_set(uint8_t rfg);       /* RF gain 0 - 100 */
AETHER_X6200CTRL_API void x6200_control_txpwr_set(float pwr);       /* TX power 0 - 10.0 */
AETHER_X6200CTRL_API void x6200_control_sql_set(uint8_t sql);       /* Voice SQL 0 - 100 */
AETHER_X6200CTRL_API void x6200_control_sql_fm_set(uint8_t sql);
AETHER_X6200CTRL_API void x6200_control_sql_enable_set(bool on);
AETHER_X6200CTRL_API void x6200_control_monitor_level_set(uint8_t level);
AETHER_X6200CTRL_API void x6200_control_fft_dec_set(uint8_t dec);
AETHER_X6200CTRL_API void x6200_control_fft_zoom_cw_set(uint8_t zoom);

/* Sound settings */

AETHER_X6200CTRL_API void x6200_control_rxvol_set(uint8_t vol);     /* Speaker volume */
AETHER_X6200CTRL_API void x6200_control_record_set(bool on);        /* Toggle audio stream mic/speaker or RX/TX audio */
AETHER_X6200CTRL_API void x6200_control_spmode_set(bool phone);     /* Toggle Speaker/Phone audio out */

/* Filters */
AETHER_X6200CTRL_API void x6200_control_rx_filter_set(int16_t low, int16_t high);
AETHER_X6200CTRL_API void x6200_control_rx_filter_set_low(int16_t low);
AETHER_X6200CTRL_API void x6200_control_rx_filter_set_high(int16_t high);

// AETHER_X6200CTRL_API void x6200_control_ssb_filter_set_set(uint16_t low, uint16_t high);
// AETHER_X6200CTRL_API void x6200_control_cw_filter_set_set(uint16_t bw);
// AETHER_X6200CTRL_API void x6200_control_am_filter_set_set(uint16_t bw);
// AETHER_X6200CTRL_API void x6200_control_nfm_filter_set_set(uint16_t bw);
// AETHER_X6200CTRL_API void x6200_control_wfm_filter_set_set(uint16_t bw);

/* Operation */

AETHER_X6200CTRL_API void x6200_control_ptt_set(bool on);           /* Toggle PTT signal */
AETHER_X6200CTRL_API void x6200_control_atu_tune(bool on);          /* Toggle ATU tuning process */
AETHER_X6200CTRL_API void x6200_control_poweroff();                 /* Poweroff the device */
AETHER_X6200CTRL_API void x6200_control_split_set(bool on);         /* Toggle split */
AETHER_X6200CTRL_API void x6200_control_atu_set(bool on);           /* Toggle using ATU */
AETHER_X6200CTRL_API void x6200_control_modem_set(bool on);         /* Toggle serial port for SMT32 firmware update */
AETHER_X6200CTRL_API void x6200_control_swrscan_set(bool on);       /* Toggle SWR scan process */
AETHER_X6200CTRL_API void x6200_control_calibration_set(bool on);   /* Toggle calibration process after update STM32 firmware */
AETHER_X6200CTRL_API void x6200_control_charger_set(bool on);       /* Toggle charger */
AETHER_X6200CTRL_API void x6200_control_bias_drive_set(uint16_t x);
AETHER_X6200CTRL_API void x6200_control_bias_final_set(uint16_t x);

/* Keyer settings */

AETHER_X6200CTRL_API void x6200_control_key_speed_set(uint8_t wpm);
AETHER_X6200CTRL_API void x6200_control_key_mode_set(x6200_key_mode_t mode);
AETHER_X6200CTRL_API void x6200_control_iambic_mode_set(x6200_iambic_mode_t mode);
AETHER_X6200CTRL_API void x6200_control_key_tone_set(uint16_t tone);
AETHER_X6200CTRL_API void x6200_control_key_vol_set(uint16_t vol);
AETHER_X6200CTRL_API void x6200_control_key_train_set(bool train);
AETHER_X6200CTRL_API void x6200_control_qsk_time_set(uint16_t time);
AETHER_X6200CTRL_API void x6200_control_key_ratio_set(float ratio);

/* Line's settings */

AETHER_X6200CTRL_API void x6200_control_linein_set(uint8_t gain);
AETHER_X6200CTRL_API void x6200_control_lineout_set(uint8_t gain);
AETHER_X6200CTRL_API void x6200_control_iqout_set(bool on);

/* Mic settings */

AETHER_X6200CTRL_API void x6200_control_imic_set(uint8_t gain);         /* Built-in MIC gain */
AETHER_X6200CTRL_API void x6200_control_hmic_set(uint8_t gain);         /* Handle MIC gain */
AETHER_X6200CTRL_API void x6200_control_mic_set(x6200_mic_sel_t mic);   /* MIC selector */

/* DSP */

AETHER_X6200CTRL_API void x6200_control_dnf_set(x6200_dnf_mode_t mode);               /* Notch filter */
AETHER_X6200CTRL_API void x6200_control_dnf_center_set(uint16_t freq);
AETHER_X6200CTRL_API void x6200_control_dnf_width_set(uint16_t hz);

AETHER_X6200CTRL_API void x6200_control_nb_set(bool on);                /* Noise blanker */
AETHER_X6200CTRL_API void x6200_control_nb_level_set(uint8_t level);
AETHER_X6200CTRL_API void x6200_control_nb_width_set(uint8_t hz);

AETHER_X6200CTRL_API void x6200_control_nr_set(bool on);                /* Noise reduce */
AETHER_X6200CTRL_API void x6200_control_nr_level_set(uint8_t level);

/* AGC */

AETHER_X6200CTRL_API void x6200_control_agc_hang_set(bool on);
AETHER_X6200CTRL_API void x6200_control_agc_knee_set(int8_t db);
AETHER_X6200CTRL_API void x6200_control_agc_slope_set(uint8_t db);
AETHER_X6200CTRL_API void x6200_control_agc_time_set(uint16_t ms);

/* VOX */

AETHER_X6200CTRL_API void x6200_control_vox_set(bool on);
AETHER_X6200CTRL_API void x6200_control_vox_ag_set(uint8_t level);      /* 0 - 100% */
AETHER_X6200CTRL_API void x6200_control_vox_delay_set(uint16_t ms);     /* 100 - 2000 ms */
AETHER_X6200CTRL_API void x6200_control_vox_gain_set(uint8_t level);    /* 0 - 100% */


/* RIT/XIT */
AETHER_X6200CTRL_API void x6200_control_rit_set(int16_t val);
AETHER_X6200CTRL_API void x6200_control_xit_set(int16_t val);


/* COMP */

AETHER_X6200CTRL_API void x6200_control_comp_set(bool on);
AETHER_X6200CTRL_API void x6200_control_comp_level_set(x6200_comp_level_t level);


/* RX EQ */
AETHER_X6200CTRL_API void x6200_control_rx_eq_set(bool on);
AETHER_X6200CTRL_API void x6200_control_rx_eq_p1_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_p2_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_p3_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_p4_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_p5_set(int8_t level);

/* RX EQ WFM */
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_set(bool on);
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_p1_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_p2_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_p3_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_p4_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_rx_eq_wfm_p5_set(int8_t level);

/* MIC EQ */
AETHER_X6200CTRL_API void x6200_control_mic_eq_set(bool on);
AETHER_X6200CTRL_API void x6200_control_mic_eq_p1_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_mic_eq_p2_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_mic_eq_p3_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_mic_eq_p4_set(int8_t level);
AETHER_X6200CTRL_API void x6200_control_mic_eq_p5_set(int8_t level);
