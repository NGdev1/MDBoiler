/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/
#include "MDBoilerButtons.h"
#include "MDSim800.h"

class MDBoilerController
{
    typedef void (*parseCommandCallback_t)(String);

  public:
    MDBoilerController(int enablePin, int plusPin, int minesPin, int redLightPin, int rxPin, int txPin, parseCommandCallback_t parseCommand);
    void sendStatus(float boilerTemperature, float roomTemperature);
    void checkRedLight();
    bool isTimeToSendStatus();
    void changeTimeToSendStatusToNow();
    void changeBoilerEnabled();
    void checkForNewSms();
    void set(String temperature);
    
  private:
    MDBoilerButtons *boiler;
    MDSim800 *sim800;

    bool isUserNotifiedAboutRedLight = false;
    long lastSendStatusTime = millis();
    long statusSendingPeriod = 60000 * 60 * 4;
};
