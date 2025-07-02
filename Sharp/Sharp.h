#ifndef sharp_dist
#define sharp_dist

#include "Arduino.h"
#include "math.h"

class Sharp{
  public:
    Sharp();
    void set_pin(int pin);
    float get_distance_cm();
    float get_distance_inch();
  private:
    int A_r;
    int _pin;
    float d;
};

#endif
