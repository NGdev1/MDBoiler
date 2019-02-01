/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/
#include "MDBoilerButtons.h"
#include "MDSim800.h"

class MDBoilerController
{
  public:
    MDBoilerController(int enablePin, int plusPin, int minesPin, int redLightPin, int rxPin, int txPin);
    void sendStatus(float boilerTemperature, float roomTemperature);
    void checkRedLight();
    bool isTimeToSendStatus();
    void changeTimeToSendStatusToNow();
    void changeBoilerEnabled();
    void checkForNewSms();
    void set(String temperature);
    
    MDSim800 *sim800;
  private:
    MDBoilerButtons *boiler;

    bool isUserNotifiedAboutRedLight = false;
    long lastSendStatusTime = millis();
    long statusSendingPeriod = 60000 * 60 * 4;
};
