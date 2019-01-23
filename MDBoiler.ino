#include "MDBoilerController.h"
#include <OneWire.h>                  // Библиотека протокола 1-Wire (датчики температуры)
#include <DallasTemperature.h>        // Библиотека для работы с датчиками DS* температуры

MDBoilerController *boilerController;
MDBoilerButtons *boilerButtons;
MDSim800 *sim800;
OneWire oneWire(4);        // Создаем экземпляр объекта протокола 1-WIRE - OneWire
DallasTemperature sensors(&oneWire);  // На базе ссылки OneWire создаем экземпляр объекта, работающего с датчиками DS*

//14 - ON/OFF
//16- PLUS
//18- MINES

void parseCommand(String command) {
     if (command == "Status") {
        boilerController->changeTimeToSendStatusToNow();
    } else if (command.indexOf("Set ") > -1) {
        String temperatureToSetStr = command.substring(4);
        int temperatureToSet = temperatureToSetStr.toInt();
        boilerButtons->set(temperatureToSet);
        sim800->sendToUser("New temperature " + temperatureToSetStr + " has been set.");
    } else if (command == "Switch") {
        boilerButtons->changeEnabled();
        sim800->sendToUser("Switched.");
    }
}

void setup()
{
    Serial.begin(9600);
    sensors.begin();    // Запускаем поиск всех датчиков температуры
    
    boilerController = new MDBoilerController();

    boilerButtons = new MDBoilerButtons(17, 14, 15);
    boilerController->setBoiler(boilerButtons);

    sim800 = new MDSim800(6, 3);
    sim800->setParseCommand(parseCommand);
    boilerController->setSim800(sim800);
}

void loop()
{
    sim800->checkForNewSms();
    
    if(boilerController->isTimeToSendStatus()) {
        sensors.requestTemperatures();      // Запускаем измерение температуры на всех датчиках
        float boilerTemperature = sensors.getTempCByIndex(0);
        float roomTemperature = sensors.getTempCByIndex(1);
        boilerController->sendStatus(boilerTemperature, roomTemperature);
    }
}
