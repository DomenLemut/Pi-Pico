#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_PIN 25
#define PWM_FREQ 5000
#define PWM_RANGE 255

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize PWM channel
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, PWM_RANGE);
    pwm_set_clkdiv(slice_num, 4.0f);
    pwm_set_enabled(slice_num, true);

    // Set PWM frequency
    uint32_t clock_freq = 125000000;
    pwm_set_clkdiv_int_frac(slice_num, clock_freq / (PWM_FREQ * PWM_RANGE), 0);

    while (true) {
        for (int duty_cycle = 0; duty_cycle <= PWM_RANGE; duty_cycle++) {
            pwm_set_chan_level(slice_num, PWM_CHAN_A, duty_cycle);
            sleep_ms(10);
        }
        for (int duty_cycle = PWM_RANGE; duty_cycle >= 0; duty_cycle--) {
            pwm_set_chan_level(slice_num, PWM_CHAN_A, duty_cycle);
            sleep_ms(10);
        }
    }
}
