#ifndef LOOKUP_H_
#define LOOKUP_H_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Arduino.h"

class Lookup
{
    public:
        Lookup();
        void AdjustParams(int rpm);
    private:
        int params[3][300];
};

#endif