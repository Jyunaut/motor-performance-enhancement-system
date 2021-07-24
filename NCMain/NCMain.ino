#include "NonlinearityCompensator.h"

int input = 0;

void setup()
{
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(HandleI2CInput);
    Serial.begin(SERIAL_BAUDRATE);

    NC::Initialize();

    //StartupSequence();
}

void loop()
{
    while (Serial.available()) {
        input = HandleSerialInput();
    }
    if (MODE == OperatingMode::Calibration) {
        CalibrationMode();
    } else if (MODE == OperatingMode::DAQ) {
        DAQMode();
    } else if (MODE == OperatingMode::DAQ_Compensated) {
        DAQMode_Compensated();
    } else if (MODE == OperatingMode::StandaloneSpeed) {
        StandaloneSpeedMode(input);
    } else if (MODE == OperatingMode::StandaloneSpeed_Compensated) {
        StandaloneSpeedMode_Compensated(input);
    } else if (MODE == OperatingMode::StandalonePosition) {
        StandalonePositionMode(input);
    } else if (MODE == OperatingMode::StandalonePosition_Compensated) {
        StandalonePositionMode_Compensated(input);
    } else {
        Serial.println("Invalid Operating Mode.");
        while(1);
    }
}

void DAQMode()
{
    HandleI2CInput(1);
    analogWrite(PIN_PWMA, map(dutyCycle, 0, 100, 0, 255));
}

void DAQMode_Compensated()
{
    NC::EnableCompensation();
}

void StandaloneSpeedMode(int input)
{
    static unsigned long timer;
    dutyCycle = input;
    NC::WriteToMotor();
    if (millis() - timer >= SAMPLE_TIME) {
        Serial.print(input);
        Serial.print("Time (ms): ");
        Serial.print(millis());
        Serial.print("\t\t");
        Serial.print("RPM: ");
        Serial.println(NC::GetEncoderPulseCount() * 60 / ENC_CPR / (SAMPLE_TIME / (float)1000));
        NC::ResetEncoderPulseCount();
        timer = millis();
    }
}

void StandaloneSpeedMode_Compensated(int input)
{
    NC::EnableCompensation();
    static unsigned long timer;
    dutyCycle = HandleSerialInput();
    NC::Compensate();
    analogWrite(PIN_PWMA, map(dutyCycleCompensated, 0, 100, 0, 255));
    if (millis() - timer >= SAMPLE_TIME) {
        Serial.print("Time (ms): ");
        Serial.print(millis());
        Serial.print("RPM: ");
        Serial.println(NC::GetEncoderPulseCount() * 60 / ENC_CPR / (SAMPLE_TIME / (float)1000));
        NC::ResetEncoderPulseCount();
        timer = millis();
    }
}

void StandalonePositionMode(int input)
{
    currentPositionCPR = NC::GetEncoderPulseCount();
    currentPosition = map(currentPositionCPR, 0, ENC_CPR, 0, 360);
    desiredPosition = HandleSerialInput();
    desiredPositionCPR = desiredPosition * ENC_CPR / 360;
    pid.CalculateDutyCycle();
    NC::WriteToMotor();
}

void StandalonePositionMode_Compensated(int input)
{
    NC::EnableCompensation();
    currentPositionCPR = NC::GetEncoderPulseCount();
    currentPosition = map(currentPositionCPR, 0, ENC_CPR, 0, 360);
    desiredPosition = HandleSerialInput();
    desiredPositionCPR = desiredPosition * ENC_CPR / 360;
    pid.CalculateDutyCycle();
    NC::Compensate();
    NC::WriteToMotor();
}

