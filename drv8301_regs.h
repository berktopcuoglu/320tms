/*
 * drv8301_regs.h
 *
 *  Created on: 4 Aug 2025
 *      Author: berktopcuoglu
 */

#ifndef DRV8301_REGS_H_
#define DRV8301_REGS_H_

// DRV8301 register adresleri

#define DRV8301_Status_Register_1       0x00
#define DRV8301_Status_Register_2       0x01
#define DRV8301_Control_Register_1      0x02
#define DRV8301_Control_Register_2      0x03

//Table 9. Status Register 1 (Address: 0x00) (All Default Values are Zero)
typedef union{
    uint16_t all;
    struct {
        uint16_t FAULT      :1;
        uint16_t GVDD_UV    :1;
        uint16_t PVDD_UV    :1;
        uint16_t OTSD       :1;
        uint16_t OTW        :1;
        uint16_t FETHA_OC   :1;
        uint16_t FETLA_OC   :1;
        uint16_t FETHB_OC   :1;
        uint16_t FETLB_OC   :1;
        uint16_t FETHC_OC   :1;
        uint16_t FETLC_OC   :1;
    }bit;
}Status_Register_1;

//Table 10. Status Register 2 (Address: 0x01) (All Default Values are Zero)
typedef union{
    uint16_t all;
    struct{
        uint16_t RESERVED_0   :3;
        uint16_t GVDD_OV    :1;
        uint16_t RESERVED_1   :3;
        uint16_t Device_ID  :4;
    }bit;
}Status_Register_2;

//Table 11. Control Register 1 for Gate Driver Control (Address: 0x02)
typedef union{
    uint16_t all;
    struct{
        uint16_t GATE_CURRENT   :2;
        uint16_t GATE_RESET     :1;
        uint16_t PWM_MODE       :1;
        uint16_t OCP_MODE       :2;
        uint16_t OC_ADJ_SET     :5;
    }bit;

}Control_Register_1;

//Table 12. Control Register 2 for Current Shunt Amplifiers and Misc Control (Address: 0x03)(
typedef union{
    uint16_t all;
    struct{
        uint16_t OCTW_MODE      :2;
        uint16_t GAIN           :2;
        uint16_t DC_CAL_CH1     :1;
        uint16_t DC_CAL_CH2     :1;
        uint16_t OC_TOFF        :1;
        uint16_t OC_ADJ_SET     :4;
    }bit;

}Control_Register_2;

typedef enum{
    GATE_PEAK_1_7_A = 0,
    GATE_PEAK_0_7_A,
    GATE_PEAK_0_25A
}DRV8301_GATE_CURRENT;

typedef enum{
    DRV8301_NORMAL_MODE = 0,
    DRV8301_RESET
}DRV8301_GATE_RESET;

typedef enum{
    PWM_6 = 0,
    PWM_3
}DRV8301_PWM_MODE;

typedef enum{
    DRV8301_OC_CURRENT_LIMIT = 0,
    DRV8301_OC_LATCH_SHUTDOWN,
    DRV8301_OC_REPORT_ONLY,
    DRV8301_OC_DISABLE,
}DRV8301_OCP_Mode;

typedef enum{
    DRV8301_nOCTW_Enable = 0,
    DRV8301_ONLY_OT,
    DRV8301_ONLY_OC
}DRV8301_OCTW_MODE;

typedef enum{
    DRV8301_GAIN_10 = 0,
    DRV8301_GAIN_20,
    DRV8301_GAIN_40,
    DRV8301_GAIN_80
}DRV8301_GAIN;

typedef enum{
    DRV8301_DC_CAL_CH1_CONNECT =0,
    DRV8301_DC_CAL_CH1_DISCONNECT
}DRV8301_DC_CAL_CH1;

typedef enum{
    DRV8301_DC_CAL_CH2_CONNECT =0,
    DRV8301_DC_CAL_CH2_DISCONNECT
}DRV8301_DC_CAL_CH2;

typedef enum{
    DRV8301_CYCLE_BY_CYLE =0 ,
    DRV8301_OFF_TIME_CONTROL
}DRV8301_OC_TOFF;

typedef enum{
    DRV8301_DEADTIME_50ns = 0,
    DRV8301_DEADTIME_100ns,
    DRV8301_DEADTIME_200ns,
    DRV8301_DEADTIME_400ns
}DRV8301_DEADTIME;

#endif /* DRV8301_REGS_H_ */
