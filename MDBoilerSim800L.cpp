/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/

#include "Arduino.h"
#include "MDBoilerSim800L.h"

MDBoilerSim800L::MDBoilerSim800L(int rxPin, int txPin)
{
    SIM800 = new SoftwareSerial(rxPin, txPin);

    Serial.println("MDBoilerSim800L Start!");

    SIM800->begin(9600);
    sendATCommand("AT", true);
    _response = sendATCommand("AT+CMGF=1;&W", true); // Включаем текстовый режима SMS (Text mode) и сразу сохраняем значение (AT&W)!
}

String MDBoilerSim800L::waitResponse()
{                                     // Функция ожидания ответа и возврата полученного результата
    String _resp = "";                // Переменная для хранения результата
    long _timeout = millis() + 10000; // Переменная для отслеживания таймаута (10 секунд)
    while (!SIM800->available() && millis() < _timeout)
    {
    }; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
    if (SIM800->available())
    {                                 // Если есть, что считывать...
        _resp = SIM800->readString(); // ... считываем и запоминаем
    }
    else
    {                                 // Если пришел таймаут, то...
        Serial.println("Timeout..."); // ... оповещаем об этом и...
    }
    return _resp; // ... возвращаем результат. Пусто, если проблема
}

String MDBoilerSim800L::sendATCommand(String cmd, bool waiting)
{
    String _resp = "";    // Переменная для хранения результата
    Serial.println(cmd);  // Дублируем команду в монитор порта
    SIM800->println(cmd); // Отправляем команду модулю
    if (waiting)
    {                           // Если необходимо дождаться ответа...
        _resp = waitResponse(); // ... ждем, когда будет передан ответ
        // Если Echo Mode выключен (ATE0), то эти 3 строки можно закомментировать
        if (_resp.startsWith(cmd))
        { // Убираем из ответа дублирующуюся команду
            _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
        }
        Serial.println(_resp); // Дублируем ответ в монитор порта
    }
    if (_resp != "" || _resp.length() > 2)
    {
        lastSim800lResponce = _resp;
    }
    return _resp;
}

void MDBoilerSim800L::sendToUser(String message) {
    if(userPhone == NULL) {
        Serial.println("User phone is not defined. Failed to send message.");
        return;
    }

    sendSMS(userPhone, message);
}

void MDBoilerSim800L::parseCommand(String command) {
     if (command == "Status") {
        lastSendStatusTime = 0;
    } else if (command.indexOf("Set ") > -1) {
        int temperatureToSet = command.substring(command.indexOf("Set ")).toInt();
        boiler->set(temperatureToSet);
        sendToUser("New temperature " + String(temperatureToSet) + " has been set.");
    } else if (command == "Switch") {
        boiler->changeEnabled();
        sendToUser("Switched.");
    }
}

void MDBoilerSim800L::parseSMS(String msg)
{
    String msgheader = "";
    String msgbody = "";
    String msgphone = "";

    msg = msg.substring(msg.indexOf("+CMGR: "));
    msgheader = msg.substring(0, msg.indexOf("\r"));

    msgbody = msg.substring(msgheader.length() + 2);
    msgbody = msgbody.substring(0, msgbody.lastIndexOf("OK"));
    msgbody.trim();

    int firstIndex = msgheader.indexOf("\",\"") + 3;
    int secondIndex = msgheader.indexOf("\",\"", firstIndex);
    msgphone = msgheader.substring(firstIndex, secondIndex);

    Serial.println("Phone: " + msgphone);
    Serial.println("Message: " + msgbody);

    if(msgbody == "Start") {
        userPhone = msgphone;
        sendSMS(msgphone, "OK");
    } else {
        parseCommand(msgbody);
    }
}

void MDBoilerSim800L::checkForNewSms()
{
    if (lastUpdate + updatePeriod < millis())
    { // Пора проверить наличие новых сообщений
        do
        {
            _response = sendATCommand("AT+CMGL=\"REC UNREAD\",1", true); // Отправляем запрос чтения непрочитанных сообщений
            if (_response.indexOf("+CMGL: ") > -1)
            { // Если есть хоть одно, получаем его индекс
                int msgIndex = _response.substring(_response.indexOf("+CMGL: ") + 7, _response.indexOf("\"REC UNREAD\"", _response.indexOf("+CMGL: ")) - 1).toInt();
                char i = 0; // Объявляем счетчик попыток
                do
                {
                    i++;                                                                   // Увеличиваем счетчик
                    _response = sendATCommand("AT+CMGR=" + (String)msgIndex + ",1", true); // Пробуем получить текст SMS по индексу
                    _response.trim();                                                      // Убираем пробелы в начале/конце
                    if (_response.endsWith("OK"))
                    { // Если ответ заканчивается на "ОК"
                        if (!hasmsg)
                            hasmsg = true;                                  // Ставим флаг наличия сообщений для удаления
                        sendATCommand("AT+CMGR=" + (String)msgIndex, true); // Делаем сообщение прочитанным
                        sendATCommand("\n", true);                          // Перестраховка - вывод новой строки
                        parseSMS(_response);                                // Отправляем текст сообщения на обработку
                        break;                                              // Выход из do{}
                    }
                    else
                    {                                   // Если сообщение не заканчивается на OK
                        Serial.println("Error answer"); // Какая-то ошибка
                        sendATCommand("\n", true);      // Отправляем новую строку и повторяем попытку
                    }
                } while (i < 10);
                break;
            }
            else
            {
                lastUpdate = millis(); // Обнуляем таймер
                if (hasmsg)
                {
                    sendATCommand("AT+CMGDA=\"DEL ALL\"", true); // Удаляем все сообщения
                    hasmsg = false;
                }
                break;
            }
        } while (1);
    }

    if (SIM800->available())
    {                               // Если модем, что-то отправил...
        _response = waitResponse(); // Получаем ответ от модема для анализа
        _response.trim();           // Убираем лишние пробелы в начале и конце
        Serial.println(_response);  // Если нужно выводим в монитор порта
        if (_response.indexOf("+CMTI:") > -1)
        {                                         // Пришло сообщение об отправке SMS
            lastUpdate = millis() - updatePeriod; // Теперь нет необходимости обрабатываеть SMS здесь, достаточно просто// сбросить счетчик автопроверки и в следующем цикле все будет обработано
        }
        lastSim800lResponce = _response;
    }
    if (Serial.available())
    {                                 // Ожидаем команды по Serial...
        SIM800->write(Serial.read()); // ...и отправляем полученную команду модему
    };
}

void MDBoilerSim800L::sendSMS(String phone, String message)
{
    sendATCommand("AT+CMGS=\"" + phone + "\"", true);           // Переходим в режим ввода текстового сообщения
    sendATCommand(message + "\r\n" + (String)((char)26), true); // После текста отправляем перенос строки и Ctrl+Z
}

void MDBoilerSim800L::setBoiler(MDBoiler *_boiler) {
    boiler = _boiler;
}

void MDBoilerSim800L::sendStatus(float boilerTemperature) {
    if(userPhone == NULL) {
        Serial.println("User phone is not defined. Failed to send status.");
        return;
    }

    String textToSend = "Temperature near boiler is " + String(boilerTemperature) + "\n";
    if(boiler->currentTemperature == -1) {
        textToSend += "No boiler temperature set";
    } else {
        textToSend += "Last boiler temperature set: " + String(boiler->currentTemperature);
    }

    sendSMS(userPhone, textToSend);
}
