#include <nrf52.h>
#include "pinetime_board.h"
#include "i2c.h"
#include <cstring>
#include <semphr.h>

SemaphoreHandle_t i2c_mutex = nullptr;

static constexpr uint32_t hw_freezed_delay{ 161000 };

void wire_begin(void) {

    uint8_t _uc_pinSDA = g_ADigitalPinMap[TWI_SDA];
    uint8_t _uc_pinSCL = g_ADigitalPinMap[TWI_SCL];

    if (i2c_mutex == nullptr) {
        i2c_mutex = xSemaphoreCreateBinary();
    }

    NRF_GPIO->PIN_CNF[_uc_pinSCL] =
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos) |
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    NRF_GPIO->PIN_CNF[_uc_pinSDA] =
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos) |
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);


    NRF_TWIM1->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400;

    NRF_TWIM1->PSEL.SCL = _uc_pinSCL;
    NRF_TWIM1->PSEL.SDA = _uc_pinSDA;
    NRF_TWIM1->EVENTS_LASTRX = 0;
    NRF_TWIM1->EVENTS_STOPPED = 0;
    NRF_TWIM1->EVENTS_LASTTX = 0;
    NRF_TWIM1->EVENTS_ERROR = 0;
    NRF_TWIM1->EVENTS_RXSTARTED = 0;
    NRF_TWIM1->EVENTS_SUSPENDED = 0;
    NRF_TWIM1->EVENTS_TXSTARTED = 0;

    NRF_TWIM1->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);

    // IRQ
    //NVIC_ClearPendingIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
    //NVIC_SetPriority(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, 2);
    //NVIC_EnableIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
  
    xSemaphoreGive(i2c_mutex);
}

void i2c_sleep() {
    NRF_TWIM1->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
}

void i2c_wakeup() {
    NRF_TWIM1->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
}

void fix_hw_freezed() {
    uint32_t twi_state = NRF_TWIM1->ENABLE;
    i2c_sleep();

    uint8_t _uc_pinSDA = g_ADigitalPinMap[TWI_SDA];
    uint8_t _uc_pinSCL = g_ADigitalPinMap[TWI_SCL];

    NRF_GPIO->PIN_CNF[_uc_pinSCL] =
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos) |
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    NRF_GPIO->PIN_CNF[_uc_pinSDA] =
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos) |
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    NRF_TWIM1->ENABLE = twi_state;
}

uint8_t i2c_read(uint8_t deviceAddress, uint8_t* buffer, size_t size, bool stop) {
    NRF_TWIM1->ADDRESS = deviceAddress;
    NRF_TWIM1->TASKS_RESUME = 0x1UL;
    NRF_TWIM1->RXD.PTR = (uint32_t)buffer;
    NRF_TWIM1->RXD.MAXCNT = size;

    NRF_TWIM1->TASKS_STARTRX = 1;

    while (!NRF_TWIM1->EVENTS_RXSTARTED && !NRF_TWIM1->EVENTS_ERROR)
        ;
    NRF_TWIM1->EVENTS_RXSTARTED = 0x0UL;

    uint32_t txStartedCycleCount = DWT->CYCCNT;
    uint32_t currentCycleCount;
    while (!NRF_TWIM1->EVENTS_LASTRX && !NRF_TWIM1->EVENTS_ERROR) {
        currentCycleCount = DWT->CYCCNT;
        if ((currentCycleCount - txStartedCycleCount) > hw_freezed_delay) {
            fix_hw_freezed();
            return 1;
        }
    }
    NRF_TWIM1->EVENTS_LASTRX = 0x0UL;

    if (stop || NRF_TWIM1->EVENTS_ERROR) {
        NRF_TWIM1->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM1->EVENTS_STOPPED)
            ;
        NRF_TWIM1->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM1->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM1->EVENTS_SUSPENDED)
            ;
        NRF_TWIM1->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM1->EVENTS_ERROR) {
        NRF_TWIM1->EVENTS_ERROR = 0x0UL;
    }
    return 0;
}


uint8_t i2c_write(uint8_t deviceAddress, const uint8_t* data, size_t size, bool stop) {
    NRF_TWIM1->ADDRESS = deviceAddress;
    NRF_TWIM1->TASKS_RESUME = 0x1UL;
    NRF_TWIM1->TXD.PTR = (uint32_t)data;
    NRF_TWIM1->TXD.MAXCNT = size;

    NRF_TWIM1->TASKS_STARTTX = 1;

    while (!NRF_TWIM1->EVENTS_TXSTARTED && !NRF_TWIM1->EVENTS_ERROR)
        ;
    NRF_TWIM1->EVENTS_TXSTARTED = 0x0UL;

    uint32_t txStartedCycleCount = DWT->CYCCNT;
    uint32_t currentCycleCount;
    while (!NRF_TWIM1->EVENTS_LASTTX && !NRF_TWIM1->EVENTS_ERROR) {
        currentCycleCount = DWT->CYCCNT;
        if ((currentCycleCount - txStartedCycleCount) > hw_freezed_delay) {
            fix_hw_freezed();
            return 1;
        }
    }
    NRF_TWIM1->EVENTS_LASTTX = 0x0UL;

    if (stop || NRF_TWIM1->EVENTS_ERROR) {
        NRF_TWIM1->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM1->EVENTS_STOPPED)
            ;
        NRF_TWIM1->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM1->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM1->EVENTS_SUSPENDED)
            ;
        NRF_TWIM1->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM1->EVENTS_ERROR) {
        NRF_TWIM1->EVENTS_ERROR = 0x0UL;
        uint32_t error = NRF_TWIM1->ERRORSRC;
        NRF_TWIM1->ERRORSRC = error;
    }

    return 0;
}

void init_i2c() {
    wire_begin();
    //Wire.begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    i2c_wakeup();
    uint8_t ret = i2c_write(addr, &reg_addr, 1, false);
    ret = i2c_read(addr, reg_data, length, true);

    i2c_sleep();
    xSemaphoreGive(i2c_mutex);
    return ret;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {
    //static constexpr uint8_t maxDataSize {16};
    //static constexpr uint8_t registerSize {1};

    /*if(length > maxDataSize) {
        return 1;
    }*/

    //uint8_t internalBuffer[maxDataSize + registerSize];

    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    i2c_wakeup();
    //internalBuffer[0] = reg_addr;
    //std::memcpy(internalBuffer + 1, reg_data, length);
    uint8_t ret = i2c_write(addr, &reg_addr, 1, false);
    ret = i2c_write(addr, reg_data, length, true);
    i2c_sleep();
    xSemaphoreGive(i2c_mutex);
    return ret;
}
