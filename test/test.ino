#include <CustomPWM.h>
#include "SendSerial.h"
#include "table.h"

#define PIN_PWM      10
#define PIN_VPWM     9
#define PIN_INA      11
#define PIN_INB      12
#define PIN_ENCODER  3
#define PIN_PWM_MEAS 2
#define PIN_POT      A5

volatile long encoderPulseCount = 0;
volatile long PWMMeasPulseCount = 0;

unsigned long timer1 = 0;
unsigned long timer2 = 0;
int rpm          = 0;
int dutyCycle    = 0;
int dutyCycle_c  = 0;
int dutyCycleMap = 0;
bool doneTest = false;
bool firstMeas = true;
String testName = " ";

int voltage      = 0; // in mV
int vPWM         = 62500; // in Hz
int vDutyCycle = 0;
int frequencyPWM = 15;

enum MotorType  { Pololu,  DFRobot };
enum OutputMode { Arduino, Excel, Test };
enum TestMode   { Incremental, Fixed, Volt };

/* ==================================================================
 *                         Motor Parameters
 */

// Max speed in RPM
const int MAX_RPM = 300;

// Motor Encoder Counts Per Revolution
const float ENCODER_CPR = 224.4;

/* ==================================================================
 *                            Test Modes
 */

// Default: 9600
const int BAUDRATE = 9600;

// Different encoder calculations for Pololu and Digikey
MotorType motor = DFRobot;

// Excel: Serial Output to Excel
// Arduino: Serial Output to Arduino Serial Monitor
// Test: Serial Output to Arduino for debugging purposes
OutputMode outputMode = Test;

// Incremental: Increases/Decreases duty cycle by a fixed amount from dutyCycleA to dutyCycleB
// Fixed: Maintains a requested duty cycle from the tester
TestMode testMode = Volt;

// Detect RISING, FALLING or both (CHANGE) edges
int edgeCountMode = CHANGE;

// Enable this option to let measurement timing slow down for lower RPM
bool variableDelayTime = false;

// Time interval between measurements (ms). Only works if variableDelayTime is false
int delayTime = 1000;

// Note: Turn on continuous measurement while testing code
bool continuousMeasurement = true;

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
bool doReverse = false;

// Duty Cycle from A to B
int dutyCycleA = 0;
int dutyCycleB = 100;

// How many times to sample per duty cycle increment
int samplesPerInc = 1;

// Increment Duty Cycle by a certain percentage
int dutyCycleStep = 1;

/* ==================================================================
 *                        Helper Functions
 */

float MillisToSec(float length)
{
    return length / (float)1000;
}

void IncrementEncoder()
{
    encoderPulseCount++;
}

void ResetEncoder()
{
    encoderPulseCount = 0;
}

void IncrementPWMMeasurement()
{
    PWMMeasPulseCount++;
}

void ResetPWMMeasurement()
{
    PWMMeasPulseCount = 0;
}

float GetRPM()
{ 
    switch (motor)
    {
        case DFRobot:
            if (edgeCountMode == RISING) return encoderPulseCount * 60 /  ENCODER_CPR      / MillisToSec(delayTime) * samplesPerInc;
            if (edgeCountMode == CHANGE) return encoderPulseCount * 60 / (ENCODER_CPR * 2) / MillisToSec(delayTime) * samplesPerInc;
            break;
        case Pololu:
            if (edgeCountMode == RISING) return encoderPulseCount * 60 / (ENCODER_CPR / 4) / MillisToSec(delayTime) * samplesPerInc;
            if (edgeCountMode == CHANGE) return encoderPulseCount * 60 / (ENCODER_CPR / 2) / MillisToSec(delayTime) * samplesPerInc;
            break;
        default:
            return 0;
    }
}

int GetDutyCycle()
{
    return map(analogRead(PIN_POT), 0, 1023, 0, 100);
}

int AdjustDelayTime()
{
    if      (rpm <= 0)              delayTime = 1000;
    //else if (rpm <= MAX_RPM * 0.10) delayTime = 8000; // 30 rpm
    else if (rpm <= MAX_RPM * 0.15) delayTime = 3000; // 45 rpm
    else if (rpm <= MAX_RPM * 0.25) delayTime = 2000;
    else                            delayTime = 1000;
}

