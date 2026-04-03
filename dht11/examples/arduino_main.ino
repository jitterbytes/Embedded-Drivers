#include "dht11.h"

dht11_t sensor = {4};
dht11_data_t data;

void setup() {
    Serial.begin(115200);
    dht11_init(&sensor);
}

void loop() {
    if (dht11_read(&sensor, &data) == 0) {
        Serial.print("Temp: ");
        Serial.print(data.temperature);
        Serial.print(" C, Humidity: ");
        Serial.println(data.humidity);
    } else {
        Serial.println("Read failed");
    }
    delay(2000);
}