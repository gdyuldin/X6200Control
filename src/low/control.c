/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Aether Xiegu X6100 Control
 *
 *  Copyright (c) 2022 Belousov Oleg aka R1CBU
 *  Copyright (c) 2022 Rui Oliveira aka CT7ALW
 */

#include "aether_radio/x6100_control/low/control.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define REFRASH_TIMEOUT (1 * 1000)

typedef struct __attribute__((__packed__))
{
    uint16_t addr;
    uint32_t arg;
} cmd_struct_t;

typedef struct __attribute__((__packed__))
{
    uint16_t addr;
    uint32_t arg[x6100_last + 1];
} all_cmd_struct_t;

static int i2c_fd = -1;
static int i2c_addr = 0x72;
static all_cmd_struct_t all_cmd;
static uint8_t cur_band = 0;

static char base_info[129];
static char calibration_data[507];
static char *ver1 = base_info + 1;
static char *ver2 = base_info + 0x21;
static char *base_fw_version = base_info + 0x41;
static char *base_fw_date = base_info + 0x61;

static bool i2c_open()
{
    i2c_fd = open("/dev/i2c-0", O_RDWR);
    if (i2c_fd < 0) {
        perror("Can't open i2c");
        return false;
    }
    return true;
}

static void i2c_close()
{
    if (i2c_fd < 0) {
        printf("Can't close i2c, not opened\n");
        return;
    }
    if (close(i2c_fd) < 0) {
        perror("Can't close i2c");
    }
    i2c_fd = -1;
}

static bool send_regs(void *regs, size_t size)
{
    if (i2c_fd < 0) {
        printf("Can't write to i2c, not opened\n");
        return false;
    }
    struct i2c_msg messages[] = {
        {
            .addr = i2c_addr,
            .flags = 0,
            .buf = regs,
            .len = size,
        }
    };
    struct i2c_rdwr_ioctl_data packets = {
        .msgs = messages,
        .nmsgs = 1,
    };

    if(ioctl(i2c_fd, I2C_RDWR, &packets) < 0) {
        perror("Can't write to i2c");
        return false;
    }
    return true;
}

static bool get_regs(uint16_t reg, void *buf, uint16_t cnt) {
    if (i2c_fd < 0) {
        printf("Can't read from i2c, not opened");
        return false;
    }
    reg = (reg & 0xFF) << 8 | (reg >> 8);
    struct i2c_msg messages[] = {
        {
            .addr  = i2c_addr,
            .flags = 0,
            .len = 2,
            .buf = (char*)&reg,
        },
        {
            .addr  = i2c_addr,
            .flags = I2C_M_RD,
            .len   = cnt,
            .buf   = buf,
        }
    };

    struct i2c_rdwr_ioctl_data packets = {
        .msgs      = messages,
        .nmsgs     = 2,
    };

    if(ioctl(i2c_fd, I2C_RDWR, &packets) < 0) {
        perror("Can't read from i2c");
        return false;
    }
    return true;
}

