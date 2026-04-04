#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes — tells you exactly where it failed
typedef enum {
    DHT11_OK                =  0,
    DHT11_ERR_LINE_STUCK    = -1,  // line didn't go HIGH after MCU released — wiring/short
    DHT11_ERR_RESPONSE_LOW  = -2,  // sensor didn't pull LOW (sensor not responding)
    DHT11_ERR_RESPONSE_HIGH = -3,  // sensor didn't pull HIGH (80us ack missing)
    DHT11_ERR_DATA_START    = -4,  // sensor didn't pull LOW to start data
    DHT11_ERR_BIT_PREAMBLE  = -5,  // bit preamble (50us LOW) timed out
    DHT11_ERR_BIT_TIMING    = -6,  // bit HIGH pulse timed out
    DHT11_ERR_CHECKSUM      = -7,  // data corrupted
} dht11_err_t;

typedef struct { int pin; } dht11_t;

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