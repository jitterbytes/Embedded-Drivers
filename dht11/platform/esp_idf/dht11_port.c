#include "driver/gpio.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"

void dht11_gpio_set_output(int pin) {
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void dht11_gpio_set_input(int pin) {
    gpio_set_direction(pin, GPIO_MODE_INPUT);
}

void dht11_gpio_write(int pin, int level) {
    gpio_set_level(pin, level);
}

int dht11_gpio_read(int pin) {
    return gpio_get_level(pin);
}

void dht11_delay_us(uint32_t us) {
    ets_delay_us(us);
}

uint32_t dht11_get_time_us(void) {
    return (uint32_t)esp_timer_get_time();
}