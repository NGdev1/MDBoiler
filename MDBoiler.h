#include "Arduino.h"

class MDBoiler
{
  public:

    MDBoiler(int _enablePin, int _plusPin, int _minesPin);

    void changeEnabled();
    void set(int tempetarure);
    
    int currentTemperature = -1;
    
  private:

    int enablePin;
    int plusPin;
    int minesPin;
};
