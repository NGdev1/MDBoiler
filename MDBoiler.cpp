#include "MDBoiler.h"

MDBoiler::MDBoiler(int _enablePin, int _plusPin, int _minesPin) {
    enablePin = _enablePin;
    plusPin = _plusPin;
    minesPin = _minesPin;

    pinMode(enablePin, OUTPUT);
    pinMode(plusPin, OUTPUT);
    pinMode(minesPin, OUTPUT);
}

void MDBoiler::changeEnabled() {
    digitalWrite(enablePin, HIGH);
    delay(100);
    digitalWrite(enablePin, LOW);
}

void MDBoiler::set(int tempetarure) {
    currentTemperature = tempetarure;

    //Сброс заданной температуры до нуля
    for (int i = 0; i < 50; i++) {
        digitalWrite(minesPin, HIGH);
        delay(100);
        digitalWrite(minesPin, LOW);
        delay(100);
    }

    for (int i = 0; i < tempetarure; i++) {
        digitalWrite(plusPin, HIGH);
        delay(100);
        digitalWrite(plusPin, LOW);
        delay(100);
    }
}
