/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6200 Control
 *
 *  Copyright (c) 2022 Belousov Oleg a.k.a. R1CBU
 *  Copyright (c) 2022 Rui Oliveira a.k.a. CT7ALW
 */

#pragma once

#include "aether_radio/x6200_control/api.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    x6200_vfoa_ham_band = 0,
    x6200_vfoa_freq,
    x6200_vfoa_att,
    x6200_vfoa_pre,
    x6200_vfoa_mode,
    x6200_vfoa_agc,

    x6200_vfob_ham_band = 6,
    x6200_vfob_freq,
    x6200_vfob_att,
    x6200_vfob_pre,
    x6200_vfob_mode,
    x6200_vfob_agc,

    x6200_sple_atue_trx = 12,
    x6200_vi_vm,
    x6200_rxvol,
    x6200_rfg_txpwr,

    x6200_atu_network = 17,

    x6200_ling_loutg_imicg_hmicg = 20,
    x6200_micsel_pttmode_chge_spmode_auxiqgen_sqlthr,
    x6200_voxg_voxag_voxdly_voxe,
    x6200_nrthr_nbw_nbthr_nre_nbe,
    x6200_dnfcnt_dnfwidth_dnfe,
    x6200_cmplevel_cmpe,
    // 26 - 1, 0
    x6200_agcknee_agcslope_agchang = 27,
    x6200_agctime,
    x6200_monilevel_fftdec_fftzoomcw,
    x6200_rxfilter,

    x6200_reg_32 = 32,
    x6200_ks_km_kimb_cwtone_cwvol_cwtrain,
    x6200_qsktime_kr,
    x6200_rxeq,
    x6200_rxeqwfm,
    x6200_miceq,

    x6200_biasdrive_biasfinal = 41,
    x6200_rit,
    x6200_xit,
    x6200_filter_ssb,
    x6200_filter_ssb_2,
    x6200_filter_cw,
    x6200_filter_am,
    x6200_filter_nfm,
    x6200_filter_wfm,

    x6200_last = 53
} x6200_cmd_enum_t;

/* Regs x6200_vfoa... x6200_vfob... */

typedef enum
{
    x6200_agc_off = 0,
    x6200_agc_slow = 1,
    x6200_agc_fast = 2,
    x6200_agc_auto = 3
} x6200_agc_t;

typedef enum
{
    x6200_att_off = 0,
    x6200_att_on = 1
} x6200_att_t;

typedef enum
{
    x6200_mode_lsb = 0,
    x6200_mode_lsb_dig = 1,
    x6200_mode_usb = 2,
    x6200_mode_usb_dig = 3,
    x6200_mode_cw = 4,
    x6200_mode_cwr = 5,
    x6200_mode_am = 6,
    x6200_mode_sam = 7,
    x6200_mode_nfm = 8,
    x6200_mode_wfm = 9,
} x6200_mode_t;

typedef enum
{
    x6200_pre_off = 0,
    x6200_pre_on = 1
} x6200_pre_t;

/* Reg x6200_sple_atue_trx */

enum
{
    x6200_sple = 0x00002,
    x6200_voice_rec = 0x00008,
    x6200_swrscan_trx = 0x00010,
    x6200_tune_flag = 0x00020,
    x6200_atue = 0x01000,
    x6200_atu_tune = 0x02000,
    x6200_modem_trx = 0x04000,
    x6200_calibration = 0x08000,
    x6200_power_off = 0x10000,
    x6200_iptt = 0x40000,  // ?
    x6200_calibration_trx = 0x100000,
    x6200_bias_drive_off = 0x200000,
    x6200_bias_final_off = 0x400000,
};

typedef enum
{
    x6200_key_manual = 0,
    x6200_key_auto_left = 1,
    x6200_key_auto_right = 2
} x6200_key_mode_t;

typedef enum
{
    x6200_iambic_a = 0,
    x6200_iambic_b = 1
} x6200_iambic_mode_t;

typedef enum {
    x6200_mic_builtin = 0,
    x6200_mic_handle,
    x6200_mic_auto
} x6200_mic_sel_t;

typedef enum {
    x6200_comp_off = 0,
    x6200_comp_1_2,
    x6200_comp_1_4,
    x6200_comp_1_8
} x6200_comp_level_t;

typedef enum {
    x6200_dnf_off = 0,
    x6200_dnf_manual,
    x6200_dnf_auto,
} x6200_dnf_mode_t;

/* Functions */

AETHER_X6200CTRL_API bool x6200_control_init();
AETHER_X6200CTRL_API bool x6200_control_cmd(x6200_cmd_enum_t cmd, uint32_t arg);
AETHER_X6200CTRL_API bool x6200_control_host_cmd(uint16_t data);
AETHER_X6200CTRL_API void x6200_control_idle();
AETHER_X6200CTRL_API bool x6200_control_set_band(uint32_t freq);
AETHER_X6200CTRL_API uint32_t x6200_control_get(x6200_cmd_enum_t cmd);
AETHER_X6200CTRL_API char* x6200_control_get_fw_version();
