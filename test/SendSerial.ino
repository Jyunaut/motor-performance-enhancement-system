extern const int MAX_RPM;

extern volatile long encoderPulseCount;
extern volatile long PWMMeasPulseCount;
extern float current;
extern int dutyCycle;
extern int delayTime;
extern int rpm;

void SendSerialData_Arduino()
{
    Serial.print("PWM Frequency: ");
    Serial.print(PWMMeasPulseCount * (1000 / (float)delayTime));
    Serial.print(" Hz \t\t");
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.print("% \t\t");
    Serial.print("Motor Speed: ");
    Serial.print(rpm);
    Serial.print(" RPM \t\t");
    Serial.print("Desired Motor Speed: ");
    Serial.print((int)(dutyCycle / (float)100 * MAX_RPM));
    Serial.println(" RPM");
    // Serial.print("Measured Current: ");
    // Serial.print(current);
    // Serial.println(" mA");
}

void SendSerialData_Excel()
{
    Serial.print("DATA,TIME,");
    Serial.print(dutyCycle);
    Serial.print(",");
    Serial.print((int)(dutyCycle / (float)100 * MAX_RPM));
    Serial.print(",");
    Serial.println(rpm);
}

String num1;
void SendSerialData_Test()
{
    Serial.println(num1);
    if (Serial.available())
    {
        num1 = Serial.readStringUntil('\n');
        Serial.println(num1);
    }
}
