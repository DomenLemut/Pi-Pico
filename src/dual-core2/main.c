#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#define LED_PIN 25

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq);

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

            uint offset = pio_add_program(pio0, &blink_program);
            blink_pin_forever(pio0, 0, offset, 25, 3);
        }
    }
}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}
