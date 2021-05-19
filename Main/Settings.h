#ifndef SETTINGS_H
#define SETTINGS_H

#define PIN_MOT_PWMA 10
#define PIN_MOT_PWMB 9
#define PIN_MOT_ENCA 3
#define PIN_MOT_ENCB 2
#define PIN_VVC_PWM  6
#define PIN_POT      A5

enum MotorType  { Pololu, DFRobot };
enum OutputMode { Arduino, Excel, Test };
enum TestMode   { Incremental, Fixed, Volt };

/* ==================================================================
 *                         Motor Parameters
 */

// Max speed in RPM
inline int motMaxRPM = 300;

// Motor Encoder Counts Per Revolution
inline float motCPR = 224.4 * 2;

/* ==================================================================
 *                            Test Modes
 */

// Default: 9600
inline int baudrate = 9600;

// Excel: Serial Output to Excel
// Arduino: Serial Output to Arduino Serial Monitor
// Test: Serial Output to Arduino for debugging purposes
inline OutputMode outputMode;

// Incremental: Increases/Decreases duty cycle by a fixed amount from dutyCycleA to dutyCycleB
// Fixed: Maintains a requested duty cycle from the tester
inline TestMode testMode;

// Detect RISING, FALLING or both (CHANGE) edges
inline int edgeCountMode = RISING;

// Enable this option to let measurement timing slow down for lower RPM
inline bool variableDelayTime = false;

// Time interval between measurements (ms). Only works if variableDelayTime is false
inline int delayTime = 1000;

// Note: Turn on continuous measurement while testing code
inline bool continuousMeasurement = true;

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
inline bool doReverse = false;

// Duty Cycle from A to B
inline int dutyCycleA = 0;
inline int dutyCycleB = 100;

// How many times to sample per duty cycle increment
inline int samplesPerInc = 1;

// Increment Duty Cycle by a certain percentage
inline int dutyCycleStep = 1;

#endif