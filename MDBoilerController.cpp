/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/

#include "Arduino.h"
#include "MDBoilerController.h"

MDBoilerController::MDBoilerController()
{

}

void MDBoilerController::parseCommand(String command) {
     if (command == "Status") {
        lastSendStatusTime = 0;
    } else if (command.indexOf("Set ") > -1) {
        String temperatureToSetStr = command.substring(command.indexOf("Set") + 4);
        int temperatureToSet = temperatureToSetStr.toInt();
        boiler->set(temperatureToSet);
        sendToUser("New temperature " + temperatureToSetStr + " has been set.");
    } else if (command == "Switch") {
        boiler->changeEnabled();
        sendToUser("Switched.");
    }
}

void MDBoilerController::setBoiler(MDBoilerButtons *_boiler) {
    boiler = _boiler;
}

void MDBoilerController::setSim800(MDSim800 *_sim800) {
    sim8000 = _sim800;
}

void MDBoilerController::sendStatus(float boilerTemperature) {
    String textToSend = "Temperature near boiler is " + String(boilerTemperature) + "\n";
    if(boiler->currentTemperature == -1) {
        textToSend += "No boiler temperature set";
    } else {
        textToSend += "Last boiler temperature set: " + String(boiler->currentTemperature);
    }

    sendSMS(userPhone, textToSend);
}
