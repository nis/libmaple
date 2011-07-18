#include "wirish.h"
#include "libraries/FreeRTOS/IRLib.h"

void setup() {
    
}

void loop() {
    
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
