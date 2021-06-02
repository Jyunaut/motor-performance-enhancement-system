#include "EEPROM.h"
#include "HelperFunctions.h"
#include "Settings.h"
#include "Serial.h"
#include "PID.h"
#include "Table.h"

const float analogToPercent = 100 / (float)255;

// Position and speed values
float desPos           = 0;
float curPos           = 0;
float rpm              = 0;

// Variable voltage circuit values
int   vvcDutyCycle     = 0;
int   vvcPWMFreq       = 0;

// Compensation parameters and duty cycle
float motDutyCycle     = 0;
float motVoltage       = 0;
int   motPWMFreq       = 0;
int   motDutyCycleComp = 0;

PID pid(&curPos, &motDutyCycle, &desPos, 0.85, 0, 0, 20);

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
    Serial.print("Time: ");          Serial.print(MillisToSec(millis()));          Serial.print("\t\t");
    Serial.print("Desired Angle: "); Serial.print(map(desPos, 0, motCPR, 0, 360)); Serial.print("\t\t");
    Serial.print("Angle: ");         Serial.print(map(curPos, 0, motCPR, 0, 360)); Serial.print("\t\t");
    Serial.print("Duty Cycle %: ");  Serial.print(motDutyCycle * analogToPercent); Serial.print("\t\t");
    Serial.print("Voltage: ");       Serial.print(motVoltage);                     Serial.print("\t\t");
    Serial.print("PWM Frequency: "); Serial.print(motPWMFreq);                     Serial.print("\t\t");
    Serial.print("Compensated Duty Cycle %: "); Serial.println(motDutyCycleComp);
}

void StoreNewAngle()
{
    EEPROM.update(0, curPos);
}

void Compensate()
{
    motVoltage       = params[(int)abs(motDutyCycle * analogToPercent)][0];
    motPWMFreq       = params[(int)abs(motDutyCycle * analogToPercent)][1];
    motDutyCycleComp = params[(int)abs(motDutyCycle * analogToPercent)][2];
}

void setup()
{
    Serial.begin(baudrate);

    PinInit();
    InitTimersSafe();
    SetPinFrequencySafe(PIN_MOT_PWMA, motPWMFreq);
    SetPinFrequencySafe(PIN_MOT_PWMB, motPWMFreq);

    encPulseCount = EEPROM.read(0);

    delay(1000);
}

void loop()
{
    // Get current and desired angles
    curPos = GetEncoderPulseCount();
    desPos = map(HandleUserInput(), 0, 360, 0, motCPR);

    // Calculate motor PWM duty cycle using PID
    pid.CalculateDutyCycle();

    // Apply Compensation system
    Compensate();

    // Output to Motor
    WriteToPins();

    // Save current angle to EEPROM
    StoreNewAngle();
    PrintStuff();
}