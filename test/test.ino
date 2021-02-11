#define pin_PWM     10
#define pin_ENCODER 3

void setup()
{
    Serial.begin(9600);

    pinMode(10, OUTPUT);
    pinMode(3, INPUT_PULLUP);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
}

void loop()
{
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    analogWrite(pin_PWM, 50);
    
    delay(1500);
    
    while(1) {
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
    }
}

