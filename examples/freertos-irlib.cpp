#include "wirish.h"
#include "libraries/FreeRTOS/MapleFreeRTOS.h"
#include "libraries/ARRLib/ARRLib.h"

#define IR_RECEIVER_PIN 14 // IR Receiver pin

ARRLib ir(IR_RECEIVER_PIN);

static void vLEDFlashTask(void *pvParameters) {
    for (;;) {
        vTaskDelay(500);
        digitalWrite(BOARD_LED_PIN, HIGH);
        vTaskDelay(1);
        digitalWrite(BOARD_LED_PIN, LOW);
    }
}

static void dev_task(void *pvParameters) {
	while (1) {
		ir.ir_debug();
		// SerialUSB.println(ir.ir_debug(), DEC);
		// if(ir.ir_debug())
		// {
		// 	SerialUSB.println("Weeee!");
		// } else {
		// 	SerialUSB.println("Boooh!");
		// }
		vTaskDelay(500);
	}
}

void setup() {
    // initialize the digital pin as an output:
    pinMode(BOARD_LED_PIN, OUTPUT);
    //pinMode(IR_RECEIVER_PIN, INPUT);
	
	Serial1.begin(115200);
	
	xTaskCreate(dev_task,
                (signed portCHAR *)"Task2",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);
	
    xTaskCreate(vLEDFlashTask,
                (signed portCHAR *)"Task1",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);
    vTaskStartScheduler();
}

void loop() {
    // Insert background code here
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (true) {
        loop();
    }
    return 0;
}
