#ifndef PROPERTIES_H
#define PROPERTIES_H

// Motor Parameters =======================================

// Motor Time Constant in milliseconds (ms)
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
}; inline OperatingMode MODE = Calibration;

// Position Control Parameters
inline const float K_P       = 6;
inline const float K_I       = 0;
inline const float K_D       = 0;

// Sample Time in milliseconds (ms)
inline const int SAMPLE_TIME = 500;

// PWM Frequency in Hertz (Hz)
inline const int DEFAULT_PWM_FREQUENCY = 8000;

#endif

