#include <Arduino.h>
#line 1 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
#line 1 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
#define PIN_PWM     5
#define PIN_INA     12
#define PIN_INB     13
#define PIN_ENCODER 3
#define ENCODERCPR  244.8//980

volatile long encoderVal = 0;

int interval        = 500;
unsigned long timer = 0;

int rpm      = 0;
int motorPWM = 0;

int dutyCycle = 255;

#line 17 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void setup();
#line 25 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void loop();
#line 38 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void PinInit();
#line 47 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void EncoderInit();
#line 52 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void WriteToPins();
#line 60 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void UpdateEncoder();
#line 65 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void ResetEncoder();
#line 70 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
void SendSerialDataRPM();
#line 81 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
float MillisToSec(float length);
#line 17 "c:\\Users\\Bread Martin\\Desktop\\Google Drive\\SCHOOL SHET\\MSE Capstone SP21-SU21\\Source\\test\\test.ino"
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
    rpm = (float)(encoderVal * 60 / ENCODERCPR / MillisToSec(interval));

    WriteToPins();
    SendSerialDataRPM();
    ResetEncoder();
}

void PinInit()
{
    pinMode(PIN_PWM, OUTPUT);
    pinMode(PIN_INA, OUTPUT);
    pinMode(PIN_INB, OUTPUT);
    pinMode(PIN_ENCODER, INPUT_PULLUP);
}

// Attach interrupt at hall sensor A on each rising signal
void EncoderInit()
{
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), UpdateEncoder, RISING);
}

void WriteToPins()
{
    digitalWrite(PIN_INA, HIGH);
    digitalWrite(PIN_INB, LOW);
    analogWrite(PIN_PWM, dutyCycle);
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
    Serial.print(ENCODERCPR);
    Serial.print(" pulse per rotation * 60 seconds = ");
    Serial.print(rpm);
    Serial.println(" RPM");
    Serial.write(rpm);
}

float MillisToSec(float length)
{
    return length / 1000;
}
