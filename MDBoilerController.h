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
    void sendStatus(float boilerTemperature);

    long lastSendStatusTime = millis();
    long timeToSendStatus = 60000;
  private:
    void parseCommand(String command);

    MDBoilerButtons *boiler;
    MDSim800L *sim8000;
};
