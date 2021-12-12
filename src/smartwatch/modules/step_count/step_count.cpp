/*
    A library for Grove - Step Counter(BMA421)
    Copyright (c) 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :
    The MIT License (MIT)
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "step_count.h"
#include "i2c.h"
#include "bma421_config.h"
#include "pinetime_board.h"


static int8_t bma_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
    user_i2c_read(BMA421_TWI_ADDR, reg_addr, reg_data, length);
    return 0;
}

static int8_t bma_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
    user_i2c_write(BMA421_TWI_ADDR, reg_addr, reg_data, length);
    return 0;
}

static void bma_delay_ms(uint32_t period_us, void *intf_ptr) {
    delay_ns(period_us);
}

void StepCount::initialize() {

    bma.intf = BMA4_I2C_INTF;
    bma.bus_read = bma_i2c_read;
    bma.bus_write = bma_i2c_write;
    bma.variant = BMA42X_VARIANT;
    bma.intf_ptr = this;
    bma.delay_us = bma_delay_ms;
    bma.read_write_len = 16;

    /* Sensor initialization */
    int8_t rslt;

    /* Soft reset */
    //bma4_soft_reset(&bma);
    //delay_ns(50);

    rslt = bma421_init(&bma);
    if ( rslt != BMA4_OK ) {
        return;
    }

    delay(50);

    bma4_set_advance_power_save(BMA4_DISABLE, &bma);

    /* Upload the configuration file to enable the features of the sensor. */
    bma421_write_config_file(&bma);
    delay(50);

    /* Enable the accelerometer */
    bma4_set_accel_enable(BMA4_ENABLE, &bma);
    delay(50);

    /* Accelerometer Configuration Setting */
    accel_conf.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    accel_conf.range = BMA4_ACCEL_RANGE_2G;
    accel_conf.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    accel_conf.perf_mode = BMA4_CIC_AVG_MODE;

    /* Set the accel configurations */
    bma4_set_accel_config(&accel_conf, &bma);

    bma421_step_detector_enable(BMA4_ENABLE, &bma);

    /* Enable step counter */
    bma421_feature_enable(BMA421_STEP_CNTR | BMA421_STEP_ACT, BMA4_ENABLE, &bma);

    //bma421_reset_step_counter(&bma);

    //bma4_set_interrupt_mode(BMA4_LATCH_MODE, &bma);

    /* Sets the electrical behaviour of interrupt */
    /*pinConfig.edge_ctrl = BMA4_LEVEL_TRIGGER;
    pinConfig.lvl = BMA4_ACTIVE_LOW;
    pinConfig.od = BMA4_PUSH_PULL;
    pinConfig.output_en = BMA4_OUTPUT_ENABLE;
    pinConfig.input_en = BMA4_INPUT_DISABLE;
    bma4_set_int_pin_config(&pinConfig, BMA4_INTR1_MAP, &bma);*/

    /* Interrupt Mapping */
    //bma421_map_interrupt(BMA4_INTR1_MAP, BMA421_STEP_CNTR_INT , BMA4_ENABLE, &bma);

    //bma421_step_counter_set_watermark(1, &bma);

    bma4_set_advance_power_save(BMA4_ENABLE, &bma);
}


void StepCount::ResetSteps() {
    bma421_reset_step_counter(&bma);
}

void StepCount::getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    struct bma4_accel sens_data;

    bma4_read_accel_xyz(&sens_data, &bma);

    /**x = (float)sens_data.x * devRange / 32768;
    *y = (float)sens_data.y * devRange / 32768;
    *z = (float)sens_data.z * devRange / 32768;*/

    *x = (int16_t)sens_data.y / 0x10;
    *y = -(int16_t)sens_data.x / 0x10;
    *z = (int16_t)sens_data.z / 0x10;
}

uint32_t StepCount::getStepCounterOutput(void) {
    uint32_t step = 0;

    bma421_step_counter_output(&step, &bma);

    return step;
}
