# MSE Capstone 2021 - Embedded Motor Performance Enhancement System

This project is for an Embedded Smart Motor Performance Enhancement System.

The overall objective of this system is to compensate for the nonlinearities typically found in DC motors at low duty cycles.

## Libraries Required:
For the Arduino Uno R3

[CustomPWM](https://drive.google.com/file/d/1kZl4b6dBvbuqZpM3YXR_VyvR6hqdwDXl/view?usp=sharing)
An open source custom PWM library to adjust PWM frequencies to specific values in real time.

## Current Demos:
[Integrated Robotic Arm Demo](https://drive.google.com/file/d/1WMokHUQ9lWXRpCI-F4r0GELZ6kqYKE1o/view?usp=sharing)
This demo shows the motor nonlinearity compensator integrated with the DAQ and robot arm, performing the final pick and place operation.

[Motor Linearization Demo](https://drive.google.com/file/d/1r8NmuWPFzsUseKfYyBEeedj0Dmy52c4B/view?usp=sharing)
This demo shows the motor calibration and comparisons between the compensated and uncompensated motor performance.

Motor: [DFRobot FIT0520 6V 300RPM Gearmotor](https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/FIT0520_Web.pdf)

Motor Driver: [Seeed Technology MAKER MDD3A 3A DC Motor Driver](https://media.digikey.com/pdf/Data%20Sheets/Seeed%20Technology/105090004_Web.pdf)

[Motor Characterization Demo](https://drive.google.com/file/d/1oYsNVFDm6LVXxxN1sZ4GI3_xq9xwSXgS/view?usp=sharing)
This demo shows how we visualize the selected motor's nonlinearity based on its RPM for a chosen set of test parameters.

Motor: [Pololu 12V 500RPM Gearmotor](https://www.pololu.com/product/4843)

Motor Driver: [Pololu VNH5019 Motor Driver](https://www.pololu.com/product/1451)
