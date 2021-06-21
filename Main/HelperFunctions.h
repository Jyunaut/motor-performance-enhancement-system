#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

#include <Arduino.h>
#include "CustomPWM.h"
#include "Settings.h"

inline int prevStateA;
inline volatile long encPulseCount     = 0;
inline volatile long measPWMPulseCount = 0;
inline float samplingRate = 20;

void PinInit();
void ResetMeasPWM();
int HandleUserInput();
float MillisToSec(float length);
float GetRPM(float pulseCount);
float GetEncoderPulseCount();
int GetDutyCycle();

#endif
