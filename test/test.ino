#define PIN_PWM     10
#define PIN_INA     12
#define PIN_INB     13
#define PIN_ENCODER 3
#define ENCODERCPR  980

volatile long encoderVal = 0;

int interval    = 1000;
long prevMillis = 0;
long curMillis  = 0;

int rpm         = 0;
int motorPWM    = 0;
bool measureRPM = false;

int test = 255;

void setup()
{
    Serial.begin(9600);

    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), UpdateEncoder, RISING);
    encoderVal = 0;

    PinInit();

    prevMillis = millis();
}

void loop()
{
    // Update RPM value each second
    curMillis = millis();
    if (curMillis - prevMillis > interval) {
        prevMillis = curMillis;

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);
        analogWrite(PIN_PWM, test);

        rpm = (float)(encoderVal * 60 / ENCODERCPR);

        Serial.print(encoderVal);
        Serial.print(" pulse / ");
        Serial.print(ENCODERCPR);
        Serial.print(" pulse per rotation * 60 seconds = ");
        Serial.print(rpm);
        Serial.println(" RPM");

        encoderVal = 0;
    }
}

void PinInit()
{
    pinMode(PIN_PWM, OUTPUT);
    pinMode(PIN_INA, OUTPUT);
    pinMode(PIN_INB, OUTPUT);
    pinMode(PIN_ENCODER, INPUT_PULLUP);
}

// Attach interrupt at hall sensor A on each rising signal
// void EncoderInit()
// {

// }

// Add encoderVal by 1 each time it detects a rising signal from hall sensor A
void UpdateEncoder()
{
    Serial.print("Hello");
    encoderVal++;
}
