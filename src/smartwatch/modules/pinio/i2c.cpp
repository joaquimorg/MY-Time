#include <nrf52.h>
#include "pinetime_board.h"
#include "i2c.h"

//#include "Wire.h"

static constexpr uint32_t HwFreezedDelay {161000};

volatile bool i2cReading = false;

void set_i2c_reading(bool state) {
    i2cReading = state;
}

bool get_i2cReading() {
    return i2cReading;
}

uint8_t _uc_pinSDA = g_ADigitalPinMap[TWI_SDA];
uint8_t _uc_pinSCL = g_ADigitalPinMap[TWI_SCL];
bool receiving;
bool transmissionBegun = false;
bool suspended;
RingBuffer rxBuffer;
RingBuffer txBuffer;
uint8_t txAddress;

/* Sometimes, the TWIM device just freeze and never set the event EVENTS_LASTTX.
 * This method disable and re-enable the peripheral so that it works again.
 * This is just a workaround, and it would be better if we could find a way to prevent
 * this issue from happening.
 * */
void FixHwFreezed() {
  uint32_t twi_state = NRF_TWIM0->ENABLE;
  NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
  NRF_TWIM0->ENABLE = twi_state;
}


void wire_begin(void) {
    NRF_GPIO->PIN_CNF[_uc_pinSCL] = ((uint32_t)GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos)
        | ((uint32_t)GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
        | ((uint32_t)GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
        | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
        | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    NRF_GPIO->PIN_CNF[_uc_pinSDA] = ((uint32_t)GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos)
        | ((uint32_t)GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
        | ((uint32_t)GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
        | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
        | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    NRF_TWIM0->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K250;
    NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
    NRF_TWIM0->PSEL.SCL = _uc_pinSCL;
    NRF_TWIM0->PSEL.SDA = _uc_pinSDA;

    NVIC_ClearPendingIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
    NVIC_SetPriority(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn, 2);
    NVIC_EnableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
}

uint8_t wire_request_from(uint8_t address, size_t quantity, bool stopBit) {
    if (quantity == 0) {
        return 0;
    }

    size_t byteRead = 0;
    rxBuffer.clear();

    NRF_TWIM0->ADDRESS = address;

    NRF_TWIM0->TASKS_RESUME = 0x1UL;
    NRF_TWIM0->RXD.PTR = (uint32_t)rxBuffer._aucBuffer;
    NRF_TWIM0->RXD.MAXCNT = quantity;
    NRF_TWIM0->TASKS_STARTRX = 0x1UL;

    while (!NRF_TWIM0->EVENTS_RXSTARTED && !NRF_TWIM0->EVENTS_ERROR);
    NRF_TWIM0->EVENTS_RXSTARTED = 0x0UL;

    while (!NRF_TWIM0->EVENTS_LASTRX && !NRF_TWIM0->EVENTS_ERROR);
    NRF_TWIM0->EVENTS_LASTRX = 0x0UL;

    if (stopBit || NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM0->EVENTS_STOPPED);
        NRF_TWIM0->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM0->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM0->EVENTS_SUSPENDED);
        NRF_TWIM0->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->EVENTS_ERROR = 0x0UL;
    }

    byteRead = rxBuffer._iHead = NRF_TWIM0->RXD.AMOUNT;

    return byteRead;
}

void wire_begin_transmission(uint8_t address) {
    txAddress = address;
    txBuffer.clear();
    transmissionBegun = true;
}

uint8_t wire_end_transmission(bool stopBit) {
    transmissionBegun = false;

    // Start I2C transmission
    NRF_TWIM0->ADDRESS = txAddress;

    NRF_TWIM0->TASKS_RESUME = 0x1UL;

    NRF_TWIM0->TXD.PTR = (uint32_t)txBuffer._aucBuffer;
    NRF_TWIM0->TXD.MAXCNT = txBuffer.available();

    NRF_TWIM0->TASKS_STARTTX = 0x1UL;

    while (!NRF_TWIM0->EVENTS_TXSTARTED && !NRF_TWIM0->EVENTS_ERROR);
    NRF_TWIM0->EVENTS_TXSTARTED = 0x0UL;

    if (txBuffer.available()) {

        uint32_t txStartedCycleCount = DWT->CYCCNT;
        uint32_t currentCycleCount;

        while (!NRF_TWIM0->EVENTS_LASTTX && !NRF_TWIM0->EVENTS_ERROR) {
            currentCycleCount = DWT->CYCCNT;
            if ((currentCycleCount - txStartedCycleCount) > HwFreezedDelay) {
                FixHwFreezed();
                return 4;
            }
        }
    }
    NRF_TWIM0->EVENTS_LASTTX = 0x0UL;

    if (stopBit || NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM0->EVENTS_STOPPED);
        NRF_TWIM0->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM0->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM0->EVENTS_SUSPENDED);
        NRF_TWIM0->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->EVENTS_ERROR = 0x0UL;

        uint32_t error = NRF_TWIM0->ERRORSRC;

        NRF_TWIM0->ERRORSRC = error;

        if (error == TWIM_ERRORSRC_ANACK_Msk) {
            return 2;
        }
        else if (error == TWIM_ERRORSRC_DNACK_Msk) {
            return 3;
        }
        else {
            return 4;
        }
    }
    return 0;
}