/* ==================================================================
 *                           Main Program
 */

void PinInit()
{
    pinMode(PIN_INA, OUTPUT);
    pinMode(PIN_INB, OUTPUT);
    pinMode(PIN_ENCODER, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), IncrementEncoder, edgeCountMode);
    attachInterrupt(digitalPinToInterrupt(PIN_PWM_MEAS), IncrementPWMMeasurement, RISING);
}

void WriteToPins()
{
    digitalWrite(PIN_INA, HIGH);
    digitalWrite(PIN_INB, LOW);
    pwmWrite(PIN_PWM, dutyCycleMap);
    pwmWrite(PIN_VPWM, map(vDutyCycle, 0, 100, 0, 255));
}

void HandleSerialData()
{

    if      (outputMode == Arduino) SendSerialData_Arduino();
    else if (outputMode == Excel)   SendSerialData_Excel();
    else if (outputMode == Test)    SendSerialData_Test();
}

void setup()
{
    Serial.begin(BAUDRATE);

    if (!doReverse)
        testName = String((String)((float)voltage / 1000) + " V " + (String)frequencyPWM + " Hz " + dutyCycleA + " to " + dutyCycleB + "%");
    else if (doReverse)
        testName = String((String)((float)voltage / 1000) + " V " + (String)frequencyPWM + " Hz " + dutyCycleB + " to " + dutyCycleA + "%");
    
    dutyCycle = doReverse ? dutyCycleB : dutyCycleA;
    dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);

    PinInit();
    InitTimersSafe();
    SetPinFrequencySafe(PIN_PWM, frequencyPWM);

    if (outputMode == Excel) {
        Serial.print("LABEL,Time,Duty Cycle,Desired RPM,");
        Serial.println(testName);
    }
    delay(1000);
}

void loop()
{
    if (millis() - timer2 > delayTime / samplesPerInc) {
        rpm = GetRPM();
        if (continuousMeasurement) {
            HandleSerialData();
        }
        else if (!continuousMeasurement && !doneTest) {
            if (dutyCycle >= dutyCycleA && doReverse
                || dutyCycle <= dutyCycleB && !doReverse) {
                HandleSerialData();
                if (dutyCycle == dutyCycleA && doReverse
                    || dutyCycle == dutyCycleB && !doReverse) {
                        doneTest = true;
                }
            }
        }
        timer2 = millis();
        ResetEncoder();
        ResetPWMMeasurement();
    }
    if (millis() - timer1 < delayTime)
        return;

    timer1 = millis();

    if (variableDelayTime) {
        AdjustDelayTime();
    }

    switch (testMode)
    {
        case Incremental:
            if (dutyCycle != dutyCycleA && doReverse
                || dutyCycle != dutyCycleB && !doReverse) {
                if (!firstMeas) {
                    dutyCycle = doReverse ? dutyCycle-dutyCycleStep : dutyCycle+dutyCycleStep;
                    frequencyPWM = params[dutyCycle][1];
                    SetPinFrequencySafe(PIN_PWM, frequencyPWM);
                    dutyCycle_c = params[dutyCycle][2];
                    dutyCycleMap = map(dutyCycle_c, 0, 100, 0, 255);
                    WriteToPins();
                }
                firstMeas = false;
            }
            break;
        case Fixed:
            dutyCycle = GetDutyCycle();
            dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);
            WriteToPins();
            break;
        case Volt: // Motor duty cycle and frequency is fixed
            dutyCycle    = 7; // motor duty cycle %
            frequencyPWM = 7; // motor frequency in Hz

            // Fix motor PWM duty cycle
            pwmWrite(PIN_PWM, map(dutyCycle, 0, 100, 0, 255));

            // Fix motor frequency
            SetPinFrequencySafe(PIN_PWM, frequencyPWM);

            // Read potentiometer input for the VVC %
            vDutyCycle = GetDutyCycle();
            
            // Set VVC duty cycle
            pwmWrite(PIN_VPWM, map(vDutyCycle, 0, 100, 0, 255));
            break;
        default:
            Serial.println("Undefined Test Mode");
            break;
    }
}
