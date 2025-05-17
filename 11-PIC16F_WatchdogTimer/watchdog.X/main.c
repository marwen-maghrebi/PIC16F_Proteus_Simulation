#include <xc.h>

// Configuration bits
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (Low-voltage programming disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000      // Assume 4MHz crystal frequency, adjust if different

// Function to create a longer delay while still clearing the watchdog timer
void long_delay_with_wdt(unsigned int delay_ms) {
    unsigned int i;
    for (i = 0; i < delay_ms; i++) {
        __delay_ms(1);          // 1 ms delay
        CLRWDT();  // Clear the watchdog timer every millisecond
    }
}

void main() {
    // Set up the watchdog timer prescaler (1:32)
    OPTION_REGbits.PSA = 0;     // Prescaler is assigned to WDT
    OPTION_REGbits.PS = 0b100;  // Prescaler ratio 1:32

    TRISB = 0;                  // All PORTB pins are configured as outputs
    
    while(1) {
        PORTB = 0x0F;           // Set PORTB to 0x0F (00001111 in binary)
        long_delay_with_wdt(1000);  // 1 second delay while clearing WDT
        
        PORTB = 0xF0;           // Set PORTB to 0xF0 (11110000 in binary)
        long_delay_with_wdt(1000);  // 1 second delay while clearing WDT
    }
}