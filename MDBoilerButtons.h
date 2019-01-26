#include "Arduino.h"

class MDBoilerButtons
{
  public:

    MDBoilerButtons(int _enablePin, int _plusPin, int _minesPin, int _redLightPin);

    void changeEnabled();
    void set(int tempetarure);
    bool isRedLightOn();
    
    int currentTemperature = -1;
    
  private:

    int enablePin;
    int plusPin;
    int minesPin;
    int redLightPin;
};
