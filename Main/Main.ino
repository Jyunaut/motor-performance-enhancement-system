#include <EEPROM.h>
#include "HelperFunctions.h"
#include "Settings.h"
#include "PID.h"
#include "Table.h"

const float ANALOG_TO_PERCENT = 100 / (float)255;
const bool COMPENSATION = true;
const int TOLERANCE = 1; // In Degrees

//0.4,0,0
const float K_P = 0.4, K_I = 0, K_D = 0;

// Position and speed values
int   desPos           = 0;
float desPosCPR        = 0;
int   curPos           = 0;
float curPosCPR        = 0;
float rpm              = 0;

// Compensation parameters and duty cycle
float motDutyCycle     = 0;
float motVoltage       = 0;
int   motPWMFreq       = 0;
int   motDutyCycleComp = 0;
int test = 25;
PID pid(&curPosCPR, &motDutyCycle, &desPosCPR, K_P, K_I, K_D, samplingRate);

void WriteToPins()
{
    // Adjust Voltage
    if (motVoltage <= 2.5) {
        digitalWrite(PIN_VVC_A, LOW);
        digitalWrite(PIN_VVC_B, HIGH);
    } else {
        digitalWrite(PIN_VVC_A, HIGH);
        digitalWrite(PIN_VVC_B, LOW);
    }

    if (COMPENSATION) {
        // Adjust PWM Frequency
        SetPinFrequencySafe(PIN_MOT_PWMA, motPWMFreq);

        // Adjust PWM Duty Cycle
        if (motDutyCycle > 0) {
            pwmWrite(PIN_MOT_PWMA, map(motDutyCycleComp, 0, 100, 0, 255));
            pwmWrite(PIN_MOT_PWMB, 0);
        }
        else if (motDutyCycle < 0) {
            pwmWrite(PIN_MOT_PWMA, 0);
            pwmWrite(PIN_MOT_PWMB, map(motDutyCycleComp, 0, 100, 0, 255));
        }
    } else {
        // Adjust PWM Frequency
        SetPinFrequencySafe(PIN_MOT_PWMA, 16000);

        // Adjust PWM Duty Cycle
        if (motDutyCycle > 0) {
            pwmWrite(PIN_MOT_PWMA, abs(motDutyCycle));
            pwmWrite(PIN_MOT_PWMB, 0);
        }
        else if (motDutyCycle < 0) {
            pwmWrite(PIN_MOT_PWMA, 0);
            pwmWrite(PIN_MOT_PWMB, abs(motDutyCycle));
        }
    }

   if (abs(desPos - curPos) <= TOLERANCE) {
       pwmWrite(PIN_MOT_PWMA, 0);
       pwmWrite(PIN_MOT_PWMB, 0);
   }
}

void StoreNewAngle()
{
    EEPROM.update(0, curPosCPR);
}

void Compensate()
{
    motVoltage       = params[(int)roundf(abs(motDutyCycle * ANALOG_TO_PERCENT))][0];
    motPWMFreq       = params[(int)roundf(abs(motDutyCycle * ANALOG_TO_PERCENT))][1];
    motDutyCycleComp = params[(int)roundf(abs(motDutyCycle * ANALOG_TO_PERCENT))][2];
}

void PrintStuff()
{
// //    Serial.print("Time: ");          Serial.print(MillisToSec(millis()));            Serial.print("\t\t");
//     Serial.print("Desired Angle: "); Serial.print(desPos);                           Serial.print("\t\t");
//     Serial.print("Angle: ");         Serial.print(curPos);                           Serial.print("\t\t");
//     Serial.print("Duty Cycle %: ");  Serial.print(motDutyCycle * ANALOG_TO_PERCENT); Serial.print("\t\t");
// //    Serial.print("Voltage: ");       Serial.print(motVoltage);                       Serial.print("\t\t");
//     Serial.print("PWM Frequency: "); Serial.print(motPWMFreq);                       Serial.print("\t\t");
//     Serial.print("Compensated Duty Cycle %: "); Serial.println(motDutyCycleComp);
   Serial.print(desPos);
   Serial.print("\t");
   Serial.println(curPos);
}

void setup()
{
    Serial.begin(BAUDRATE);

    PinInit();
    InitTimersSafe();
    SetPinFrequencySafe(PIN_MOT_PWMA, motPWMFreq);

    encPulseCount = EEPROM.read(0);

    delay(1000);
}
unsigned long timer = 0;
void loop()
{
    // Get current angle
    curPosCPR = GetEncoderPulseCount();
    curPos = map(curPosCPR, 0, ENC_CPR, 0, 360);

    // Get desired angle
    desPos = HandleUserInput();
    desPosCPR = desPos * ENC_CPR / 360;

    // Calculate motor PWM duty cycle using PID
    pid.CalculateDutyCycle();

    // Apply Compensation system
    if (COMPENSATION) Compensate();

    // Output to Motor
    WriteToPins();
    //pwmWrite(PIN_MOT_PWMA, map(test, 0, 100, 0, 255));
    
    // Save current angle to EEPROM
    StoreNewAngle();
    PrintStuff();
    // if (millis() - timer >= samplingRate) {
    //     Serial.println(GetRPM(encPulseCount));
    //     encPulseCount = 0;
    // }
    // if (millis() - timer < samplingRate)
    //     return;
    // timer = millis();
}
