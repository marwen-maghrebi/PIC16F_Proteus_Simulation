/* File:   main.c
 * Author: Marwen Maghrebi
 * For More Information Visit My Blogue https://theembeddedthings.com/
 *
 * Description:
 * This program demonstrates basic SPI communication in slave mode using a PIC microcontroller.
 * The received data is displayed on PORTB to validate successful communication.
 * An interrupt service routine (ISR) is used to handle SPI data reception.
 */
 
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
 
#include <xc.h>
#include <stdint.h>
 
// Function Prototypes
void SPI_Slave_Init(void);
uint8_t SPI_Read(void);
 
// Global Variable to Store Received Data
uint8_t Data; // Not recommended to use global variables for actual projects. Used here for demonstration purposes only.
 
// Interrupt Service Routine
void __interrupt() ISR(void)
{
    if(SSPIF)
    {
        Data = SSPBUF;  // Read the received data from the buffer
        SSPIF = 0;      // Clear the interrupt flag
    }
}
 
// Main Function
void main(void)
{
    SPI_Slave_Init(); // Initialize SPI in slave mode
    TRISB = 0x00;     // Set PORTB as output to display received data
 
    while(1)
    {
        // Display the received data on PORTB
        PORTB = Data;
    }
 
    // Return statement included to avoid compilation warnings. Not required in practice due to infinite loop.
    return;
}
 
// Function to Initialize SPI in Slave Mode
void SPI_Slave_Init(void)
{
    // Set SPI mode to Slave with SS enabled
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 1;
    SSPM3 = 0;
 
    // Enable the synchronous serial port
    SSPEN = 1;
 
    // Configure the clock polarity and phase (SPI Mode 1)
    CKP = 0;
    CKE = 0;
 
    // Clear the SMP bit
    SMP = 0;
 
    // Configure the I/O pins for SPI slave mode
    TRISC5 = 0; // SDO -> Output
    TRISC4 = 1; // SDI -> Input
    TRISC3 = 1; // SCK -> Input
    PCFG3 = 0;  // Set SS (RA5/AN4) to be digital I/O
    PCFG2 = 1;
    PCFG1 = 0;
    PCFG0 = 0;
    TRISA5 = 1; // SS -> Input
 
    // Enable SPI interrupt
    SSPIE = 1;
    PEIE = 1;
    GIE = 1;
}
 
// Function to Read Data from SPI Buffer
uint8_t SPI_Read(void)
{
    uint8_t data = 0;
    if(BF) // Check if any new data is received
    {
        data = SSPBUF;  // Read the buffer
        BF = 0;         // Clear the buffer-full indicator bit
        SSPIF = 0;      // Clear the interrupt flag bit
        SSPOV = 0;      // Clear the overflow indicator bit
    }
    return data;
}