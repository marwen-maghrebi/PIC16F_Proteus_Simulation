/* File:   main.c
 * Author: Marwen Maghrebi
 *
 * Description:
 * This project demonstrates controlling two LEDs and a relay using the PIC16F877A microcontroller 
 * based on the Timer0 module. The Timer0 is configured to count external clock pulses from a button 
 * connected to the RA4/T0CKI pin. As the Timer0 value increments, the LEDs and relay are triggered 
 * at specific thresholds, providing visual feedback on the count value. The project showcases 
 * simple external clock counting with thresholds for controlling outputs.
 */


#include <xc.h>
#include <stdint.h>

// Configuration bits
#pragma config FOSC = HS        // High Speed Crystal/Resonator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection disabled
#pragma config WRT = OFF        // Flash Program Memory Write Protection disabled
#pragma config CP = OFF         // Flash Program Memory Code Protection disabled

#define _XTAL_FREQ 4000000      // 4 MHz Crystal

#define BUTTON PORTAbits.RA4    // Button connected to RA4/T0CKI
#define LED1   PORTBbits.RB0    // LED1 connected to RB0
#define LED2   PORTBbits.RB1    // LED2 connected to RB1
#define RELAY  PORTBbits.RB2    // Relay connected to RB2

#define COUNT_THRESHOLD1 5      // First threshold for LED1
#define COUNT_THRESHOLD2 10     // Second threshold for LED2
#define COUNT_THRESHOLD3 15     // Third threshold for Relay

void main(void) {
    // Configure ports
    TRISA = 0xFF;  // Set all PORTA pins as inputs
    TRISB = 0;     // Set all PORTB pins as outputs
    
    // Initialize PORTB
    PORTB = 0;

    // Configure TMR0
    OPTION_REGbits.T0CS = 1;  // TMR0 Clock Source is from T0CKI pin (RA4)
    OPTION_REGbits.T0SE = 0;  // Increment on low-to-high transition on RA4/T0CKI pin
    OPTION_REGbits.PSA = 1;   // Prescaler is not assigned to the Timer0 module

    TMR0 = 0;  // Clear TMR0

    uint8_t prevTMR0 = 0;  // To detect TMR0 changes

    while(1) {
        // Check if TMR0 has changed
        if (TMR0 != prevTMR0) {
            prevTMR0 = TMR0;  // Update prevTMR0
            
            // Check thresholds and update outputs
            if (TMR0 >= COUNT_THRESHOLD1) {
                LED1 = 1;  // Turn on LED1
            } else {
                LED1 = 0;  // Turn off LED1
            }
            
            if (TMR0 >= COUNT_THRESHOLD2) {
                LED2 = 1;  // Turn on LED2
            } else {
                LED2 = 0;  // Turn off LED2
            }
            
            if (TMR0 >= COUNT_THRESHOLD3) {
                RELAY = 1;  // Activate relay
            } else {
                RELAY = 0;  // Deactivate relay
            }
        }
        
        // Optional: Add a small delay to debounce the button
        __delay_ms(10);
    }
}