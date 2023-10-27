#include <Arduino.h>
int ledPin = 13;

void setup() {
    Serial.begin(9600, SERIAL_8N1);
    pinMode(ledPin, OUTPUT);
}

void loop() {
    char buffer[100];
    uint8_t state;

    if (Serial.available() > 0) {
        state = Serial.read();
        if (state == 0 || state == 1) {
            digitalWrite(ledPin, state);
            snprintf(buffer, sizeof(buffer), "Set state to %d", state);
        } else {
            snprintf(buffer, sizeof(buffer), "State unknown: %d", state);
        }
        Serial.print(buffer);
    }
}
