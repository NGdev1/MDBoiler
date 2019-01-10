/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/

#include <SoftwareSerial.h>
#include "MDBoiler.h"

class MDBoilerSim800L
{
  public:
    MDBoilerSim800L(int rxPin, int txPin);
    void setBoiler(MDBoiler *_boiler);
    void sendSMS(String phone, String message);
    void checkForNewSms();
    void sendStatus(float boilerTemperature);
    void sendToUser(String message);

    long lastSendStatusTime = millis();
    long timeToSendStatus = 60000;
  private:
    String sendATCommand(String cmd, bool waiting);
    String waitResponse();
    void parseSMS(String msg);
    void parseCommand(String command);

    SoftwareSerial *SIM800;
    MDBoiler *boiler;

    String _response;
    String lastSim800lResponce;
    String userPhone;
    long lastUpdate = millis(); // Время последнего обновления
    long updatePeriod = 60000;  // Проверять каждую минуту
    bool hasmsg = false;        // Флаг наличия сообщений к удалению
};
