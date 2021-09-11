# MSE Capstone 2021 - Embedded Motor Performance Enhancement System

This project is for an Embedded Smart Motor Performance Enhancement System.

The overall objective of this system is to compensate for the nonlinearities typically found in DC motors at low duty cycles.

## Libraries Required:
[CustomPWM](https://drive.google.com/file/d/1kZl4b6dBvbuqZpM3YXR_VyvR6hqdwDXl/view?usp=sharing)
An open source custom PWM library to adjust PWM frequencies to specific values in real time.

## Current Demos:
[Integrated Robotic Arm Demo](https://drive.google.com/file/d/1WMokHUQ9lWXRpCI-F4r0GELZ6kqYKE1o/view?usp=sharing)
This demo shows the motor nonlinearity compensator integrated with the DAQ and robot arm, performing the final pick and place operation.
[Motor Linearization Demo](https://drive.google.com/file/d/1r8NmuWPFzsUseKfYyBEeedj0Dmy52c4B/view?usp=sharing)
This demo shows the motor calibration and comparisons between the compensated and uncompensated motor performance.
[Motor Characterization Demo](https://drive.google.com/file/d/1oYsNVFDm6LVXxxN1sZ4GI3_xq9xwSXgS/view?usp=sharing)
This demo shows how we visualize the selected motor's nonlinearity based on its RPM for a chosen set of test parameters.
