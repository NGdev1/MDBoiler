/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/

#include "Arduino.h"
#include "MDBoilerController.h"

MDBoilerController::MDBoilerController()
{

}

void MDBoilerController::setBoiler(MDBoilerButtons *_boiler) {
    boiler = _boiler;
}

void MDBoilerController::setSim800(MDSim800 *_sim800) {
    sim800 = _sim800;
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
    lastSendStatusTime = 0;
}
