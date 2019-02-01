#include "MDBoilerController.h"
#include <OneWire.h>                  // Библиотека протокола 1-Wire (датчики температуры)
#include <DallasTemperature.h>        // Библиотека для работы с датчиками DS* температуры

MDBoilerController *boilerController;
OneWire oneWire(4);        // Создаем экземпляр объекта протокола 1-WIRE - OneWire
DallasTemperature sensors(&oneWire);  // На базе ссылки OneWire создаем экземпляр объекта, работающего с датчиками DS*

//14 - ON/OFF
//15- MINES
//17- PLUS

void parseCommand(String command) {
     if (command == "Status") {
        boilerController->changeTimeToSendStatusToNow();
    } else if (command.indexOf("Set ") > -1) {
        String temperatureToSetStr = command.substring(4);
        boilerController->set(temperatureToSetStr);
    } else if (command == "Switch") {
        boilerController->changeBoilerEnabled();
    }
}

void setup()
{
    Serial.begin(9600);
    sensors.begin();    // Запускаем поиск всех датчиков температуры
    
    boilerController = new MDBoilerController(14, 17, 15, 16, 6, 3);
    boilerController->sim800->setParseCommand(parseCommand);
}

void loop()
{
    boilerController->checkForNewSms();
    boilerController->checkRedLight();

    if(boilerController->isTimeToSendStatus()) {
        sensors.requestTemperatures();      // Запускаем измерение температуры на всех датчиках
        float roomTemperature = sensors.getTempCByIndex(0);
        float boilerTemperature = sensors.getTempCByIndex(1);
        boilerController->sendStatus(boilerTemperature, roomTemperature);
    }
}
