#include "HelperFunctions.h"
#include "Settings.h"

volatile long encPulseCount     = 0;
volatile long measPWMPulseCount = 0;
int prevStateA = 0;

float MillisToSec(float length)
{
    return length / (float)1000;
}

void UpdateEncoder()
{
    int stateA = digitalRead(PIN_MOT_ENCA);
    int stateB = digitalRead(PIN_MOT_ENCB);
    // Serial.print(stateA);
    // Serial.print("\t");
    // Serial.println(stateB);
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

float GetRPM()
{
    return encPulseCount * 60 / motCPR / MillisToSec(delayTime) * samplesPerInc;
}

float GetEncoderPulseCount()
{
    return encPulseCount;
}

int GetDutyCycle()
{
    return map(analogRead(PIN_POT), 0, 1023, 0, 100);
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
    pinMode(PIN_MOT_ENCA, INPUT_PULLUP);
    pinMode(PIN_MOT_ENCB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_MOT_ENCA), UpdateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_MOT_ENCB), UpdateEncoder, CHANGE);
}