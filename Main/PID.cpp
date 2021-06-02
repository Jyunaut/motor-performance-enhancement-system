#include <Arduino.h>
#include "Settings.h"
#include "PID.h"

PID::PID(float* input, float* output, float* setpoint,
         float kp, float ki, float kd, int sampleTime)
{
    PID::input    = input;
    PID::output   = output;
    PID::setpoint = setpoint;
    PID::kp = kp;
    PID::ki = ki;
    PID::kd = kd;
    PID::sampleTime = sampleTime;
}

void PID::CalculateDutyCycle()
{
    curTime = millis();

    p_error = *setpoint - *input;
    i_error += p_error * (curTime - prevTime);
    d_error = (p_error - prevError) / (curTime - prevTime);

    prevError = p_error;
    prevTime = curTime;

    *output = kp*p_error + ki*i_error + kd*d_error;

    if      (*output >  255) *output =  255;
    else if (*output < -255) *output = -255;

    delay(sampleTime);
}

float PID::GetP()
{
    return kp*p_error;
}

float PID::GetI()
{
    return ki*i_error;
}

float PID::GetD()
{
    return kd*d_error;
}