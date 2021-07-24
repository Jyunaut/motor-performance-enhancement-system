#ifndef PID_H
#define PID_H

class PID
{
public:
    PID(float* input, float* output, float* setpoint,
        float kp, float ki, float kd, int sampleTime);
    /* ==================================================================
    * Returns the appropriate duty cycle based on the current position of
    * the motor.
    */
   void CalculateDutyCycle();
   float Get_P_Error();
   float Get_I_Error();
   float Get_D_Error();
private:
    unsigned long curTime;
    unsigned long prevTime;

    float* input;
    float* output;
    float* setpoint;
    float kp;
    float ki;
    float kd;
    int sampleTime;

    float prevError;
    float p_error;
    float i_error;
    float d_error;
};

#endif