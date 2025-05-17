/* File:   main.c
 * Author: PC-MAGHREBI
 * Created on 15 May 2024, 15:12
 *
 * Description:
 * This project demonstrates controlling two LEDs using the PIC16F877A microcontroller. 
 * The project showcases both current sourcing and current sinking configurations, 
 * along with the use of input pins with pull-up and pull-down resistors.
 */
 
// Configuration Bits
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
// Include necessary libraries
#include <xc.h>
 
// Define crystal frequency
#define _XTAL_FREQ 8000000
 
void main() {
    // Configure I/O pins
    TRISB0 = 0; // Set RB0 as output for current sourcing (LED1)
    TRISB3 = 0; // Set RB3 as output for current sinking (LED2)
    TRISB1 = 1; // Set RB1 as input with pull-up resistor
    TRISB2 = 1; // Set RB2 as input with pull-down resistor
    
    // Initialize outputs
    RB0 = 0; // Ensure LED1 is off initially
    RB3 = 1; // Ensure LED2 is off initially (active low configuration)
    
    // Main program loop
    while (1) {
        // Check state of RB1 (pull-up configuration)
        if (RB1 == 0) {
            RB0 = 1; // Turn on LED1 (current sourcing)
        } else {
            RB0 = 0; // Turn off LED1
        }
        
        // Check state of RB2 (pull-down configuration)
        if (RB2 == 1) {
            RB3 = 0; // Turn on LED2 (current sinking)
        } else {
            RB3 = 1; // Turn off LED2
        }
    }
}