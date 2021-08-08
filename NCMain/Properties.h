#ifndef PROPERTIES_H
#define PROPERTIES_H

// Motor Parameters =======================================

// Motor Time Constant in milliseconds (ms)
// Note: Currently, a PWM frequency of 40 Hz was pretty good,
//       but the motor doesn't actually have a mechanical
//       time constant of 25 ms (it's actually around 125 ms for FIT0520).
//       This means that the lowest PWM Hz we can use is actually 8 Hz
//       which is way too low based on our observations.
//       This is something to improve for future work.
inline const float MOTOR_TIME_CONSTANT = 25;

// Max RPM
inline const int MAX_RPM = 300;

// Motor Encoder Counts Per Revolution
inline const float ENC_CPR = 224.4 * 2;

// Program Parameters =====================================

// Operating Mode
enum OperatingMode { 
    Calibration,
    DAQ,
    DAQ_Compensated,
    StandaloneSpeed,
    StandaloneSpeed_Compensated,
    StandalonePosition,
    StandalonePosition_Compensated
}; inline OperatingMode MODE = DAQ_Compensated;

// Position Control Parameters
inline const float K_P       = 4;
inline const float K_I       = 0;
inline const float K_D       = 0;

// Sample Time in milliseconds (ms)
inline const int SAMPLE_TIME = 2000;

// PWM Frequency in Hertz (Hz)
inline const int DEFAULT_PWM_FREQUENCY = 8000;

#endif

