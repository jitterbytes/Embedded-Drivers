#include <Arduino.h>

extern "C" {

void dht11_gpio_set_output(int pin) {
    pinMode(pin, OUTPUT);
}

void dht11_gpio_set_input(int pin) {
    pinMode(pin, INPUT);
}

void dht11_gpio_write(int pin, int level) {
    digitalWrite(pin, level);
}

int dht11_gpio_read(int pin) {
    return digitalRead(pin);
}

void dht11_delay_us(uint32_t us) {
    delayMicroseconds(us);
}

uint32_t dht11_get_time_us(void) {
    return micros();
}

}