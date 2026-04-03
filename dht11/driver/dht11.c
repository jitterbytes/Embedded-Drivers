#include "dht11.h"
#include "dht11_interface.h"

#define DHT11_TIMEOUT_US 100

int dht11_init(dht11_t *dev) {
	dht11_gpio_set_output(dev->pin);
	dht11_gpio_write(dev->pin, 1);
	return 0;
}

static int wait_for_level(int pin, int level, uint32_t timeout_us) {
	uint32_t start = dht11_get_time_us();

	while(dht11_gpio_read(pin) != level) {
		if((dht11_get_time_us() - start) > timeout_us)	return -1;
	}
	return 0;
}

int dht11_read(dht11_t *dev, dht11_data_t *data) {
	uint8_t bits[5] = {0};

	// start signal
	dht11_gpio_set_output(dev->pin);
	dht11_gpio_write(dev->pin, 0);		// pulling wire low to start comm
	dht11_delay_us(18000);				// wait for 18ms
	dht11_gpio_write(dev->pin, 1);		// releasing wire high 
	dht11_delay_us(30);					// giving sensor time to respond
	
	dht11_gpio_set_input(dev->pin);		// sensor will talk MCU will listen 

	// sensor response 0 - 1 - 0
	if(wait_for_level(dev->pin, 0, DHT11_TIMEOUT_US)) return -1;
	if(wait_for_level(dev->pin, 1, DHT11_TIMEOUT_US)) return -1;
	if(wait_for_level(dev->pin, 0, DHT11_TIMEOUT_US)) return -1;

	// read 40 bits
	for (int i=0; i < 40; i++) {
		
		if(wait_for_level(dev->pin, 1, DHT11_TIMEOUT_US)) return -1;	// waiting for start bit		
		uint32_t start = dht11_get_time_us();							// start timing		
		if(wait_for_level(dev->pin, 0, DHT11_TIMEOUT_US)) return -1;	// waiting for line to go low
		uint32_t duration = dht11_get_time_us() - start;				// calculate duration - how long it was high
		// data comes bit by bit so if bit is 1 then add the bit to respective byte if 0 then dont do anything
		if(duration > 40) {	// >40us = bit = 1, else bit = 0
			bits[i/8] |= (1 << (7 - (i%8)));
		}
	}

	// checksum - if data is valid
	if((bits[0] + bits[1] + bits[2] + bits[3]) != bits[4])	return -2;

	data->humidity = bits[0];
	data->temperature = bits[2];

	return 0;
}