void CalibrationMode()
{
    const long sampleTime = 1000;
    unsigned long timer = 0;
    int timeConstantPWM = 0;
    int lowestDutyCycle = 0;
    // Temporary Table: { index, duty cycle }
    // int tempTable[51][2] = {0};
    // dutyCycle = 0;

    // Calculate lowest PWM frequency
    if (MOTOR_TIME_CONSTANT > 0) {
        timeConstantPWM = 1 / (MOTOR_TIME_CONSTANT / 1000);
        // TODO: Round PWM to nearest ten
        EEPROM.update(1, timeConstantPWM);
        EEPROM.update(2, DEFAULT_PWM_FREQUENCY / 100);
        Serial.print("Lowest PWM frequency: ");
        Serial.println(timeConstantPWM);
        delay(2000);
    } else {
        Serial.println("Time constant should not be zero.");
        while(1);
    }

    // // Generate table with normal operating PWM
    // bool startedMoving = false;
    // int i = 5, j = 0;
    // int rpm = 0;
    // int actualDutyCycle = 0;
    // int sampleSkips = 0, sampleSkipCount = 0;
    // while (i <= 100) {
    //     SetPinFrequencySafe(PIN_PWMA, DEFAULT_PWM_FREQUENCY);
    //     dutyCycle = i;
    //     NC::WriteToMotor();
    //     if (millis() - timer >= sampleTime) {
    //         if (sampleSkipCount < sampleSkips) {
    //             Serial.print(sampleSkipCount);
    //             sampleSkipCount++;
    //             timer = millis();
    //             continue;
    //         } else {
    //             sampleSkipCount = 0;
    //         }
    //         rpm = (NC::GetEncoderPulseCount() * 60 / ENC_CPR / ((float)sampleTime / (float)1000)) / (sampleSkips + 1);
    //         Serial.print("Duty Cycle: "); Serial.print(dutyCycle); Serial.print("\t");
    //         Serial.print("RPM: "); Serial.print(rpm); Serial.print("\t\t");
    //         Serial.print("Actual Duty Cycle: "); Serial.println(roundf((float)rpm / (float)MAX_RPM * 100));
    //         if (rpm > MAX_RPM * 5 / 100) {
    //             actualDutyCycle = roundf((float)rpm / (float)MAX_RPM * 100);
    //             if (actualDutyCycle > 100)
    //                 break;
    //             tempTable[j][0] = actualDutyCycle;
    //             tempTable[j][1] = i;
    //             j++;
    //             if (!startedMoving) {
    //                 lowestDutyCycle = actualDutyCycle;
    //                 EEPROM.update(0, lowestDutyCycle);
    //                 startedMoving = true;
    //             }
    //         }
    //         NC::ResetEncoderPulseCount();
    //         timer = millis();
    //         i = i + 5;
    //     }
    // }
    // // Save Low Frequency Index
    // EEPROM.update(3, 2*j+5);
    // while (rpm > 0) {
    //     pwmWrite(PIN_PWMA, 0);
    //     pwmWrite(PIN_PWMB, 0);
    //     rpm = NC::GetEncoderPulseCount() * 60 / ENC_CPR / ((float)sampleTime / (float)1000);
    //     delay(1000);
    // }

    // // Generate table with time constant PWM
    // bool firstPass = true;
    // i = 0;
    // actualDutyCycle = 0;
    // sampleSkips = 0;
    // sampleSkipCount = 0;
    // while (actualDutyCycle < lowestDutyCycle) {
    //     SetPinFrequencySafe(PIN_PWMA, timeConstantPWM);
    //     dutyCycle = i;
    //     NC::WriteToMotor();
    //     if (millis() - timer >= sampleTime) {
    //         if (sampleSkipCount < sampleSkips) {
    //             Serial.print(sampleSkipCount);
    //             sampleSkipCount++;
    //             timer = millis();
    //             continue;
    //         } else {
    //             sampleSkipCount = 0;
    //         }
    //         rpm = (NC::GetEncoderPulseCount() * 60 / ENC_CPR / ((float)sampleTime / (float)1000)) / (sampleSkips + 1);
    //         Serial.print("Duty Cycle: "); Serial.print(dutyCycle); Serial.print("\t");
    //         Serial.print("RPM: "); Serial.print(rpm); Serial.print("\t\t");
    //         Serial.print("Actual Duty Cycle: "); Serial.println(roundf((float)rpm / (float)MAX_RPM * 100));
    //         actualDutyCycle = roundf((float)rpm / (float)MAX_RPM * 100);
    //         if (rpm > MAX_RPM * 5 / 100 && actualDutyCycle > tempTable[j][0]) {
    //             if (actualDutyCycle > lowestDutyCycle)
    //                 break;
    //             tempTable[j][0] = actualDutyCycle;
    //             tempTable[j][1] = i;
    //             j++;
    //         }
    //         NC::ResetEncoderPulseCount();
    //         timer = millis();
    //         i = i + 2;
    //     }
    // }
    // // Save End of Key Value Index
    // EEPROM.update(4, 2*j+4);
    
    // i = 5;
    // for (int k = 0; k <= j; k++) {
    //     EEPROM.update(i, tempTable[k][0]);
    //     EEPROM.update(i+1, tempTable[k][1]);
    //     i = i + 2;
    // }

    // // ---------------------------------------------

    // // Verify Temporary Table
    // for (int i = 0; i <= 50; i++) {
    //     Serial.print(i); Serial.print("\t");
    //     Serial.print(tempTable[i][0]); Serial.print("\t");
    //     Serial.println(tempTable[i][1]);
    // }

    // // Verify EEPROM
    // for (int i = 0; i <= 50; i++) {
    //     Serial.print(i); Serial.print("\t");
    //     Serial.println(EEPROM.read(i));
    // }

    // // Set up lookup table in RAM from EEPROM
    // i = 5;
    // while (i <= EEPROM.read(4)) {
    //     if (i >= EEPROM.read(3))
    //         NC::SaveToTable(EEPROM.read(i), EEPROM.read(1), EEPROM.read(i+1));
    //     else
    //         NC::SaveToTable(EEPROM.read(i), EEPROM.read(2) * 100, EEPROM.read(i+1));
    //     i = i + 2;
    // }

    // Verify Lookup Table
    for (int i = 0; i <= 100; i++) {
        Serial.print(i);
        Serial.print("\t\t\t");
        Serial.print(NC::GetTablePWMFrequency(i));
        Serial.print("\t\t\t");
        Serial.println(NC::GetTableCompensatedDutyCycle(i));
    }

    // Interpolate!
    bool skippingDeadband = true;
    int lowestIndex = 0;
    int tempIndex = 0;
    int interpolateLength = 0;
    for (int i = 0; i <= 100; i++) {
        // Skip through deadband
        if (skippingDeadband) {
            if (NC::GetTablePWMFrequency(i) == 0) {
                continue;
            } else {
                // Save the lowest index and the first range to begin interpolating
                lowestIndex = i;
                tempIndex = i;
                skippingDeadband = false;
            }
        } else {
            // Go through zeros and increment the length of the interpolation range
            // Otherwise, linearly spread the difference of the low and high range
            // across the length of the zeros
            if (NC::GetTablePWMFrequency(i) == 0) {
                interpolateLength++;
            } else if (interpolateLength > 0) {
                float ratio = (NC::GetTableCompensatedDutyCycle(i) - NC::GetTableCompensatedDutyCycle(tempIndex)) / (float)interpolateLength;
                Serial.println(ratio);
                for (int j = tempIndex + 1; j <= i - 1; j++) {
                    NC::SaveToTable(j, timeConstantPWM, NC::GetTableCompensatedDutyCycle(tempIndex) * ratio);
                }
                interpolateLength = 0;
            } else {
                tempIndex = i;
            }
        }
    }

    Serial.println("Verify Interpolated Lookup Table");

    // Verify Interpolated Lookup Table
    for (int i = 0; i <= 100; i++) {
        Serial.print(i);
        Serial.print("\t\t\t");
        Serial.print(NC::GetTablePWMFrequency(i));
        Serial.print("\t\t\t");
        Serial.println(NC::GetTableCompensatedDutyCycle(i));
    }

    Serial.println("Done");
    while(1);

    /*
    
    Example parameters:

        Max RPM: 300 RPM
        Normal Operating PWM: 8000 Hz
        Time Constant PWM: 43 Hz

    1.  Generate table with normal operating PWM
    
        Start from 0% duty cycle and increment by 5% each second and save corresponding RPM
        to duty cycle in index matching the actual duty cycle

        ...
        15% Duty Cycle => Measured RPM:   0 RPM (  0/300 =  0% Actual Duty Cycle)
            - Measured RPM < 5% of Max RPM
            > Index  0: 8000,  0
        20% Duty Cycle => Measured RPM:  75 RPM ( 75/300 = 25% Actual Duty Cycle)
            - Measured RPM > 5% of Max RPM -> Save deadband index
            > Index 25: 8000, 20
        25% Duty Cycle => Measured RPM:  93 RPM ( 93/300 = 31% Actual Duty Cycle)
            > Index 31: 8000, 25
        30% Duty Cycle => Measured RPM: 108 RPM (108/300 = 36% Actual Duty Cycle)
            > Index 36: 8000, 30
        35% Duty Cycle => Measured RPM: 126 RPM (126/300 = 42% Actual Duty Cycle)
            > Index 42: 8000, 35
        ...
        90% Duty Cycle => Measured RPM: 255 RPM (255/300 = 85% Actual Duty Cycle)
            > Index 85: 8000, 90
        95% Duty Cycle => Measured RPM: 255 RPM (255/300 = 85% Actual Duty Cycle)
            > Index 85: Already filled, omit value
        100% Duty Cycle => Measured RPM: 255 RPM (255/300 = 85% Actual Duty Cycle)
            > Index 85: Already filled, omit value

        EEPROM Storage: |---|---|---|---|---|---
                          0   1   2   3   4   5+
        
        0: Deadband index
        1: Time Constant PWM Frequency
        2: Operating PWM Frequency
        3: Low Frequency Index
        4: End of Key Value Index
        5: Duty Cycle Index (7)
        6: Duty Cycle       (5)
        7: Duty Cycle Index (13)
        8: Duty Cycle       (10)
        ...

    2.  Generate table with time constant PWM

        Round calculated PWM duty cycle up to the nearest ten
            > 43 Hz -> 50 Hz

        Start from 0% duty cycle and increment by 1% each second until the motor 
        starts moving.

        -   We will consider motor movement to be greater than 5% of the max RPM. This means
            that there will always be a 5% deadband. This is to ensure that the motor moves
            at the requested duty cycle since there is more uncertainty on whether the motor
            will actually start moving for duty cycles very close to the deadband.
                eg. 15 RPM for 300 RPM motor

        ...
        14% Duty Cycle => Measured RPM:  0 RPM (  0/300 =  0% Actual Duty Cycle)
            - Measured RPM < 5% of Max RPM
            > Index  0: 50, 0
        15% Duty Cycle => Measured RPM: 33 RPM ( 33/300 = 11% Actual Duty Cycle)
            > Index 11: 50, 15
        ...
        24% Duty Cycle => Measured RPM: 63 RPM ( 63/300 = 21% Actual Duty Cycle)
            - Actual Duty Cycle > Deadband index, omit value

    3.  Interpolate table

        Start at 0% Duty Cycle and fill the indices with Time Constant PWM and zero until the
        duty cycle value is non-zero and greater than 5%
            eg. { 50, 0 },
                { 50, 0 },
                { 50, 0 },
                { 50, 0 },
                { 50, 0 },
                ...

    */
}