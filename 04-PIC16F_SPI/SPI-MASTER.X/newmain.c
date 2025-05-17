/** File: main.c
* Author: Marwen Maghrebi
* 
* Description: This code demonstrates SPI communication using a PIC16F877A microcontroller as the master device. 
* It allows incrementing or decrementing a data value using push buttons (UP and Down), and then sending this 
* data value via SPI when another button (Send) is pressed. The current data value is displayed on PORTD.
* For more information, visit My Blog at https://theembeddedthings.com/
*/
 // Configuration bits
#pragma config FOSC = HS    // High-speed oscillator
#pragma config WDTE = OFF   // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable (PWRT disabled)
#pragma config BOREN = ON   // Brown-out Reset Enable (BOR enabled)
#pragma config LVP = OFF    // Low-Voltage (single-supply) In-Circuit Serial Programming Enable (LVP disabled)
#pragma config CPD = OFF    // Data EEPROM Memory Code Protection (Data EEPROM code protection off)
#pragma config WRT = OFF    // Flash Program Memory Write Enable (Write protection off)
#pragma config CP = OFF     // Flash Program Memory Code Protection (Code protection off)

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000
 
// IO Pins Definitions
#define UP   RB0
#define Down RB1
#define Send RB2
 
// Function Declarations
void SPI_Master_Init();
void SPI_Write(uint8_t);
 
// Main Routine
void main(void)
{
  // Peripherals & IO Configurations
  SPI_Master_Init(); // Initialize SPI as Master @ Fosc/64 SCK
  uint8_t Data = 0; // Data Byte
  TRISB = 0x07;     // RB0, RB1 & RB2: Input Pins (Push Buttons)
  TRISD = 0x00;     // Output Port (4-Pins)
  PORTD = 0x00;     // Initially OFF
  
  while(1)
  {
    // Increment Data Value
    if (UP)
    {
      Data++;
      __delay_ms(250);
    }
    // Decrement Data Value
    if (Down)
    {
      Data--;
      __delay_ms(250);
    }
    // Send Data Value Via SPI
    if (Send)
    {
      SPI_Write(Data);
      __delay_ms(250);
    }
    PORTD = Data; // Display Current Data Value @ PORTD
  }
  return;
}
 
// Function Definitions
void SPI_Master_Init()
{
  // Set SPI Mode To Master + Set SCK Rate To Fosc/64
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 0;
  SSPM3 = 0;
  // Enable The Synchronous Serial Port
  SSPEN = 1;
  // Configure Clock Polarity & Phase (SPI Mode Num. 1)
  CKP = 0;
  CKE = 0;
  // Configure Sampling Time (Middle)
  SMP = 0;
  // Configure IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 0; // SCK -> Output
}
 
void SPI_Write(uint8_t Data)
{
  SSPBUF = Data; // Transfer Data To Buffer Register
  // while(!SSPSTATbits.BF); // Uncomment if Not Using Interrupts!
  // The Above While Loop Protects Against Writing To Buffer
  // Before Previous Transmission Ends
}