bool x6100_control_init()
{
    if(!i2c_open()) {
        return false;
    }
    memset(&all_cmd, 0, sizeof(all_cmd));

    uint8_t val;
    while (true) {
        if (!get_regs(0x2000, &val, 1)) {
            printf("Can't connect BASE, retry after 1s\n");
            usleep(1000000);
        } else if (val == 0) {
            printf("BASE is not ready, retry after 1s\n");
            usleep(1000000);
        } else {
            break;
        }
    }

    if (!get_regs(0, base_info, sizeof(base_info))) {
        printf("Can't read BASE info\n");
        return false;
    }
    printf("BASE ver1: %s\n", ver1);  // FPGA
    printf("BASE ver2: %s\n", ver2);  // HW?
    printf("BASE fw version: %s\n", base_fw_version);
    printf("BASE fw date: %s\n", base_fw_date);

    if (!get_regs(0x100, calibration_data, sizeof(calibration_data))) {
        printf("Can't read BASE calibration data\n");
        return false;
    }

    // // Copy from captures OEM
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_freq, 0x00d81080);
    // x6100_control_cmd(x6100_vfoa_mode, 0x00000006);
    // x6100_control_cmd(x6100_vfoa_agc, 0x00000003);
    // x6100_control_cmd(x6100_vfoa_pre, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_freq, 0x006b9310);
    // x6100_control_cmd(x6100_vfob_mode, 0x00000000);
    // x6100_control_cmd(x6100_vfob_agc, 0x00000003);
    // x6100_control_cmd(x6100_vfob_pre, 0x00000000);
    // x6100_control_cmd(x6100_vfob_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000001);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_filter_ssb, 0x00960b22);
    // x6100_control_cmd(x6100_filter_cw, 0xff0600fa);
    // x6100_control_cmd(x6100_filter_am, 0xee6c1194);
    // x6100_control_cmd(x6100_filter_nfm, 0xe2b41d4c);
    // x6100_control_cmd(x6100_filter_wfm, 0xe0c01f40);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_freq, 0x00d81080);
    // x6100_control_cmd(x6100_vfoa_mode, 0x00000006);
    // x6100_control_cmd(x6100_vfoa_agc, 0x00000003);
    // x6100_control_cmd(x6100_vfoa_pre, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_freq, 0x006b9310);
    // x6100_control_cmd(x6100_vfob_mode, 0x00000000);
    // x6100_control_cmd(x6100_vfob_agc, 0x00000003);
    // x6100_control_cmd(x6100_vfob_pre, 0x00000000);
    // x6100_control_cmd(x6100_vfob_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000001);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_filter_ssb, 0x00960b22);
    // x6100_control_cmd(x6100_filter_cw, 0xff0600fa);
    // x6100_control_cmd(x6100_filter_am, 0xee6c1194);
    // x6100_control_cmd(x6100_filter_nfm, 0xe2b41d4c);
    // x6100_control_cmd(x6100_filter_wfm, 0xe0c01f40);

    // Write calib data
    // iicWrite(0xf000,1,(char *)&this->calibration_data_maybe,0x1fb);
    // char calib_data_req[sizeof(calibration_data) + 2];
    // calib_data_req[0] = 0xf0;
    // calib_data_req[1] = 0x00;
    // memcpy(calib_data_req + 2, calibration_data, sizeof(calibration_data));
    // send_regs(calib_data_req, sizeof(calib_data_req));

    // Send host_cmd
    if (!x6100_control_host_cmd(0x8003)) {
        printf("Can't send host_cmd 0x8003\n");
        return false;
    }
    if (!x6100_control_host_cmd(0x8002)) {
        printf("Can't send host_cmd 0x8002\n");
        return false;
    }

    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_freq, 0x00d81080);
    // x6100_control_cmd(x6100_vfoa_mode, 0x00000006);
    // x6100_control_cmd(x6100_vfoa_agc, 0x00000003);
    // x6100_control_cmd(x6100_atu_network, 0x00000000);
    // x6100_control_cmd(x6100_rxfilter, 0xee6c1194);
    // x6100_control_cmd(x6100_filter_am, 0xee6c1194);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_freq, 0x00d81080);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_agc, 0x00000003);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_pre, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfoa_mode, 0x00000006);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_freq, 0x006b9310);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_agc, 0x00000003);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_att, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_pre, 0x00000000);
    // x6100_control_cmd(x6100_vi_vm, 0x00000000);
    // x6100_control_cmd(x6100_vfob_mode, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00000000);
    // x6100_control_cmd(x6100_sple_atue_trx, 0x00001000);
    // x6100_control_cmd(x6100_rxvol, 0x00000030);
    // x6100_control_cmd(x6100_rfg_txpwr, 0x00000064);
    // x6100_control_cmd(x6100_rfg_txpwr, 0x00003264);
    // x6100_control_cmd(x6100_ling_loutg_imicg_hmicg, 0x0000000a);
    // x6100_control_cmd(x6100_ling_loutg_imicg_hmicg, 0x00000a0a);
    // x6100_control_cmd(x6100_ling_loutg_imicg_hmicg, 0x001e0a0a);
    // x6100_control_cmd(x6100_ling_loutg_imicg_hmicg, 0x0f1e0a0a);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000002);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000002);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000012);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000032);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000032);
    // x6100_control_cmd(x6100_micsel_pttmode_chge_spmode_auxiqgen_sqlthr, 0x00000032);
    // x6100_control_cmd(x6100_voxg_voxag_voxdly_voxe, 0x00000032);
    // x6100_control_cmd(x6100_voxg_voxag_voxdly_voxe, 0x00000032);
    // x6100_control_cmd(x6100_voxg_voxag_voxdly_voxe, 0x00190032);
    // x6100_control_cmd(x6100_voxg_voxag_voxdly_voxe, 0x00190032);
    // x6100_control_cmd(x6100_nrthr_nbw_nbthr_nre_nbe, 0x00000032);
    // x6100_control_cmd(x6100_nrthr_nbw_nbthr_nre_nbe, 0x00000a32);
    // x6100_control_cmd(x6100_nrthr_nbw_nbthr_nre_nbe, 0x000a0a32);
    // x6100_control_cmd(x6100_nrthr_nbw_nbthr_nre_nbe, 0x000a0a32);
    // x6100_control_cmd(x6100_nrthr_nbw_nbthr_nre_nbe, 0x000a0a32);
    // x6100_control_cmd(x6100_dnfcnt_dnfwidth_dnfe, 0x000003e8);
    // x6100_control_cmd(x6100_dnfcnt_dnfwidth_dnfe, 0x000323e8);
    // x6100_control_cmd(x6100_dnfcnt_dnfwidth_dnfe, 0x000323e8);
    // x6100_control_cmd(x6100_cmplevel_cmpe, 0x00000000);
    // x6100_control_cmd(x6100_cmplevel_cmpe, 0x00000000);
    // x6100_control_cmd(x6100_agcknee_agcslope_agchang, 0x000000e2);
    // x6100_control_cmd(x6100_agcknee_agcslope_agchang, 0x000004e2);
    // x6100_control_cmd(x6100_agcknee_agcslope_agchang, 0x000004e2);
    // x6100_control_cmd(x6100_monilevel_fftdec_fftzoomcw, 0x0000001e);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x0000000f);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x0000010f);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x0000050f);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x0025850f);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x1625850f);
    // x6100_control_cmd(x6100_ks_km_kimb_cwtone_cwvol_cwtrain, 0x3625850f);
    // x6100_control_cmd(x6100_reg_32, 0x00000000);
    // x6100_control_cmd(x6100_reg_32, 0x0000000a);
    // x6100_control_cmd(x6100_qsktime_kr, 0x00000064);
    // x6100_control_cmd(x6100_qsktime_kr, 0x001e0064);
    // x6100_control_cmd(x6100_biasdrive_biasfinal, 0x00000d20);
    // x6100_control_cmd(x6100_biasdrive_biasfinal, 0x0eec0d20);
    // x6100_control_cmd(x6100_rit, 0x00000000);
    // x6100_control_cmd(x6100_xit, 0x00000000);
    // x6100_control_cmd(x6100_monilevel_fftdec_fftzoomcw, 0x0000021e);
    // x6100_control_cmd(x6100_monilevel_fftdec_fftzoomcw, 0x0000121e);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_miceq, 0x00000000);


    // 0bb8 - 3000, 012c - 300
    all_cmd.arg[x6100_filter_ssb] = 0xb80b2c01;
    all_cmd.arg[x6100_filter_ssb_2] = 0xb80b2c01;
    // 0320 - 800
    all_cmd.arg[x6100_filter_cw] = 0x2003fce0;
    // 1707 - 6000
    all_cmd.arg[x6100_filter_am] = 0x7017e890;
    // 2328 - 9000
    all_cmd.arg[x6100_filter_nfm] = 0x2823dcd8;
    // 3e80 - 16000
    all_cmd.arg[x6100_filter_wfm] = 0x803ec180;

    // // "[INFO] Baseband is ready"
    // // "160m 1800000  - 2000000"
    // // "80m  3500000  - 4000000"
    // // "60m  5332000  - 5405000"
    // // "40m  7000000  - 7300000"
    // // "30m  10100000 - 10150000"
    // // "20m  14000000 - 14350000"
    // // "17m  18068000 - 18168000"
    // // "15m  21000000 - 21450000"
    // // "12m  24890000 - 24990000"
    // // "10m  28000000 - 29700000"
    // // "6m   50000000 - 54000000"
    // // "FullBandTx: 0"
    // // "Bias drive: 3360"
    // // "Bias final: 3820"
    // // "S-Meter offset: 30"
    // // "[DEBUG] Update Front VFO Frequency 7074000"
    // // "[DEBUG] Write host cmd 8003"
    // // "[DEBUG] Write host cmd 8002"
    // // "[DEBUG] Get DBus connection 0x8b8038"



    // // x6100_rx_filters - same as for other filters
    // // host_cmd?

    // // save calibrate params
    // // 0x8005

    // // MainWidget::slot_atuTune
    // // atu start
    // // 0x8004

    // // slot_basebandStateChanged (param == 1)
    // // After base band state changed
    // // 0x8003
    // // 0x8002

    // // slot_receiveCommand

    // x6100_control_cmd(x6100_miceq, 0x00000000);
    // x6100_control_cmd(x6100_rxeqwfm, 0x00000000);
    // x6100_control_cmd(x6100_rxeq, 0x00000000);
    // x6100_control_cmd(x6100_cmplevel_cmpe, 0x00000000);
    // x6100_control_cmd(x6100_reg_32, 0x0000000a);
    // x6100_control_cmd(x6100_voxg_voxag_voxdly_voxe, 0x00190032);

    // x6100_control_cmd(x6100_vi_vm, 0);
    // x6100_control_cmd(x6100_vi_vm, 1);
    // x6100_control_cmd(x6100_vi_vm, 0);
    // x6100_control_cmd(x6100_filter_ssb, 0xb80b2c01);
    // x6100_control_cmd(x6100_filter_ssb_2, 0xb80b2c01);
    // x6100_control_cmd(x6100_filter_cw, 0x2003fce0);
    // x6100_control_cmd(x6100_filter_am, 0xee6c1194);
    // x6100_control_cmd(x6100_filter_nfm, 0xe2b41d4c);
    // x6100_control_cmd(x6100_filter_wfm, 0xe0c01f40);


    all_cmd.arg[x6100_vfoa_ham_band] = 0;
    all_cmd.arg[x6100_vfoa_freq] = 14074000;
    all_cmd.arg[x6100_vfoa_mode] = x6100_mode_usb;
    all_cmd.arg[x6100_vfoa_agc] = x6100_agc_auto;

    all_cmd.arg[x6100_vfob_ham_band] = 0;
    all_cmd.arg[x6100_vfob_freq] = 14074000;
    all_cmd.arg[x6100_vfob_mode] = x6100_mode_usb;
    all_cmd.arg[x6100_vfob_agc] = x6100_agc_auto;

    // all_cmd.arg[x6100_sple_atue_trx] = 0x00001000;

    // all_cmd.arg[x6100_reg_32] = 0x0000000a;
    all_cmd.arg[x6100_rxvol] = 0;
    all_cmd.arg[x6100_rfg_txpwr] = (10 << 8) | 64;

    // all_cmd.arg[x6100_agcknee_agcslope_agchang] = 0x000006C4;
    // all_cmd.arg[x6100_agctime] = 500;

    // all_cmd.arg[x6100_filter1_low] = (uint32_t) 50.0f;
    // all_cmd.arg[x6100_filter1_high] = (uint32_t) 2950.0;
    // all_cmd.arg[x6100_filter2_low] = (uint32_t) 50.0f;
    // all_cmd.arg[x6100_filter2_high] = (uint32_t) 2950.0f;

    // all_cmd.arg[x6100_pwrsync] = 2000000;
    // all_cmd.arg[x6100_last] = 0x100001;

    if (!send_regs(&all_cmd, sizeof(all_cmd))) {
        printf("Can't write data to BASE\n");
        return false;
    };


    return true;
}

