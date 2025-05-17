/*
 * File: master.c
* Author: Marwen Maghrebi
* 
* Description: 
*This code configures a PIC microcontroller as an I2C master.
*It reads data from an I2C slave device connected to a DIP switch
*and displays the read value on an LED bar connected to Port D.
*/
 
// CONFIG
#pragma config FOSC = HS    // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF   // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON   // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF    // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF    // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF    // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF     // Flash Program Memory Code Protection bit (Code protection off)
 
#include <xc.h>
 
#define _XTAL_FREQ 16000000      // 16 MHz Clock Frequency
#define I2C_BAUD_RATE 100000     // I2C Baud Rate: 100 Kbps
 
// Function Prototypes
void I2C_Master_Init(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
void I2C_ACK(void);
void I2C_NACK(void);
unsigned char I2C_Read(void);
unsigned char I2C_Write(unsigned char data);
 
void main(void) {
    // Configure Port D as output for LED bar
    TRISD = 0x00;
    PORTD = 0x00;
 
    // Initialize I2C master
    I2C_Master_Init();
 
    while (1) {
        I2C_Start();           // I2C Start Sequence
        I2C_Write(0x41);       // I2C Slave Device Address (0x40) + Read (1)
        PORTD = I2C_Read();    // Read data from slave and display on Port D
        I2C_Stop();            // I2C Stop Sequence
        __delay_ms(100);       // Delay 100 ms
    }
}
 
void I2C_Master_Init(void) {
    SSPCON = 0x28;              // SSPEN = 1, I2C Master mode
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ / 4) / I2C_BAUD_RATE) - 1;  // Baud rate formula
    TRISC3 = 1;                // SCL (clock) as input
    TRISC4 = 1;                // SDA (data) as input
}
 
void I2C_Wait(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // Wait until SSPBUF is clear
}
 
void I2C_Start(void) {
    I2C_Wait();
    SEN = 1;  // Initiate I2C start condition
}
 
void I2C_Stop(void) {
    I2C_Wait();
    PEN = 1;  // Initiate I2C stop condition
}
 
void I2C_Restart(void) {
    I2C_Wait();
    RSEN = 1;  // Initiate I2C restart condition
}
 
void I2C_ACK(void) {
    I2C_Wait();
    ACKDT = 0;  // 0 -> ACK
    ACKEN = 1;  // Send ACK signal
}
 
void I2C_NACK(void) {
    I2C_Wait();
    ACKDT = 1;  // 1 -> NACK
    ACKEN = 1;  // Send NACK signal
}
 
unsigned char I2C_Write(unsigned char data) {
    I2C_Wait();
    SSPBUF = data;  // Load data into buffer
    I2C_Wait();
    return ACKSTAT;  // Return ACK/NACK status
}
 
unsigned char I2C_Read(void) {
    unsigned char data;
    I2C_Wait();
    RCEN = 1;  // Enable receive mode
    I2C_Wait();
    data = SSPBUF;  // Read data from buffer
    I2C_NACK();     // Send NACK signal
    return data;
}