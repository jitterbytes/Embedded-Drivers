#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int pin;
} dht11_t;

typedef struct {
    uint8_t temperature;
    uint8_t humidity;
} dht11_data_t;

int dht11_init(dht11_t *dev);
int dht11_read(dht11_t *dev, dht11_data_t *data);

#ifdef __cplusplus
}
#endif

#endif