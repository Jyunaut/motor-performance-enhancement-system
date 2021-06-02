#include "Serial.h"

void EXCEL_StartSerial()
{
    Serial.println("LABEL,Time,Angle");
}

void EXCEL_HandleSerial(float angle)
{
    Serial.print("DATA,TIME,");
    Serial.println(angle);
}