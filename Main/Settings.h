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
extern int motMaxRPM;

// Motor Encoder Counts Per Revolution
extern float motCPR;

/* ==================================================================
 *                            Test Modes
 */

// Default: 9600
extern int baudrate;

// Different encoder calculations for Pololu and Digikey
extern MotorType motor;

// Excel: Serial Output to Excel
// Arduino: Serial Output to Arduino Serial Monitor
// Test: Serial Output to Arduino for debugging purposes
extern OutputMode outputMode;

// Incremental: Increases/Decreases duty cycle by a fixed amount from dutyCycleA to dutyCycleB
// Fixed: Maintains a requested duty cycle from the tester
extern TestMode testMode;

// Detect RISING, FALLING or both (CHANGE) edges
extern int edgeCountMode;

// Enable this option to let measurement timing slow down for lower RPM
extern bool variableDelayTime;

// Time interval between measurements (ms). Only works if variableDelayTime is false
extern int delayTime;

// Note: Turn on continuous measurement while testing code
extern bool continuousMeasurement;

// Duty Cycle 0 to 100 -> doReverse = false
// Duty Cycle 100 to 0 -> doReverse = true
extern bool doReverse;

// Duty Cycle from A to B
extern int dutyCycleA;
extern int dutyCycleB;

// How many times to sample per duty cycle increment
extern int samplesPerInc;

// Increment Duty Cycle by a certain percentage
extern int dutyCycleStep;

#endif