#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

#include <CustomPWM.h>
#include <Arduino.h>

inline int prevStateA;
inline volatile long encPulseCount     = 0;
inline volatile long measPWMPulseCount = 0;

void PinInit();
void ResetMeasPWM();
int HandleUserInput();
float MillisToSec(float length);
float GetRPM();
float GetEncoderPulseCount();
int GetDutyCycle();

#endif