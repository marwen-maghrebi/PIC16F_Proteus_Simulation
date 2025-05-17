/** File: slave.c
* Author: Marwen Maghrebi
* 
* Description: 
*This code configures a PIC microcontroller as an I2C slave.
*It reads the state of a DIP switch connected to Port B and
*sends this state to the I2C master upon request.
*/
 
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
#include <xc.h>
 
#define _XTAL_FREQ 4000000   // 4 MHz Clock Frequency
 
// Function Prototypes
void I2C_Slave_Init(unsigned char address);
 
void main(void) {
    // Configure Port B as input for DIP switch
    TRISB = 0xFF;
    nRBPU = 0;  // Enable PORTB pull-ups
 
    // Initialize I2C slave with address 0x40
    I2C_Slave_Init(0x40);
 
    while (1) {
        // Infinite loop, waiting for I2C communication
    }
}
 
void __interrupt() ISR(void) {
    if (SSPIF) {  // Check if I2C interrupt flag is set
        if (!D_nA && R_nW) {  // Check if it is an address match and read request
            volatile char dummy = SSPBUF;  // Clear the buffer
            SSPBUF = PORTB;  // Load the DIP switch state into the buffer
            CKP = 1;  // Release the clock
            while (BF);  // Wait until the buffer is clear
        }
        CKP = 1;  // Release the clock
        SSPIF = 0;  // Clear the interrupt flag
    }
}
 
void I2C_Slave_Init(unsigned char address) {
    SSPADD = address;  // Set the I2C slave address
    SSPSTAT = 0x80;  // Slew rate control disabled
    SSPCON = 0x36;  // Enable clock stretching and set slave mode
    SSPCON2 = 0x01;  // Enable general call
    TRISC3 = 1;  // Set SCL as input
    TRISC4 = 1;  // Set SDA as input
    GIE = 1;  // Enable global interrupts
    PEIE = 1;  // Enable peripheral interrupts
    SSPIF = 0;  // Clear SSP interrupt flag
    SSPIE = 1;  // Enable SSP (I2C) interrupt
}