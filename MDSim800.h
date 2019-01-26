/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/
#include <SoftwareSerial.h>

class MDSim800
{
    typedef void (*parseCommandCallback_t)(String);

  public:
    MDSim800(int rxPin, int txPin);
    void sendSMS(String phone, String message);
    void checkForNewSms();
    void sendToUser(String message);
    void setParseCommand(parseCommandCallback_t _parseCommand);
  private:
    String sendATCommand(String cmd, bool waiting);
    String waitResponse();
    void parseSMS(String msg);

    SoftwareSerial *SIM800;
    
    parseCommandCallback_t parseCommand;
    String _response;
    String userPhone;
    long lastUpdate = millis(); // Время последнего обновления
    long updatePeriod = 60000;  // Проверять каждую минуту
    bool hasmsg = false;        // Флаг наличия сообщений к удалению
};
