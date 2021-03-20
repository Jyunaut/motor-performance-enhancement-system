#include "Arduino.h"
#include "Lookup.h"

int params[3][300];

class Lookup
{
    void Lookup::SetupTable()
    {
        std::ifstream table("Book1.csv");
        if (!table.is_open()) {
            Serial.println("Could not open file.");
        }
        
    }

    void Lookup::AdjustParams(int rpm)
    {
        
    }
};