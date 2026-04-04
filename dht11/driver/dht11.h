#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes — tells you exactly where it failed
typedef enum {
    DHT11_OK               =  0,
    DHT11_ERR_RESPONSE_LOW = -1,  // sensor didn't pull LOW after start signal
    DHT11_ERR_RESPONSE_HIGH= -2,  // sensor didn't pull HIGH (80us ack)
    DHT11_ERR_RESPONSE_END = -3,  // sensor didn't release line before data
    DHT11_ERR_BIT_PREAMBLE = -4,  // bit preamble (50us LOW) timed out
    DHT11_ERR_BIT_TIMING   = -5,  // bit HIGH pulse timed out
    DHT11_ERR_CHECKSUM     = -6,  // data received but corrupted
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