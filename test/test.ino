#define PIN_PWM     10
#define PIN_INA     12
#define PIN_INB     13
#define PIN_ENCODER 3
#define ENCODERCPR  244.8 //980

volatile long encoderVal = 0;

int interval        = 1000;
unsigned long timer = 0;

int rpm      = 0;
int motorPWM = 0;

int dutyCycle;

bool doReverse = false;
enum OutputMode { SerialMonitor, Excel };
OutputMode mode;

void setup()
{
    Serial.begin(9600);

    mode = Excel;
    doReverse = false;
    
    dutyCycle = doReverse ? 255 : 0;

    // Change PWM frequency on Pin 10 to 3921.16 Hz
    TCCR1B = TCCR1B & B11111000 | B00000010;

    PinInit();
    EncoderInit();
    delay(3000);
}

void loop()
{
    if (millis() - timer < interval)
        return;
    if (dutyCycle <= 0 && doReverse
        || dutyCycle >= 255 && !doReverse)
        return;

    timer = millis();
    rpm = (float)(encoderVal * 60 / ENCODERCPR / MillisToSec(interval));
    dutyCycle = doReverse ? dutyCycle-1 : dutyCycle+1;
    WriteToPins();
    switch (mode) {
        case SerialMonitor:
            SendSerialDataRPM_SM();
            break;
        case Excel:
            SendSerialDataRPM_Excel();
            break;
        default:
            return;
    }
    ResetEncoder();
}

void PinInit()
{
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

void SendSerialDataRPM_SM()
{
    Serial.print("Duty Cycle: ");
    Serial.print((float)dutyCycle / 255 * 100);
    Serial.print("\t");
    Serial.print(encoderVal);
    Serial.print(" pulse / ");
    Serial.print(ENCODERCPR);
    Serial.print(" pulse per rotation * 60 seconds = ");
    Serial.print(rpm);
    Serial.print(" RPM");
    Serial.print(rpm);
    Serial.print((float)dutyCycle / 255 * 100);
}

void SendSerialDataRPM_Excel()
{
    Serial.print("DATA,TIME,");
    Serial.print((float)dutyCycle / 255 * 100);
    Serial.print(",");
    Serial.print(rpm);
    Serial.print(",");
    Serial.println((float)dutyCycle / 255 * 500);
}

float MillisToSec(float length)
{
    return length / 1000;
}
