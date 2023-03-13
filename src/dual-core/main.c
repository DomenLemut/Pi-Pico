#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#define LED_PIN 25

void setup() {
    //init gpio
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void main_1() {
    while(1) {
        //look for data from first CPU core
        if(multicore_fifo_rvalid()) {
            uint32_t data_c1 = multicore_fifo_pop_blocking();

            //neki procesiraj
            for(volatile int i = 0; i < data_c1; i++) {}

            //poglej, ce se da podatke nazaj poslat, öe ja jih poülji
            while(!multicore_fifo_wready()) {}
            multicore_fifo_push_blocking(data_c1);
        }
    }
}

int main() {
    setup();

    multicore_launch_core1(main_1);

    //send data to second CPU core
    uint32_t data_c2 = 100000000;
    multicore_fifo_push_blocking(data_c2);

    while(1) {
        if(multicore_fifo_rvalid()) {
            data_c2 = multicore_fifo_pop_blocking();
            while(!multicore_fifo_wready()) {}
            multicore_fifo_push_blocking(data_c2);

            //neki przigaj lucke
            gpio_put(LED_PIN, 1);
            sleep_ms(10);
            gpio_put(LED_PIN, 0);
        }
    }
}
