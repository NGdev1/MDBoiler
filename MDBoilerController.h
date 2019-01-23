/*
 Created by Дмитрий Несмелов, Михаил Андреичев, Январь 2019.
*/
#include "MDBoilerButtons.h"
#include "MDSim800.h"

class MDBoilerController
{
  public:
    MDBoilerController();
    void setBoiler(MDBoilerButtons *_boiler);
    void setSim800(MDSim800 *_sim800);
    void sendStatus(float boilerTemperature, float roomTemperature);
    bool isTimeToSendStatus();
    void changeTimeToSendStatusToNow();
    
  private:
    MDBoilerButtons *boiler;
    MDSim800 *sim800;

    long lastSendStatusTime = millis();
    long statusSendingPeriod = 60000;
};
