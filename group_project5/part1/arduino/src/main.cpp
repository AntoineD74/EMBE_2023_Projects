#include <Arduino.h>

int ledPin = 13;

void setup() {
    Serial.begin(9600, SERIAL_8N1);
    pinMode(ledPin, OUTPUT);

    Serial.println("Booting...");
    digitalWrite(ledPin, LOW);
}

void loop() {
    char buffer[100];
    uint8_t state;

    if (Serial.available() > 0) {
        Serial.print("\nCommand received");
        state = Serial.read();

        if (state == 2 || state == 1) {
            if(state == 2){ state = 0; }
            snprintf(buffer, sizeof(buffer), "  Set state to %d", state);
            digitalWrite(ledPin, state);  
            delay(1000);
        } else {
            snprintf(buffer, sizeof(buffer), "\nError command: %d", state);
        }
        Serial.print(buffer);
    }
}
