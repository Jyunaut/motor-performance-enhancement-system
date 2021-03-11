#include <CustomPWM.h>

#define PIN_PWM     10
#define PIN_INA     12
#define PIN_INB     13
#define PIN_ENCODER 3

volatile long encoderPulseCount = 0;
unsigned long timer = 0;
int rpm          = 0;
int dutyCycle    = 0;
int frequencyPWM = 0;

enum OutputMode { Arduino, Excel, TEST };

//-------------------------------------------------------------------
//                        Motor Parameters
//-------------------------------------------------------------------

#define ENCODERCPR 244.8

//-------------------------------------------------------------------
//                           Test Modes
//-------------------------------------------------------------------

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
bool doReverse = false;

// Serial Output to Excel -> mode = Excel
// Serial Output to Arduino Serial Monitor -> mode = Arduino
OutputMode mode = TEST;

// Duty Cycle from 0 to 100%
int targetDutyCycle = 100;

// Time interval between measurements (ms)
int delayTime = 1000;

//-------------------------------------------------------------------
//                        Helper Functions
//-------------------------------------------------------------------

float MillisToSec(float length)
{
    return length / 1000;
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
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), IncrementEncoder, CHANGE);
}

void WriteToPins()
{
    digitalWrite(PIN_INA, HIGH);
    digitalWrite(PIN_INB, LOW);
    analogWrite(PIN_PWM, dutyCycle);
}

void setup()
{
    Serial.begin(9600);
    
    dutyCycle = doReverse ? targetDutyCycle : 0;
    dutyCycle = map(dutyCycle, 0, 100, 0, 255);

    PinInit();
    EncoderInit();

    InitTimersSafe();
    SetPinFrequencySafe(PIN_PWM, frequencyPWM);

    delay(5000);
}

void loop()
{
    if (millis() - timer < delayTime)
        return;
    if (dutyCycle <= 0 && doReverse
        || dutyCycle >= targetDutyCycle && !doReverse)
        return;

    timer = millis();
    rpm = (float)(encoderPulseCount * 60 / ENCODERCPR / MillisToSec(delayTime) / 2);
    dutyCycle = doReverse ? dutyCycle-1 : dutyCycle+1;

    WriteToPins();
    HandleSerialData();
    ResetEncoder();
}

void IncrementEncoder()
{
    encoderPulseCount++;
}

void ResetEncoder()
{
    encoderPulseCount = 0;
}

void HandleSerialData()
{
    switch (mode) {
        case Arduino:
            SendSerialData_Arduino();
            break;
        case Excel:
            SendSerialData_Excel();
            break;
        case TEST:
            SendSerialData_TEST();
            break;
        default:
            return;
    }
}

void SendSerialData_Arduino()
{
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.print("\t");
    Serial.print(encoderPulseCount);
    Serial.print(" pulse / ");
    Serial.print(ENCODERCPR);
    Serial.print(" pulse per rotation * 60 seconds = ");
    Serial.print(rpm);
    Serial.print(" RPM");
    Serial.print(rpm);
    Serial.println(dutyCycle);
}

void SendSerialData_Excel()
{
    Serial.print("DATA,TIME,");
    Serial.print(dutyCycle);
    Serial.print(",");
    Serial.print(rpm);
    Serial.print(",");
    Serial.println(dutyCycle);
}

void SendSerialData_TEST()
{
    Serial.print("PWM Frequency: ");
    Serial.println(encoderPulseCount / 2);
    frequencyPWM++;
    SetPinFrequencySafe(PIN_PWM, frequencyPWM);
}