void wire_write(uint8_t ucData) {
    if (!transmissionBegun || txBuffer.isFull()) {
        return;
    }
    txBuffer.store_char(ucData);
}

int wire_read(void) {
    return rxBuffer.read_char();
}

extern "C"
{
    void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void) {
        if (NRF_TWIS0->EVENTS_WRITE) {
            NRF_TWIS0->EVENTS_WRITE = 0x0UL;

            receiving = true;

            rxBuffer.clear();

            NRF_TWIS0->RXD.PTR = (uint32_t)rxBuffer._aucBuffer;
            NRF_TWIS0->RXD.MAXCNT = sizeof(rxBuffer._aucBuffer);

            NRF_TWIS0->TASKS_PREPARERX = 0x1UL;
        }

        if (NRF_TWIS0->EVENTS_READ) {
            NRF_TWIS0->EVENTS_READ = 0x0UL;

            receiving = false;
            transmissionBegun = true;

            txBuffer.clear();

            transmissionBegun = false;

            NRF_TWIS0->TXD.PTR = (uint32_t)txBuffer._aucBuffer;
            NRF_TWIS0->TXD.MAXCNT = txBuffer.available();

            NRF_TWIS0->TASKS_PREPARETX = 0x1UL;
        }

        if (NRF_TWIS0->EVENTS_STOPPED) {
            NRF_TWIS0->EVENTS_STOPPED = 0x0UL;

            if (receiving)
            {
                int rxAmount = NRF_TWIS0->RXD.AMOUNT;

                rxBuffer._iHead = rxAmount;
            }
        }

        if (NRF_TWIS0->EVENTS_ERROR) {
            NRF_TWIS0->EVENTS_ERROR = 0x0UL;

            uint32_t error = NRF_TWIS0->ERRORSRC;
            NRF_TWIS0->ERRORSRC = error;

            NRF_TWIS0->TASKS_STOP = 0x1UL;
        }
    }
}

void init_i2c() {
    wire_begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    set_i2c_reading(true);
    wire_begin_transmission(addr);
    wire_write(reg_addr);
    wire_end_transmission(true);
    wire_request_from(addr, length, true);
    for (uint32_t i = 0; i < length; i++) {
        *reg_data++ = wire_read();
    }
    set_i2c_reading(false);

    return 0;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {
    set_i2c_reading(true);
    wire_begin_transmission(addr);
    wire_write(reg_addr);
    for (uint32_t i = 0; i < length; i++) {
        wire_write(*reg_data++);
    }
    wire_end_transmission(true);
    set_i2c_reading(false);

    return 0;
}



/*


void init_i2c() {
    Wire.begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    Wire.endTransmission(true);
    Wire.requestFrom(addr, length);
    for (uint32_t i = 0; i < length; i++) {
        if (Wire.available()) {
            *reg_data++ = Wire.read();
        } else {
            return 1;
        }
                
    }
    return 0;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {
    
    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    for (uint32_t i = 0; i < length; i++) {
        Wire.write(*reg_data++);
    }
    Wire.endTransmission(true);
    return 0;
}

*/
