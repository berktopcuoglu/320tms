/*
 * drv8301.c
 *
 *  Created on: 4 Aug 2025
 *      Author: berktopcuoglu
 */

#include "drv8301.h"


// ---- Chip Select kontrolü ----
static inline void DRV8301_CS_Low(uint32_t nSCS){
    GPIO_writePin(nSCS, 0);
}
static inline void DRV8301_CS_High(uint32_t nSCS){
    GPIO_writePin(nSCS, 1);
}

// ---- DRV8301 komut çerçevesi oluşturma ----
static uint16_t DRV8301_build_cmd(uint16_t rw, uint16_t addr, uint16_t data)
{
    return ((rw & 0x1) << 15) | ((addr & 0xF) << 11) | (data & 0x3FF);
}

// ---- SPI transfer (16 bit) ----
static uint16_t SPI_transfer16(uint32_t spi_base ,uint16_t data)
{
    // Yaz (blocking)
    SPI_writeDataBlockingNonFIFO(spi_base, data);
//    while(!SPI_isBusy(spi_base));
    // Oku
    return SPI_readDataBlockingNonFIFO(spi_base);
}

void DRV8301_init(DRV8301_Handle *drv8301, uint32_t spi_base, uint32_t nSCS_gpio, uint32_t nFAULT_gpio, uint32_t EN_GATE_gpio, uint32_t nOCTW_gpio )
{
    drv8301 -> spi_base = spi_base;
    drv8301 -> nSCS_gpio = nSCS_gpio;
    drv8301 -> nFAULT_gpio = nFAULT_gpio;
    drv8301 -> EN_GATE_gpio = EN_GATE_gpio;
    drv8301 -> nOCTW_gpio = nOCTW_gpio;

    GPIO_setDirectionMode(nSCS_gpio, GPIO_DIR_MODE_OUT);
    DRV8301_CS_High(nSCS_gpio);

    GPIO_setDirectionMode(nFAULT_gpio, GPIO_DIR_MODE_IN);

    GPIO_setDirectionMode(EN_GATE_gpio, GPIO_DIR_MODE_OUT);
    GPIO_writePin(EN_GATE_gpio, 1); // DRV8301'i etkinleştirir

    GPIO_setDirectionMode(nOCTW_gpio, GPIO_DIR_MODE_IN);

}
// ---- DRV8301 register yaz ----
void DRV8301_write(DRV8301_Handle *drv8301, uint16_t addr, uint16_t data)
{
    DRV8301_CS_Low(drv8301->nSCS_gpio);
    SPI_transfer16(drv8301->spi_base, DRV8301_build_cmd(0, addr, data));
    DRV8301_CS_High(drv8301->nSCS_gpio);
}

// ---- DRV8301 register oku ----
uint16_t DRV8301_read(DRV8301_Handle *drv8301, uint16_t addr)
{
    uint16_t val;
    DRV8301_CS_Low(drv8301->nSCS_gpio);
    SPI_transfer16(drv8301->spi_base, DRV8301_build_cmd(1, addr, 0)); // okuma isteği
    DRV8301_CS_High(drv8301->nSCS_gpio);

    DRV8301_CS_Low(drv8301->nSCS_gpio);
    val = SPI_transfer16(drv8301->spi_base, 0x0000); // cevabı almak için dummy frame
    DRV8301_CS_High(drv8301->nSCS_gpio);
    return val & 0x3FF;
}

void DRV8301_configure_1(DRV8301_Handle *drv8301,DRV8301_GATE_CURRENT gate_current, DRV8301_GATE_RESET gate_reset, DRV8301_PWM_MODE pwm_mode, DRV8301_OCP_Mode ocp_mode)
{
    drv8301 -> drv8301_register.control_1.bit.GATE_CURRENT = gate_current;
    drv8301 -> drv8301_register.control_1.bit.GATE_RESET = gate_reset;
    drv8301 -> drv8301_register.control_1.bit.PWM_MODE =pwm_mode;
    drv8301 -> drv8301_register.control_1.bit.OCP_MODE = ocp_mode;

    DRV8301_write(drv8301, DRV8301_Control_Register_1, drv8301-> drv8301_register.control_1.all);
}

void DRV8301_configure_2(DRV8301_Handle *drv8301, DRV8301_OCTW_MODE octw_mode, DRV8301_GAIN gain, DRV8301_DC_CAL_CH1 dc_cal_ch1, DRV8301_DC_CAL_CH2 dc_cal_ch2, DRV8301_OC_TOFF oc_toff)
{
    drv8301 -> drv8301_register.control_2.bit.OCTW_MODE = octw_mode;
    drv8301 -> drv8301_register.control_2.bit.GAIN = gain;
    drv8301 -> drv8301_register.control_2.bit.DC_CAL_CH1 = dc_cal_ch1;
    drv8301 -> drv8301_register.control_2.bit.DC_CAL_CH2 = dc_cal_ch2;
    drv8301 -> drv8301_register.control_2.bit.OC_TOFF = oc_toff;
    DRV8301_write(drv8301, DRV8301_Control_Register_2, drv8301->drv8301_register.control_2.all);
}
void DRV8301_updateStatus(DRV8301_Handle *drv8301){
    drv8301 -> drv8301_register.status_1.all = DRV8301_read(drv8301, DRV8301_Status_Register_1);
    drv8301 -> drv8301_register.status_2.all = DRV8301_read(drv8301, DRV8301_Status_Register_2);
   drv8301 -> drv8301_register.control_1.all = DRV8301_read(drv8301, DRV8301_Control_Register_1);
    drv8301 -> drv8301_register.control_2.all = DRV8301_read(drv8301, DRV8301_Control_Register_2);
}

bool DRV8301_isFaultActive(DRV8301_Handle *drv8301){
    return GPIO_readPin(drv8301->nFAULT_gpio);
}

void DRV8301_clearFault(DRV8301_Handle *drv8301){
    drv8301-> drv8301_register.control_1.bit.GATE_RESET = 1;
    DRV8301_write(drv8301, DRV8301_Control_Register_1, drv8301->drv8301_register.control_1.bit.GATE_RESET);
    drv8301-> drv8301_register.control_1.bit.GATE_RESET = 0;
    DRV8301_write(drv8301, DRV8301_Control_Register_1, drv8301->drv8301_register.control_1.bit.GATE_RESET);
}

