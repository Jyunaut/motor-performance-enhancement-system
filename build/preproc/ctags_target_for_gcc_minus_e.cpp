# 1 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
# 1 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"






volatile long encoderVal = 0;

int interval = 500;
unsigned long timer = 0;

int rpm = 0;
int motorPWM = 0;

int dutyCycle = 255;

void setup()
{
    Serial.begin(9600);

    PinInit();
    EncoderInit();
}

void loop()
{
    if (millis() - timer < interval)
        return;

    timer = millis();
    rpm = (float)(encoderVal * 60 / 244.8/*980*/ / MillisToSec(interval));

    WriteToPins();
    SendSerialDataRPM();
    ResetEncoder();
}

void PinInit()
{
    pinMode(5, 0x1);
    pinMode(12, 0x1);
    pinMode(13, 0x1);
    pinMode(3, 0x2);
}

// Attach interrupt at hall sensor A on each rising signal
void EncoderInit()
{
    attachInterrupt(((3) == 2 ? 0 : ((3) == 3 ? 1 : -1)), UpdateEncoder, 3);
}

void WriteToPins()
{
    digitalWrite(12, 0x1);
    digitalWrite(13, 0x0);
    analogWrite(5, dutyCycle);
}

// Add encoderVal by 1 each time the interrupt is fired
void UpdateEncoder()
{
    encoderVal++;
}

void ResetEncoder()
{
    encoderVal = 0;
}

void SendSerialDataRPM()
{
    Serial.print(encoderVal);
    Serial.print(" pulse / ");
    Serial.print(244.8/*980*/);
    Serial.print(" pulse per rotation * 60 seconds = ");
    Serial.print(rpm);
    Serial.println(" RPM");
    Serial.write(rpm);
}

float MillisToSec(float length)
{
    return length / 1000;
}
