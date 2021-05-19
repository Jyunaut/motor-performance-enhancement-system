#include "HelperFunctions.h"
#include "Settings.h"
#include "PID.h"

float desPos           = 0;
float curPos           = 0;
float rpm              = 0;
float motDutyCycle     = 0;
float motDutyCycleComp = 0;
float motPWMFreq       = 490;
float vvcDutyCycle     = 0;
float vvcPWMFreq       = 0;
float voltage          = 0;

PID pid(&curPos, &motDutyCycle, &desPos, 2, 0.005, 0, 20);

void WriteToPins()
{
    if (motDutyCycle > 0) {
        pwmWrite(PIN_MOT_PWMA, abs(motDutyCycle));
        pwmWrite(PIN_MOT_PWMB, 0);
    }
    else if (motDutyCycle < 0) {
        pwmWrite(PIN_MOT_PWMA, 0);
        pwmWrite(PIN_MOT_PWMB, abs(motDutyCycle));
    }
}

void PrintStuff()
{
    Serial.print("P: ");
    Serial.print(pid.GetP());
    Serial.print("\t\t");
    Serial.print("I: ");
    Serial.print(pid.GetI());
    Serial.print("\t\t");
    Serial.print("D: ");
    Serial.print(pid.GetD());
    Serial.print("\t\t");
    Serial.print("Duty Cycle %: ");
    Serial.println(motDutyCycle * 100 / 255);
}

void setup()
{
    Serial.begin(baudrate);

    PinInit();
    InitTimersSafe();
    SetPinFrequencySafe(PIN_MOT_PWMA, motPWMFreq);
    SetPinFrequencySafe(PIN_MOT_PWMB, motPWMFreq);

    prevStateA = digitalRead(PIN_MOT_ENCA);

    delay(1000);
}

void loop()
{
    curPos = GetEncoderPulseCount();

    desPos = map(HandleUserInput(), 0, 360, 0, motCPR);
    
    // Calculate motor PWM duty cycle using PID
    pid.CalculateDutyCycle();

    // Output to Motor
    WriteToPins();

    PrintStuff();
}