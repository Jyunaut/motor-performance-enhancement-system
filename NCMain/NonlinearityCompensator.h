#ifndef NONLINEARITY_COMPENSATOR_H
#define NONLINEARITY_COMPENSATOR_H

#include "Arduino.h"
#include "EEPROM.h"
#include "Wire.h"
#include "CustomPWM.h"
#include "PID.h"
#include "Properties.h"

#define PIN_PWMA        9
#define PIN_PWMB        10
#define PIN_ENCA        2
#define PIN_ENCB        3
#define I2C_ADDRESS     7
#define SERIAL_BAUDRATE 9600

inline int currentPosition        = 0;
inline int desiredPosition        = 0;
inline int pwmFrequency           = 0;
inline int timeConstantPWM        = 0;
inline float currentPositionCPR   = 0;
inline float desiredPositionCPR   = 0;
inline char dutyCycleSign         = 0;
inline float dutyCycle            = 0;

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
private:
    inline volatile static long encPulseCount;
    inline static bool compensationEnabled;
    inline static int table[101][2]; //= {
    // {    40	        ,		    0       },
    // {    40	        ,		    0       },
    // {    40	        ,		    0       },
    // {    40	        ,		    0       },
    // {    40	        ,		    0       },
    // {    40	        ,		    13      },
    // {    40	        ,		    13      },
    // {    40	        ,		    13      },
    // {    40	        ,		    13      },
    // {    40	        ,		    13      },
    // {    40	        ,		    13      },
    // {    40	        ,		    14      },
    // {    40	        ,		    15      },
    // {    40	        ,		    15      },
    // {    40	        ,		    18      },
    // {    40	        ,		    19      },
    // {    40	        ,		    19      },
    // {    40	        ,		    20      },
    // {    40	        ,		    21      },
    // {    40	        ,		    22      },
    // {    40	        ,		    24      },
    // {    40	        ,		    25      },
    // {    40	        ,   	    26      },
    // {    8000       ,			30      },
    // {    8000       ,			31      },
    // {    8000       ,			32      },
    // {    8000       ,			33      },
    // {    8000       ,			34      },
    // {    8000       ,			35      },
    // {    8000       ,			36      },
    // {    8000       ,			37      },
    // {    8000       ,			38      },
    // {    8000       ,			39      },
    // {    8000       ,			40      },
    // {    8000       ,			41      },
    // {    8000       ,			43      },
    // {    8000       ,			44      },
    // {    8000       ,			45      },
    // {    8000       ,			46      },
    // {    8000       ,			47      },
    // {    8000       ,			48      },
    // {    8000       ,			49      },
    // {    8000       ,			50      },
    // {    8000       ,			51      },
    // {    8000       ,			52      },
    // {    8000       ,			53      },
    // {    8000       ,			54      },
    // {    8000       ,			55      },
    // {    8000       ,			56      },
    // {    8000       ,			57      },
    // {    8000       ,			58      },
    // {    8000       ,			59      },
    // {    8000       ,			60      },
    // {    8000       ,			61      },
    // {    8000       ,			63      },
    // {    8000       ,			64      },
    // {    8000       ,			65      },
    // {    8000       ,			66      },
    // {    8000       ,			67      },
    // {    8000       ,			68      },
    // {    8000       ,			69      },
    // {    8000       ,			70      },
    // {    8000       ,			71      },
    // {    8000       ,			72      },
    // {    8000       ,			73      },
    // {    8000       ,			74      },
    // {    8000       ,			75      },
    // {    8000       ,			76      },
    // {    8000       ,			77      },
    // {    8000       ,			78      },
    // {    8000       ,			79      },
    // {    8000       ,			80      },
    // {    8000       ,			81      },
    // {    8000       ,			83      },
    // {    8000       ,			84      },
    // {    8000       ,			85      },
    // {    8000       ,			86      },
    // {    8000       ,			87      },
    // {    8000       ,			88      },
    // {    8000       ,			89      },
    // {    8000       ,			90      },
    // {    8000       ,  		    91      },
    // {    8000       ,			92      },
    // {    8000       ,			93      },
    // {    8000       ,			94      },
    // {    8000       ,			95      },
    // {    8000       ,			96      },
    // {    8000       ,			97      },
    // {    8000       ,			98      },
    // {    8000       ,			98      },
    // {    8000       ,			99      },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     },
    // {    8000       ,			100     }};
};

#endif
