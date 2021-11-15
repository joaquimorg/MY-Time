#include <stdio.h>
#include "nrf52.h"

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


int main(void) {

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
  if ( isInISR() ) {
    xTaskResumeFromISR(_loopHandle);
  } else {
    vTaskResume(_loopHandle);
  }
}
