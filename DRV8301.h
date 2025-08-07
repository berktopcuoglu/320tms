/*
 * DRV8301.h
 *
 *  Created on: 4 Aug 2025
 *      Author: berktopcuoglu
 */

#ifndef DRV8301_H_
#define DRV8301_H_

#include "driverlib.h"
#include "device.h"
#include "drv8301_regs.h"

typedef struct{
    Status_Register_1    status_1;
    Status_Register_2    status_2;
    Control_Register_1   control_1;
    Control_Register_2   control_2;
}DRV8301_Registers;

typedef struct{
    uint32_t spi_base;
    uint32_t nSCS_gpio;
    uint32_t nFAULT_gpio;
    uint32_t EN_GATE_gpio;
    uint32_t nOCTW_gpio;
    DRV8301_Registers drv8301_register;
}DRV8301_Handle;

void DRV8301_init(DRV8301_Handle *drv8301, uint32_t spi_base, uint32_t nSCS_gpio, uint32_t nFAULT_gpio, uint32_t EN_GATE_gpio, uint32_t nOCTW_gpio );
void DRV8301_configure_1(DRV8301_Handle *drv8301,DRV8301_GATE_CURRENT gate_current, DRV8301_GATE_RESET gate_reset, DRV8301_PWM_MODE pwm_mode, DRV8301_OCP_Mode ocp_mode);
void DRV8301_configure_2(DRV8301_Handle *drv8301, DRV8301_OCTW_MODE octw_mode, DRV8301_GAIN gain, DRV8301_DC_CAL_CH1 dc_cal_ch1, DRV8301_DC_CAL_CH2 dc_cal_ch2, DRV8301_OC_TOFF oc_toff);

uint16_t DRV8301_read(DRV8301_Handle *drv8301, uint16_t addr);
void DRV8301_write(DRV8301_Handle *drv8301, uint16_t addr, uint16_t data);
void DRV8301_updateStatus(DRV8301_Handle *drv8301);
bool DRV8301_isFaultActive(DRV8301_Handle *drv8301);
void DRV8301_clearFault(DRV8301_Handle *drv8301);

#endif /* DRV8301_H_ */
