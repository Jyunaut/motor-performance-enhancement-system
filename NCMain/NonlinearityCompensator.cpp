#include "NonlinearityCompensator.h"

void NC::Initialize()
{
    pinMode(PIN_ENCA, INPUT_PULLUP);
    pinMode(PIN_ENCB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCA), UpdateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCB), UpdateEncoder, CHANGE);

    InitTimersSafe();
    SetPinFrequencySafe(PIN_PWMA, DEFAULT_PWM_FREQUENCY);

    //EnableCompensation();
}

void NC::EnableCompensation()
{
    compensationEnabled = true;

    // Set up lookup table in RAM from EEPROM
    int i = 5;
    while (i <= EEPROM.read(4)) {
        if (i >= EEPROM.read(3))
            NC::SaveToTable(EEPROM.read(i), EEPROM.read(1), EEPROM.read(i+1));
        else
            NC::SaveToTable(EEPROM.read(i), EEPROM.read(2) * 100, EEPROM.read(i+1));
        Serial.println(EEPROM.read(i+1));
        i = i + 2;
    }

    // Interpolate
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
                // Fill the rest of the values for the low-to-high frequency transition
                if (i == EEPROM.read(0) - 1 && interpolateLength > 0) {
                    for (int j = tempIndex + 1; j <= i; j++) {
                        NC::SaveToTable(j, EEPROM.read(1), NC::GetTableCompensatedDutyCycle(tempIndex) + j - tempIndex);
                    }
                    interpolateLength = 0;
                    tempIndex = i;
                // Fill the rest of the end values with the maximum duty cycle
                } else if (i >= 100 && interpolateLength > 0) {
                    for (int j = tempIndex + 1; j <= i; j++) {
                        NC::SaveToTable(j, DEFAULT_PWM_FREQUENCY, 100);
                    }
                }
            } else if (NC::GetTablePWMFrequency(i) != 0 && interpolateLength > 0) {
                float ratio = (NC::GetTableCompensatedDutyCycle(i) - NC::GetTableCompensatedDutyCycle(tempIndex)) / (float)(interpolateLength + 1);
                for (int j = tempIndex + 1; j <= i - 1; j++) {
                    NC::SaveToTable(j, i < EEPROM.read(0) ? EEPROM.read(1) : DEFAULT_PWM_FREQUENCY, NC::GetTableCompensatedDutyCycle(tempIndex) + round(ratio * (j - tempIndex)));
                }
                interpolateLength = 0;
                tempIndex = i;
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
}

void NC::DisableCompensation()
{
    compensationEnabled = false;
}

void NC::Compensate()
{
    // Grab stuff from array
}

void NC::WriteToMotorDAQ()
{
    if (compensationEnabled)
    {
        SetPinFrequencySafe(PIN_PWMA, GetTablePWMFrequency(dutyCycle));
        if (dutyCycleSign == 0) {
            pwmWrite(PIN_PWMA, map(GetTableCompensatedDutyCycle(dutyCycle), 0, 100, 0, 255));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycleSign == 1) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, map(GetTableCompensatedDutyCycle(dutyCycle), 0, 100, 0, 255));
        }
    } else {
        SetPinFrequencySafe(PIN_PWMA, DEFAULT_PWM_FREQUENCY);
        if (dutyCycleSign == 0) {
            pwmWrite(PIN_PWMA, map(abs(dutyCycle), 0, 100, 0, 255));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycleSign == 1) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, map(abs(dutyCycle), 0, 100, 0, 255));
        }
    }
}

void NC::WriteToMotor()
{
    if (compensationEnabled)
    {
        SetPinFrequencySafe(PIN_PWMA, GetTablePWMFrequency(abs(dutyCycle)));
        if (dutyCycle > 0) {
            pwmWrite(PIN_PWMA, map(GetTableCompensatedDutyCycle(abs(dutyCycle)), 0, 100, 0, 255));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycle < 0) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, map(GetTableCompensatedDutyCycle(abs(dutyCycle)), 0, 100, 0, 255));
        } else {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, 0);
        }
    } else {
        if (dutyCycle > 0) {
            pwmWrite(PIN_PWMA, abs(map(dutyCycle, -100, 100, -255, 255)));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycle < 0) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, abs(map(dutyCycle, -100, 100, -255, 255)));
        } else {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, 0);
        }
    }
}

void NC::SaveAngleToStorage()
{
    EEPROM.update(0, encPulseCount);
}

void NC::ReadAngleFromStorage()
{
    encPulseCount = EEPROM.read(0);
}

void NC::UpdateEncoder()
{
    volatile static int prevStateA;
    int stateA = digitalRead(PIN_ENCA);
    int stateB = digitalRead(PIN_ENCB);
    if (stateA != prevStateA)
        if (stateB != stateA)
            encPulseCount++;
        else
            encPulseCount--;
    prevStateA = stateA;
}

long NC::GetEncoderPulseCount()
{
    return encPulseCount;
}

void NC::ResetEncoderPulseCount()
{
    encPulseCount = 0;
}

void NC::SaveToTable(int index, int frequency, int dutyCycle)
{
    table[index <= 100 ? index : 100][0] = frequency;
    table[index <= 100 ? index : 100][1] = dutyCycle;
}

int NC::GetTablePWMFrequency(int desiredDutyCycle)
{
    if (desiredDutyCycle < 0 || desiredDutyCycle > 100)
        return table[0][0];
    return table[desiredDutyCycle][0];
}

int NC::GetTableCompensatedDutyCycle(int desiredDutyCycle)
{
    if (desiredDutyCycle < 0 || desiredDutyCycle > 100)
        return table[0][1];
    return table[desiredDutyCycle][1];
}

// This exists in order to call NC::Compensate for I2C
void Compensate()
{
    NC::Compensate();
}

// This exists in order to attach UpdateEncoder to an interrupt
void UpdateEncoder()
{
    NC::UpdateEncoder();
}

void StartupSequence()
{
    Serial.print("Initializing: ");
    delay(1000);
    Serial.print("3, ");
    delay(1000);
    Serial.print("2, ");
    delay(1000);
    Serial.println("1...");
    delay(1000);
}

int HandleSerialInput()
{
    String input = "";

    while (Serial.available()) {
        delay(2);
        char c = Serial.read();
        input += c;
    }

    if (input.length() > 0) {
        return input.toInt();
    }
}

void HandleI2CInput(int numBytes)
{
    while (Wire.available()) {
        uint8_t num = Wire.read();
        dutyCycleSign = num >> 7;
        dutyCycle = num & 0x7F;
    }

    // while (Wire.available()) {
    //     desiredPosition = Wire.read();
    // }
}

void StoreNewAngle()
{
    EEPROM.update(0, currentPositionCPR);
}
