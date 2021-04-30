#ifndef _HELPER_FUNCTIONS_
#define _HELPER_FUNCTIONS_

#include <CustomPWM.h>
#include <Arduino.h>

void PinInit();
void ResetMeasPWM();
int HandleUserInput();
float MillisToSec(float length);
float GetRPM();
float GetEncoderPulseCount();
int GetDutyCycle();

#endif