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
        SetPinFrequencySafe(PIN_PWMA, pwmFrequency);
        if (dutyCycleSign == 0) {
            pwmWrite(PIN_PWMA, map(dutyCycleCompensated, 0, 100, 0, 255));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycleSign == 1) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, map(dutyCycleCompensated, 0, 100, 0, 255));
        }
    } else {
        SetPinFrequencySafe(PIN_PWMA, DEFAULT_PWM_FREQUENCY);
        if (dutyCycleSign == 0) {
            pwmWrite(PIN_PWMA, abs(dutyCycle));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycleSign == 1) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, abs(dutyCycle));
        }
    }
}

void NC::WriteToMotor()
{
    if (compensationEnabled)
    {
        SetPinFrequencySafe(PIN_PWMA, pwmFrequency);
        if (dutyCycle > 0) {
            pwmWrite(PIN_PWMA, map(dutyCycleCompensated, 0, 100, 0, 255));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycle < 0) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, map(dutyCycleCompensated, 0, 100, 0, 255));
        }
    } else {
        SetPinFrequencySafe(PIN_PWMA, DEFAULT_PWM_FREQUENCY);
        if (dutyCycle > 0) {
            pwmWrite(PIN_PWMA, abs(map(dutyCycle, -100, 100, -255, 255)));
            pwmWrite(PIN_PWMB, 0);
        } else if (dutyCycle < 0) {
            pwmWrite(PIN_PWMA, 0);
            pwmWrite(PIN_PWMB, abs(map(dutyCycle, -100, 100, -255, 255)));
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
    return table[desiredDutyCycle][0];
}

int NC::GetTableCompensatedDutyCycle(int desiredDutyCycle)
{
    return table[desiredDutyCycle][1];
}

void NC::SaveTableToStorage()
{

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

    while(Serial.available()) {
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
    while(Wire.available()) {
        int num = Wire.read();
        dutyCycleSign = num >> 7;
        dutyCycle = num & 0x7F;
    }
}

void StoreNewAngle()
{
    EEPROM.update(0, currentPositionCPR);
}
