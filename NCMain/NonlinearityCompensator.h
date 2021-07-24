#ifndef NONLINEARITY_COMPENSATOR_H
#define NONLINEARITY_COMPENSATOR_H

#include "Arduino.h"
#include "EEPROM.h"
#include "Wire.h"
#include "CustomPWM.h"
#include "PID.h"
#include "Properties.h"

#define PIN_PWMA        10
#define PIN_PWMB        9
#define PIN_ENCA        3
#define PIN_ENCB        2
#define I2C_ADDRESS     7
#define SERIAL_BAUDRATE 9600

inline int currentPosition        = 0;
inline int desiredPosition        = 0;
inline int pwmFrequency           = 0;
inline float currentPositionCPR   = 0;
inline float desiredPositionCPR   = 0;
inline char dutyCycleSign         = 0;
inline float dutyCycle            = 0;
inline float dutyCycleCompensated = 0;

static PID pid(&currentPositionCPR, &dutyCycle, &desiredPositionCPR, K_P, K_I, K_D, SAMPLE_TIME);

void StartupSequence();
// This exists in order to call NC::Compensate for I2C
void Compensate();
int HandleSerialInput();
void HandleI2CInput(int numBytes);

class NC
{
public:
    NC() { compensationEnabled = false; }
    static void Initialize();
    static void WriteToMotorDAQ();
    static void WriteToMotor();
    static void EnableCompensation();
    static void DisableCompensation();
    static void Compensate();
    static void SaveAngleToStorage();
    static void ReadAngleFromStorage();
    static void UpdateEncoder();
    static long GetEncoderPulseCount();
    static void ResetEncoderPulseCount();
    static void SaveToTable(int index, int frequency, int dutyCycle);
    static int GetTablePWMFrequency(int desiredDutyCycle);
    static int GetTableCompensatedDutyCycle(int desiredDutyCycle);
    static void SaveTableToStorage();
private:
    inline volatile static long encPulseCount;
    inline static bool compensationEnabled;
    inline static int table[101][2] = {
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    40		,	26},
    {    0		,	0},
    {    40		,	28},
    {    40		,	30},
    {    0		,	0},
    {    40		,	32},
    {    0		,	0},
    {    40		,	34},
    {    40		,	38},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		35},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		40},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		45},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		50},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		55},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		60},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		65},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		70},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		75},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		80},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		85},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		90},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		95},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    8000	,		100},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0},
    {    0		,	0}};
};

#endif
