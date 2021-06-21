#include "HelperFunctions.h"

float MillisToSec(float length)
{
    return length / (float)1000;
}

void UpdateEncoder()
{
    int stateA = digitalRead(PIN_MOT_ENCA);
    int stateB = digitalRead(PIN_MOT_ENCB);
    if (stateA != prevStateA) {
        if (stateB != stateA) {
            encPulseCount++;
        } else {
            encPulseCount--;
        }
    }
    prevStateA = stateA;
}

void IncrementMeasPWM()
{
    measPWMPulseCount++;
}

void ResetMeasPWM()
{
    measPWMPulseCount = 0;
}

float GetRPM(float pulseCount)
{
 return pulseCount * 60 / ENC_CPR / MillisToSec(samplingRate);
}

float GetEncoderPulseCount()
{
    return encPulseCount;
}

int HandleUserInput()
{
    String input = "";

    while(Serial.available()) {
        delay(5);
        char c = Serial.read();
        input += c;
    }

    if (input.length() > 0) {
        return input.toInt();
    }
}

void PinInit()
{
    pinMode(PIN_VVC_A, OUTPUT);
    pinMode(PIN_VVC_B, OUTPUT);
    pinMode(PIN_MOT_ENCA, INPUT_PULLUP);
    pinMode(PIN_MOT_ENCB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_MOT_ENCA), UpdateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_MOT_ENCB), UpdateEncoder, CHANGE);
}
