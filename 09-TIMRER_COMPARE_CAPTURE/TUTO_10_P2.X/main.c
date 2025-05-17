/* File: Compare-Mode.c
 * Author: Marwen Maghrebi
 * Description: Configures PIC microcontroller for compare mode using CCP1 module.
 *Using the Compare mode to generate a 1Hz square wave is a practical application commonly
 *found in various electronic devices and systems.
 */
 
#include <xc.h>
#include <stdint.h>
 
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
// Frequency settings for 1Hz square wave
#define _XTAL_FREQ 4000000      // Assuming 4MHz crystal oscillator
#define TIMER1_PRESCALE 1
#define COMPARE_VALUE ((_XTAL_FREQ / (4 * TIMER1_PRESCALE)) - 1) // Calculation for 1 second
 
// Function prototypes
void Compare_Init();
 
// Main function
void main(void) {
    // Configure IO Ports
    TRISC4 = 0; // Set The Output Pin For The Square Wave
    RC4 = 0;    // Initially OFF
 
    // Initialize Compare mode
    Compare_Init();
 
    // Main Loop
    while (1) {
        // Stay Idle, everything is taken care of in the ISR
    }
}
 
// Function to initialize Compare mode
void Compare_Init() {
    // Configure Timer1 Module to Operate In Timer Mode
    TMR1 = 0;
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    TMR1CS = 0; // Timer mode
    TMR1ON = 1;
 
    // Configure CCP1 Module to Operate in Compare Mode
    // Preload The CCPR1 Register with the compare value for 1 second
    CCPR1 = COMPARE_VALUE;
    // CCP in Compare Mode, CCPx Pin Is Unchanged & Trigger Special Event
    CCP1M0 = 1;
    CCP1M1 = 1;
    CCP1M2 = 0;
    CCP1M3 = 1;
 
    // Enable CCP1 Interrupt
    CCP1IE = 1;
    PEIE = 1;
    GIE = 1;
}
 
// Interrupt Service Routine for CCP1
void __interrupt() ISR() {
    if (CCP1IF) {
        // Toggle The Output Pin to generate a square wave
        RC4 = ~RC4;
 
        // Clear The Interrupt Flag Bit
        CCP1IF = 0;
 
        // Reset the Timer1 value
        TMR1 = 0;
    }
}