/* File:   main.c
 * Author: PC-MAGHREBI
 * Created on 15 May 2024, 15:12
 *
 * Description:
 * This project demonstrates controlling four LEDs using the PIC16F877A microcontroller.
 * The project utilizes the Analog-to-Digital Converter (ADC) to read analog signals from a sensor
 * and lights up the LEDs based on the ADC values. The ADC results are checked against predefined
 * thresholds to determine which LEDs to turn on, providing a simple method for visualizing analog
 * input values.
 */
 
#include <xc.h>
#include <stdint.h> // Include stdint.h for uint16_t data type
 
#define _XTAL_FREQ 4000000 // Define the crystal frequency in Hertz
 
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
// Function prototypes
void configure_ADC(void);
void start_ADC_conversion(void);
void wait_for_conversion(void);
void read_ADC_result(void);
 
// Main function
void main(void) {
    // Initialize ports for LEDs
    TRISBbits.TRISB0 = 0; // Set RB0 as output
    TRISBbits.TRISB1 = 0; // Set RB1 as output
    TRISBbits.TRISB2 = 0; // Set RB2 as output
    TRISBbits.TRISB3 = 0; // Set RB3 as output
 
    // Turn off all LEDs initially
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
 
    // Configure ADC module
    configure_ADC();
    
    while (1) {
        // Start ADC conversion
        start_ADC_conversion();
        
        // Wait for conversion to complete
        wait_for_conversion();
        
        // Read ADC result and control LEDs
        read_ADC_result();
        
        // Add a delay between ADC conversions to ensure proper timing
        __delay_ms(1000); // Adjust delay time as needed
    }
}
 
// Function to configure the ADC module
void configure_ADC(void) {
    ADCON0 = 0x41;  // Turn ADC ON, select AN0 channel, ADC clock = Fosc/8
    ADCON1 = 0x80;  // All 8 channels are analog, result is "right-justified"
                    // ADC clock = Fosc/8
}
 
// Function to start ADC conversion
void start_ADC_conversion(void) {
    ADCON0bits.GO_DONE = 1; // Start A/D conversion
}
 
// Function to wait for ADC conversion to complete
void wait_for_conversion(void) {
    while (ADCON0bits.GO_DONE); // Polling GO_DONE = delay until conversion is complete
}
 
// Function to read ADC result and control LEDs
void read_ADC_result(void) {
    uint16_t ADC_result = ((uint16_t)ADRESH << 8) + ADRESL; // Read the right-justified 10-bit result
    
    // Turn off all LEDs initially
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
    
    // Check ADC result and control LEDs accordingly
    if (ADC_result > 250) {
        // ADC result is greater than 250, turn on LED1
        PORTBbits.RB0 = 1;
    }
    if (ADC_result > 500) {
        // ADC result is greater than 500, turn on LED2
        PORTBbits.RB1 = 1;
    }
    if (ADC_result > 750) {
        // ADC result is greater than 750, turn on LED3
        PORTBbits.RB2 = 1;
    }
    if (ADC_result > 1000) {
        // ADC result is greater than 1000, turn on LED4
        PORTBbits.RB3 = 1;
    }
}
