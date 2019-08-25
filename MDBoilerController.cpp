/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/

#include "Arduino.h"
#include "MDBoilerController.h"

MDBoilerController::MDBoilerController(int enablePin, int plusPin, int minesPin, int redLightPin, int rxPin, int txPin)
{
    boiler = new MDBoilerButtons(enablePin, plusPin, minesPin, redLightPin);
    sim800 = new MDSim800(rxPin, txPin);
}

void MDBoilerController::sendStatus(float boilerTemperature, float roomTemperature) {
    lastSendStatusTime = millis();

    String textToSend = "Temperature near boiler is " + String(boilerTemperature) +
    ", room temperature is " + String(roomTemperature) + "\n";

    if(boiler->currentTemperature == -1) {
        textToSend += "No boiler temperature set";
    } else {
        textToSend += "Last boiler temperature set: " + String(boiler->currentTemperature);
    }

    sim800->sendToUser(textToSend);
}

bool MDBoilerController::isTimeToSendStatus() {
    return lastSendStatusTime + statusSendingPeriod < millis();
}

void MDBoilerController::changeTimeToSendStatusToNow() {
    lastSendStatusTime = -60000 * 60 * 4;
}

void MDBoilerController::checkRedLight() {
    if (isUserNotifiedAboutRedLight) return;

    if(boiler->isRedLightOn()) {
        sim800->sendToUser("Warning! Red light is on.");
        isUserNotifiedAboutRedLight = true;
    }
}

void MDBoilerController::changeBoilerEnabled() {
    isUserNotifiedAboutRedLight = false;

    boiler->changeEnabled();
    sim800->sendToUser("Switched.");
}

void MDBoilerController::set(String temperature) {
    boiler->set(temperature.toInt());
    sim800->sendToUser("New temperature " + temperature + " has been set.");
}

void MDBoilerController::checkForNewSms() {
    sim800->checkForNewSms();
}
