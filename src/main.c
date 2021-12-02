#include <stdio.h>
#include "nrf52.h"
#include "pinetime_board.h"

#define LOOP_STACK_SZ       (256*2)
#define CALLBACK_STACK_SZ   (256*1)

static TaskHandle_t  _loopHandle;

static void loop_task(void* arg) {

    (void)arg;
    setup();

    while (1) {
        loop();
        yield(); // yield to run other task
    }
}


#define NRF52_ONRAM1_OFFRAM0    POWER_RAM_POWER_S0POWER_On      << POWER_RAM_POWER_S0POWER_Pos      \
												      | POWER_RAM_POWER_S1POWER_On      << POWER_RAM_POWER_S1POWER_Pos      \
												      | POWER_RAM_POWER_S0RETENTION_Off << POWER_RAM_POWER_S0RETENTION_Pos  \
	                            | POWER_RAM_POWER_S1RETENTION_Off << POWER_RAM_POWER_S1RETENTION_Pos;

void configure_ram_retention(void) {

    // Configure nRF52 RAM retention parameters. Set for System On 64kB RAM retention
    /*NRF_POWER->RAM[0].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[1].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[2].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[3].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[4].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[5].POWER = NRF52_ONRAM1_OFFRAM0;
    NRF_POWER->RAM[6].POWER = NRF52_ONRAM1_OFFRAM0;*/
    NRF_POWER->RAM[7].POWER = NRF52_ONRAM1_OFFRAM0;
}

void clean_i2c(void) {
    NRF_TWIM0->ENABLE = 0;
    *(volatile uint32_t *)0x40003FFC = 0;
    *(volatile uint32_t *)0x40003FFC;
    *(volatile uint32_t *)0x40003FFC = 1;

    NRF_TWIM1->ENABLE = 0;
    *(volatile uint32_t *)0x40004FFC = 0;
    *(volatile uint32_t *)0x40004FFC;
    *(volatile uint32_t *)0x40004FFC = 1;

    pinMode(TWI_SCL, OUTPUT);
    digitalWrite(TWI_SCL, HIGH);
    for(uint8_t i = 0; i < 16; i++) {
        digitalToggle(TWI_SCL);
        delay_ns(5);
    }

}

int main(void) {

    dwt_enable();

    clean_i2c();

    configure_ram_retention();

    init();

    // Create a task for loop()
    xTaskCreate(loop_task, "loop", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, &_loopHandle);

    // Initialize callback task
    ada_callback_init(CALLBACK_STACK_SZ);

    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    NVIC_SystemReset();

    return 0;

}

void suspendLoop(void) {
    vTaskSuspend(_loopHandle);
}

void resumeLoop(void) {
    if (isInISR()) {
        xTaskResumeFromISR(_loopHandle);
    }
    else {
        vTaskResume(_loopHandle);
    }
}
