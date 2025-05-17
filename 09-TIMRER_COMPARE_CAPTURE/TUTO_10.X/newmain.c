/* File: Capture-Mode.c
 * Author: Marwen Maghrebi
 * Description: Configures PIC microcontroller for capture mode using CCP1 module.
 *              This code sets up the PIC microcontroller to operate in capture mode
 *              using the CCP1 module. It captures events triggered by an external
 *              signal and toggles an LED connected to pin RC3 when the captured value
 *              reaches 9.The captured value is also displayed on PORTB, and the current count of
 *              Timer1 is displayed on PORTD. The LED on RC3 serves as a visual indicator
 *              of the captured event.
 */
 
#include <xc.h>
#include <stdint.h>
 
// Configuration
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
// Global variable
uint8_t captureCount = 0;
 
// Main function
void main(void) {
    // Configure IO Ports
    TRISC3 = 0; // LED pin (now on RC3) as output
    RC3 = 0;    // Initially OFF
 
    TRISB = 0x00; // Output Port for Capture Operation (CCPR1 register)
    PORTB = 0x00; // Initial State
 
    TRISD = 0x00; // Output Port for TMR1 Module (TMR1 register)
    PORTD = 0x00; // Initial State
 
    // Configure Timer1 Module to Operate in Counter Mode
    TMR1 = 0;
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    TMR1CS = 1; // Counter mode
    T1OSCEN = 1;
    T1SYNC = 0;
    TMR1ON = 1;
 
    // Configure CCP1 Module to Operate in Capture Mode
    CCP1M0 = 1;
    CCP1M1 = 0;
    CCP1M2 = 1;
    CCP1M3 = 0;
 
    // Enable CCP1 Interrupt
    CCP1IE = 1;
    PEIE = 1;
    GIE = 1;
 
    // Main Loop
    while (1) {
        // Read & Print Out the TMR1 Counts
        PORTD = TMR1L;
    }
}
 
// ISR Handler
void __interrupt() ISR() {
    if (CCP1IF) {
        // Capture event occurred, read CCPR1 value
        uint16_t capturedValue = CCPR1L | (CCPR1H << 8);
        
        if (capturedValue == 9) {
            // Toggle the LED (now on RC3)
            RC3 = ~RC3;
        }
        
 
        // Write CCPR1 value to PORTB
        PORTB = capturedValue;
 
        // Clear the Interrupt Flag Bit
        CCP1IF = 0;
    }
}