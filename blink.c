// #include <stdio.h>
#include "pico/stdlib.h"

#define ON 1
#define OFF 0
#define SLEEP 2000

#define LED_PIN 25

int main(){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(true){
        gpio_put(LED_PIN, ON);
        sleep_ms(SLEEP);
        gpio_put(LED_PIN, OFF);
        sleep_ms(SLEEP);
    }
}