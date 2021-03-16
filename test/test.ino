#include <CustomPWM.h>
#include "SendSerial.h"
#include "Lookup.h"

#define PIN_PWM          10
#define PIN_INA          11
#define PIN_INB          12
#define PIN_ENCODER      3
#define PIN_PWM_MEAS     2
#define PIN_CURRENT_MEAS 5

volatile long encoderPulseCount = 0;
volatile long PWMMeasPulseCount = 0;

unsigned long timer = 0;
float current    = 0;
int rpm          = 0;
int dutyCycle    = 0;
int dutyCycleMap = 0;
bool doneTest = false;
bool firstMeas = true;
String testName = " ";

float voltage    = 6;
int frequencyPWM = 16000;

enum MotorType  { Pololu,  DFRobot };
enum OutputMode { Arduino, Excel };

/* ==================================================================
 *                         Motor Parameters
 */

// Max speed in RPM
const int MAX_RPM = 300;

// Motor Encoder Counts Per Revolution
const int ENCODER_CPR = 224.4;

/* ==================================================================
 *                            Test Modes
 */

// Default: 9600
const int BAUDRATE = 9600;

// Different encoder calculations for Pololu and Digikey
MotorType motor = DFRobot;

// Serial Output to Excel -> mode = Excel
// Serial Output to Arduino Serial Monitor -> mode = Arduino
OutputMode mode = Excel;

// Detect RISING, FALLING or both (CHANGE) edges
int edgeCountMode = RISING;

// Enable this option to let measurement timing slow down for lower RPM
bool variableDelayTime = true;

// Time interval between measurements (ms). Only works if variableDelayTime is true
int delayTime = 1000;

// Note: Turn on continuous measurement while testing code
bool continuousMeasurement = false;

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
bool doReverse = true;

// Duty Cycle from A to B
int dutyCycleA = 0;
int dutyCycleB = 100;

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
            if (edgeCountMode == RISING) return encoderPulseCount * 60 /  ENCODER_CPR      / MillisToSec(delayTime);
            if (edgeCountMode == CHANGE) return encoderPulseCount * 60 / (ENCODER_CPR / 2) / MillisToSec(delayTime);
            break;
        case Pololu:
            if (edgeCountMode == RISING) return encoderPulseCount * 60 / (ENCODER_CPR / 4) / MillisToSec(delayTime);
            if (edgeCountMode == CHANGE) return encoderPulseCount * 60 / (ENCODER_CPR / 2) / MillisToSec(delayTime);
            break;
        default:
            return 0;
    }
}

float GetCurrent()
{
    return analogRead(PIN_CURRENT_MEAS) / (float)140 * 1000;
}

int AdjustDelayTime()
{
    if      (rpm <= 0)              delayTime = 500;
    else if (rpm <= MAX_RPM * 0.25) delayTime = 3000;
    else if (rpm <= MAX_RPM * 0.5 ) delayTime = 2000;
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
}

void HandleSerialData()
{
    if      (mode == Arduino) SendSerialData_Arduino();
    else if (mode == Excel)   SendSerialData_Excel();
}

void setup()
{
    Serial.begin(BAUDRATE);

    if (!doReverse)
        testName = String((String)voltage + " V " + (String)frequencyPWM + " Hz " + dutyCycleA + " to " + dutyCycleB + "%");
    else if (doReverse)
        testName = String((String)voltage + " V " + (String)frequencyPWM + " Hz " + dutyCycleB + " to " + dutyCycleA + "%");
    
    dutyCycle = doReverse ? dutyCycleB : dutyCycleA;
    dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);

    PinInit();
    InitTimersSafe();
    SetPinFrequencySafe(PIN_PWM, frequencyPWM);

    if (mode == Excel) {
        Serial.print("LABEL,Time,Duty Cycle,Desired RPM,");
        Serial.println(testName);
    }
    delay(1000);
}

void loop()
{
    if (millis() - timer < delayTime)
        return;

    timer = millis();
    WriteToPins();
    rpm = GetRPM();
    current = GetCurrent();

    if (variableDelayTime) {
        AdjustDelayTime();
    }

    if (dutyCycle != dutyCycleA && doReverse
        || dutyCycle != dutyCycleB && !doReverse) {
        if (!firstMeas) {
            dutyCycle = doReverse ? dutyCycle-dutyCycleStep : dutyCycle+dutyCycleStep;
            dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);
        }
        firstMeas = false;
    }

    if (continuousMeasurement){
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

    ResetEncoder();
    ResetPWMMeasurement();
}