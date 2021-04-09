extern const int MAX_RPM;

extern volatile long encoderPulseCount;
extern volatile long PWMMeasPulseCount;
extern float current;
extern int dutyCycle;
extern int samplesPerInc;
extern int delayTime;
extern int rpm;
extern int vPWM;
extern int vDutyCycle;

void SendSerialData_Arduino()
{
    Serial.print("PWM Frequency: ");
    Serial.print(PWMMeasPulseCount * (1000 / ((float)delayTime / samplesPerInc)));
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

void SendSerialData_Test()
{
    Serial.print("Pot % (0-255): ");
    Serial.print(map(vDutyCycle, 0, 100, 0, 255));
    Serial.print("\t");
    SendSerialData_Arduino();
}