bool x6100_control_host_cmd(uint16_t data)
{
    uint16_t command[2];
    uint16_t addr = 0xfffe;

    command[0] = (addr & 0xFF) << 8 | (addr >> 8);
    command[1] = data;

    return send_regs(&command, sizeof(command));
}

bool x6100_control_cmd(x6100_cmd_enum_t cmd, uint32_t arg)
{
    all_cmd.arg[cmd] = arg;

    cmd_struct_t command;
    uint16_t addr = cmd * 4;

    command.addr = (addr & 0xFF) << 8 | (addr >> 8);
    command.arg = arg;

    return send_regs(&command, sizeof(command));
}


uint32_t x6100_control_get(x6100_cmd_enum_t cmd)
{
    return all_cmd.arg[cmd];
}

char *x6100_control_get_fw_version()
{
    static char version[0x80] = "\0";
    if (get_regs(0, version, 0x80))
        return version;
    else
        return NULL;
}

void x6100_control_idle()
{
    if (!send_regs(&all_cmd, sizeof(all_cmd))) {
        i2c_close();
        usleep(1000);
        i2c_open();
        send_regs(&all_cmd, sizeof(all_cmd));
    }
}

static uint8_t band_index(int freq)
{
    if (freq < 1800000) {
        return 0;
    } else if (freq <= 2000000) {
        return 1;
    } else if (freq < 3500000) {
        return 2;
    } else if (freq <= 4000000) {
        return 3;
    } else if (freq < 5330500) {
        return 4;
    } else if (freq <= 5405000) {
        return 5;
    } else if (freq < 7000000) {
        return 6;
    } else if (freq <= 7300000) {
        return 7;
    } else if (freq < 10100000) {
        return 8;
    } else if (freq <= 10150000) {
        return 9;
    } else if (freq < 14000000) {
        return 10;
    } else if (freq <= 14350000) {
        return 11;
    } else if (freq < 18068000) {
        return 12;
    } else if (freq <= 18168000) {
        return 13;
    } else if (freq < 21000000) {
        return 14;
    } else if (freq <= 21450000) {
        return 15;
    } else if (freq < 24890000) {
        return 16;
    } else if (freq <= 24990000) {
        return 17;
    } else if (freq < 28000000) {
        return 18;
    } else if (freq <= 29700000) {
        return 19;
    } else if (freq < 50000000) {
        return 20;
    } else if (freq <= 54000000) {
        return 21;
    } else {
        return 22;
    }
}

bool x6100_control_set_band(uint32_t freq)
{
    uint8_t band = band_index(freq);

    if (band != cur_band)
    {
        cur_band = band;

        // x6100_control_cmd(x6100_vi_vm, cur_band << 8);
        return true;
    }
    return false;
}
