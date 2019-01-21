#include "MDBoilerController.h"
#include "MDBoilerButtons.h"
#include <OneWire.h>                  // Библиотека протокола 1-Wire
#include <DallasTemperature.h>        // Библиотека для работы с датчиками DS*

MDBoilerController *boilerController;
MDBoilerButtons *boilerButtons;
OneWire oneWire(4);        // Создаем экземпляр объекта протокола 1-WIRE - OneWire
DallasTemperature sensors(&oneWire);  // На базе ссылки OneWire создаем экземпляр объекта, работающего с датчиками DS*

//14 - ON/OFF
//16- PLUS
//18- MINES

void setup()
{
    Serial.begin(9600);
    boilerController = new MDBoilerController(6, 3);
    boilerButtons = new MDBoilerButtons(17, 14, 15);
    boilerController->setBoiler(boilerButtons);

    sensors.begin();    // Запускаем поиск всех датчиков
}

void loop()
{
    boilerController->checkForNewSms();
    
    if(boilerController->lastSendStatusTime + boilerController->timeToSendStatus < millis()) {
        boilerController->lastSendStatusTime = millis();
        
        sensors.requestTemperatures();      // Запускаем измерение температуры на всех датчиках
        float temperature = sensors.getTempCByIndex(0);

        Serial.print("Temperature is: ");
        Serial.println(temperature);

        boilerController->sendStatus(temperature);
    }
}
