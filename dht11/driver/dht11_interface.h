#ifndef DHT11_INTERFACE_H
#define DHT11_INTERFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void dht11_gpio_set_output(int pin);
void dht11_gpio_set_input(int pin);
void dht11_gpio_write(int pin, int level);
int dht11_gpio_read(int pin);

void dht11_delay_us(uint32_t us);
uint32_t dht11_get_time_us(void);

#ifdef __cplusplus
}
#endif

#endif