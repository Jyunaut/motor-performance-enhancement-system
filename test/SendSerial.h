#ifndef SENDSERIAL_H_
#define SENDSERIAL_H_

/* ==================================================================
 * Sends serial data in a format that is readable in the Arduino
 * serial monitor
 */
void SendSerialData_Arduino();

/* ==================================================================
 * Sends serial data in a format that works for the PLX-DAQ Excel
 * plug-in that is used for collecting the data
 */
void SendSerialData_Excel();

/* ==================================================================
 * Use for test logic
 */
void SendSerialData_Test();
#endif