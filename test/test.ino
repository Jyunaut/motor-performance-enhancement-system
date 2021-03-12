#include <CustomPWM.h>

#define PIN_PWM     10
#define PIN_INA     11
#define PIN_INB     12
#define PIN_ENCODER 3

volatile long encoderPulseCount = 0;
unsigned long timer = 0;
int rpm          = 0;
int dutyCycle    = 0;
int dutyCycleMap = 0;
int frequencyPWM = 2000;
int input = 0;

enum OutputMode { Arduino, Excel, TEST };

//-------------------------------------------------------------------
//                        Motor Parameters
//-------------------------------------------------------------------

// Free-run speed in RPM
#define MAX_RPM 500

// Motor Encoder Counts Per Revolution
#define ENCODER_CPR 979.62

//-------------------------------------------------------------------
//                           Test Modes
//-------------------------------------------------------------------

// Default: 9600
#define BAUDRATE 9600

// Serial Output to Excel -> mode = Excel
// Serial Output to Arduino Serial Monitor -> mode = Arduino
OutputMode mode = Excel;

// Detect RISING, FALLING or both (CHANGE) edges
int edgeCountMode = CHANGE;

// Note: Turn on continuous measurement while testing code
bool continuousMeasurement = false;

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
bool doReverse = false;

// Duty Cycle from A to B
int dutyCycleA = 0;
int dutyCycleB = 100;

// Increment Duty Cycle by a certain percentage
int dutyCycleStep = 1;

// Time interval between measurements (ms)
int delayTime = 500;

//-------------------------------------------------------------------
//                        Helper Functions
//-------------------------------------------------------------------

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

float GetRPM()
{
    if (edgeCountMode == CHANGE) return encoderPulseCount * 60 / (ENCODER_CPR / 2) / MillisToSec(delayTime);
    if (edgeCountMode == RISING) return encoderPulseCount * 60 / (ENCODER_CPR / 4) / MillisToSec(delayTime);
}

//-------------------------------------------------------------------
//                   Initialization Functions
//-------------------------------------------------------------------

void PinInit()
{
    pinMode(PIN_INA, OUTPUT);
    pinMode(PIN_INB, OUTPUT);
    pinMode(PIN_ENCODER, INPUT_PULLUP);
}

void EncoderInit()
{
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), IncrementEncoder, edgeCountMode);
}

//-------------------------------------------------------------------
//                          Main Program
//-------------------------------------------------------------------

void WriteToPins()
{
    digitalWrite(PIN_INA, HIGH);
    digitalWrite(PIN_INB, LOW);
    analogWrite(PIN_PWM, dutyCycleMap);
}

void HandleSerialData()
{
    if      (mode == Arduino) SendSerialData_Arduino();
    else if (mode == Excel)   SendSerialData_Excel();
    else if (mode == TEST)    SendSerialData_TEST();
}

void SendSerialData_Arduino()
{
    Serial.print("PWM Frequency: ");
    Serial.print(frequencyPWM);
    Serial.print(" Hz \t\t");
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.print("% \t\t");
    Serial.print("Motor Speed: ");
    Serial.print(rpm);
    Serial.print(" RPM \t\t");
    Serial.print("Desired Motor Speed: ");
    Serial.print((int)(dutyCycle / (float)100 * MAX_RPM));
    Serial.println(" RPM");
}

void SendSerialData_Excel()
{
    Serial.print("DATA,TIME,");
    Serial.print(dutyCycle);
    Serial.print(",");
    Serial.print(rpm);
    Serial.print(",");
    Serial.println((int)(dutyCycle / (float)100 * MAX_RPM));
}

void SendSerialData_TEST()
{
    // Serial.print("RPM: ");
    // Serial.println(rpm);
    // frequencyPWM++;
    // SetPinFrequencySafe(PIN_PWM, frequencyPWM);
}

void setup()
{
    Serial.begin(BAUDRATE);
    
    dutyCycle = doReverse ? dutyCycleB : dutyCycleA;
    dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);

    PinInit();
    EncoderInit();

    InitTimersSafe();
    SetPinFrequencySafe(PIN_PWM, frequencyPWM);

    if (mode == Excel) {
        Serial.println("LABEL,Time,Duty Cycle,Measured RPM,Desired RPM");
    }

    delay(1000);
}

void loop()
{
    if (millis() - timer < delayTime)
        return;

    timer = millis();
    rpm = GetRPM();
    ResetEncoder();

    if (continuousMeasurement) {
        HandleSerialData();
    }

    if (dutyCycle < dutyCycleA && doReverse
        || dutyCycle > dutyCycleB && !doReverse)
        return;

    if (!(dutyCycle >= dutyCycleB)
        || !(dutyCycle <= dutyCycleA)) {
        if (!continuousMeasurement) {
            HandleSerialData();
        }
        dutyCycle = doReverse ? dutyCycle-dutyCycleStep : dutyCycle+dutyCycleStep;
        dutyCycleMap = map(dutyCycle, 0, 100, 0, 255);
    }
    WriteToPins();
}