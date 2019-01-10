#include "MDBoilerSim800L.h"
#include <OneWire.h>                  // Библиотека протокола 1-Wire
#include <DallasTemperature.h>        // Библиотека для работы с датчиками DS*

MDBoilerSim800L *sim800Boiler;
MDBoiler *boiler;
OneWire oneWire(5);        // Создаем экземпляр объекта протокола 1-WIRE - OneWire
DallasTemperature sensors(&oneWire);  // На базе ссылки OneWire создаем экземпляр объекта, работающего с датчиками DS*

//14 - ON/OFF
//16- PLUS
//18- MINES

void setup()
{
    Serial.begin(9600);
    sim800Boiler = new MDBoilerSim800L(6, 3);
    boiler = new MDBoiler(14, 16, 18);
    sim800Boiler->setBoiler(boiler);

    sensors.begin();                    // Запускаем поиск всех датчиков
}

void loop()
{
    sim800Boiler->checkForNewSms();

    if(sim800Boiler->lastSendStatusTime + sim800Boiler->timeToSendStatus < millis()) {
        sim800Boiler->lastSendStatusTime = millis();
        
        sensors.requestTemperatures();      // Запускаем измерение температуры на всех датчиках
        float temperature = sensors.getTempCByIndex(0);

        Serial.print("Temperature is: ");
        Serial.println(temperature);

        sim800Boiler->sendStatus(temperature);
    }
}
