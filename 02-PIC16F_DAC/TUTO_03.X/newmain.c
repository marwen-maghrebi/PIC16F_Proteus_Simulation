/* File:   main.c
 * Author: Marwen Maghrebi
 * 
 *
 * Description:
 * This program demonstrates the generation of sinusoidal and triangular waveforms using a PIC microcontroller (PIC16F877A).
 * The waveform type is selected based on the state of a switch connected to RC0. When the switch is OFF, the program generates
 * a sinusoidal waveform using a predefined lookup table. Conversely, when the switch is ON, it generates a triangular waveform.
 * The waveforms are output through the DAC (Digital-to-Analog Converter) pin, configured as PORTB, to visualize them using
 * external instrumentation or to drive analog components. The program allows for easy modification of waveform characteristics
 * and switch inputs for versatile waveform generation applications.
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
 
 
#define DAC_OUT PORTB
#define _XTAL_FREQ 4000000
 
// Define the sine wave lookup table with 64 samples
uint8_t SineTable[64] = {
    128, 139, 150, 161, 171, 181, 191, 199,
    207, 214, 220, 225, 229, 232, 235, 236,
    237, 236, 235, 232, 229, 225, 220, 214,
    207, 199, 191, 181, 171, 161, 150, 139,
    128, 117, 106, 95, 85, 75, 65, 57,
    49, 42, 36, 31, 27, 24, 21, 20,
    19, 20, 21, 24, 27, 31, 36, 42,
    49, 57, 65, 75, 85, 95, 106, 117
};
 
// Define the triangular wave lookup table with 64 samples
uint8_t TriangleTable[64] = {
    0, 4, 8, 12, 16, 20, 24, 28,
    32, 36, 40, 44, 48, 52, 56, 60,
    64, 68, 72, 76, 80, 84, 88, 92,
    96, 100, 104, 108, 112, 116, 120, 124,
    128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188,
    192, 196, 200, 204, 208, 212, 216, 220,
    224, 228, 232, 236, 240, 244, 248, 252
};
 
void main(void) {
    // Configure PORTB as output for DAC
    TRISB = 0x00;
 
    // Configure RC0 as input for switch
    TRISCbits.TRISC0 = 1;
 
    // Initialize index variable
    uint8_t i = 0;
 
    // Initialize switch state variable
    uint8_t switchState = 0;
 
    // Main loop
    while(1) {
        // Read switch state
        switchState = PORTCbits.RC0;
 
        // Generate sinusoidal or triangular waveform based on switch state
        if (switchState == 0) {
            // Output the sine wave value to DAC
            DAC_OUT = SineTable[i++];
        } else {
            // Output the triangular wave value to DAC
            DAC_OUT = TriangleTable[i++];
        }
 
        // Delay for the desired waveform frequency
        __delay_us(390); // Adjust delay for desired frequency
 
        // Reset index if end of waveform table is reached
        if (i == 64)
            i = 0;
    }
 
    return;
}