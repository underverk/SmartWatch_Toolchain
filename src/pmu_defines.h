#ifndef _PMU_DEFINES_H_
#define _PMU_DEFINES_H_

#define PMU_ADDRESS             0x90
#define CHGSTATUS               0x01

#define CH_ACTIVE_MSK           0x08

#define CHGCONFIG0              0x02

#define VSYS_4_4V               0x40
#define VSYS_5V                 0x80
#define ACIC_100mA_DPPM_ENABLE  0x00
#define ACIC_500mA_DPPM_ENABLE  0x10
#define ACIC_500mA_DPPM_DISABLE 0x20
#define ACIC_USB_SUSPEND        0x20
#define TH_LOOP                 0x08
#define DYN_TMR                 0x04
#define TERM_EN                 0x02
#define CH_EN                   0x01

#define CHGCONFIG1              0x03
#define I_PRE_05                0x00
#define I_PRE_10                0x40
#define I_PRE_15                0x80
#define I_PRE_20                0xC0

#define DEFDCDC                 0x07
#define DCDC1_DEFAULT           0x29
#define DCDC_DISCH              0x40
#define HOLD_DCDC1              0x80

#define ISET_25                 0x00
#define ISET_50                 0x10
#define ISET_75                 0x20
#define ISET_100                0x30

#define I_TERM_05               0x00
#define I_TERM_10               0x04
#define I_TERM_15               0x08
#define I_TERM_20               0x0C

#define CHGCONFIG2 0x04
#define SFTY_TMR_4h 0x0
#define SFTY_TMR_5h 0x40
#define SFTY_TMR_6h 0x80
#define SFTY_TMR_8h 0xC0

#define PRE_TMR_30m 0x0
#define PRE_TMR_60m 0x20

#define NTC_100k 0x0
#define NTC_10k 0x8

#define V_DPPM_VBAT_100mV 0x0
#define V_DPPM_4_3_V 0x04

#define VBAT_COMP_ENABLE 0x02
#define VBAT_COMP_DISABLE 0x00

#endif