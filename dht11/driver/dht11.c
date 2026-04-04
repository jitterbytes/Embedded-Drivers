#include "dht11.h"
#include "dht11_interface.h"

// Generous timeouts based on datasheet maximums + margin
#define DHT11_TIMEOUT_RESPONSE_US  200   // sensor response phase (80+80us)
#define DHT11_TIMEOUT_BIT_US       200   // bit preamble (50us) + bit pulse (max 70us)

int dht11_init(dht11_t *dev) {
    dht11_gpio_set_output(dev->pin);
    dht11_gpio_write(dev->pin, 1);
    return DHT11_OK;
}

static int wait_for_level(int pin, int level, uint32_t timeout_us) {
    uint32_t start = dht11_get_time_us();
    while (dht11_gpio_read(pin) != level) {
        if ((dht11_get_time_us() - start) > timeout_us) return -1;
    }
    return 0;
}

int dht11_read(dht11_t *dev, dht11_data_t *data) {
    uint8_t bits[5] = {0};
    uint32_t low_time[40], high_time[40];

    // --- START SIGNAL ---
    dht11_gpio_set_output(dev->pin);
    dht11_gpio_write(dev->pin, 0);
    dht11_delay_us(18000);          // ≥18ms — MCU pulls low
    dht11_gpio_write(dev->pin, 1);
    dht11_delay_us(40);             // 20-40us — MCU releases, datasheet max is 40
    dht11_gpio_set_input(dev->pin); // now listen

    // --- SENSOR HANDSHAKE (outside interrupt lock, these are long pulses) ---
    // Confirm line is HIGH before watching for sensor response
	if (wait_for_level(dev->pin, 1, DHT11_TIMEOUT_RESPONSE_US))
		return DHT11_ERR_RESPONSE_LOW;   // line stuck low — wiring problem

	// Sensor pulls LOW ~80us
	if (wait_for_level(dev->pin, 0, DHT11_TIMEOUT_RESPONSE_US))
		return DHT11_ERR_RESPONSE_HIGH;

	// Sensor releases HIGH ~80us  
	if (wait_for_level(dev->pin, 1, DHT11_TIMEOUT_RESPONSE_US))
		return DHT11_ERR_RESPONSE_END;

	// Sensor pulls LOW — data starts
	if (wait_for_level(dev->pin, 0, DHT11_TIMEOUT_RESPONSE_US))
		return DHT11_ERR_DATA_START;

    // --- READ 40 BITS (timing critical — disable interrupts) ---
    dht11_disable_interrupts();

    for (int i = 0; i < 40; i++) {
        // Each bit starts with a 50us LOW preamble — use it as reference
        uint32_t t = dht11_get_time_us();
        if (wait_for_level(dev->pin, 1, DHT11_TIMEOUT_BIT_US)) {
            dht11_enable_interrupts();
            return DHT11_ERR_BIT_PREAMBLE;
        }
        low_time[i] = dht11_get_time_us() - t;  // how long was LOW (ref = 50us)

        // Now measure the HIGH pulse: 26-28us = 0, 70us = 1
        t = dht11_get_time_us();
        if (wait_for_level(dev->pin, 0, DHT11_TIMEOUT_BIT_US)) {
            dht11_enable_interrupts();
            return DHT11_ERR_BIT_TIMING;
        }
        high_time[i] = dht11_get_time_us() - t; // how long was HIGH
    }

    dht11_enable_interrupts();

    // --- DECODE: ratio-based, no magic number ---
    for (int i = 0; i < 40; i++) {
        bits[i/8] <<= 1;
        if (high_time[i] > low_time[i]) {  // HIGH > LOW means bit=1
            bits[i/8] |= 1;
        }
    }

    // --- CHECKSUM ---
    if ((bits[0] + bits[1] + bits[2] + bits[3]) != bits[4])
        return DHT11_ERR_CHECKSUM;

    data->humidity    = bits[0];
    data->temperature = bits[2];

    return DHT11_OK;
}