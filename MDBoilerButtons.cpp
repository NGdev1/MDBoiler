#include "MDBoilerButtons.h"

MDBoilerButtons::MDBoilerButtons(int _enablePin, int _plusPin, int _minesPin, int _redLightPin) {
    enablePin = _enablePin;
    plusPin = _plusPin;
    minesPin = _minesPin;
    redLightPin = _redLightPin;

    pinMode(enablePin, OUTPUT);
    pinMode(plusPin, OUTPUT);
    pinMode(minesPin, OUTPUT);
    pinMode(redLightPin, INPUT);

    digitalWrite(enablePin, LOW);
    digitalWrite(plusPin, LOW);
    digitalWrite(minesPin, LOW);
}

void MDBoilerButtons::changeEnabled() {
    digitalWrite(enablePin, HIGH);
    delay(100);
    digitalWrite(enablePin, LOW);
}

void MDBoilerButtons::set(int tempetarure) {
    currentTemperature = tempetarure;

    //Сброс заданной температуры до 40
    for (int i = 0; i < 40; i++) {
        digitalWrite(minesPin, HIGH);
        delay(100);
        digitalWrite(minesPin, LOW);
        delay(100);
    }

    for (int i = 0; i < (tempetarure - 40); i++) {
        digitalWrite(plusPin, HIGH);
        delay(100);
        digitalWrite(plusPin, LOW);
        delay(100);
    }
}

bool MDBoilerButtons::isRedLightOn() {
    return (analogRead(redLightPin) > 400);
}
