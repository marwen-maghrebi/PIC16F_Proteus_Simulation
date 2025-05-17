#include <xc.h>

// Configuration bits: Adjust these according to your specific needs
// This is a basic configuration, you may need to modify it
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000      // Assume 4MHz crystal frequency, adjust if different

// Function prototypes
void EEPROM_Write(unsigned char address, unsigned char data);
unsigned char EEPROM_Read(unsigned char address);

void main() {
    ADCON1 = 0x06;  // Configure all pins as digital I/O
    
    TRISB = 0x00;   // Set all PORTB pins as outputs
    TRISD = 0x00;   // Set all PORTD pins as outputs
    TRISA = 0xFF;   // Set all PORTA pins as inputs
    
    PORTB = 0;      // Initialize PORTB
    PORTD = 0;      // Initialize PORTD
    
    PORTD = EEPROM_Read(5);  // Read EEPROM memory at address 5
    
    while(1) {
        PORTB++;            // Increment PORTB
        __delay_ms(100);    // 100ms delay
        
        if (PORTAbits.RA2) {  // If MEMO button (connected to RA2) is pressed
            EEPROM_Write(5, PORTB);  // Save PORTB value to EEPROM
            PORTD = EEPROM_Read(5);  // Read written data and display on PORTD
            
            while(PORTAbits.RA2);    // Wait for button release
            __delay_ms(20);          // Simple debounce
        }
    }
}

void EEPROM_Write(unsigned char address, unsigned char data) {
    while(EECON1bits.WR);    // Wait for write to complete
    EEADR = address;         // Set up address register
    EEDATA = data;           // Set up data register
    EECON1bits.EEPGD = 0;    // Point to Data memory
    EECON1bits.WREN = 1;     // Enable writes
    
    INTCONbits.GIE = 0;      // Disable interrupts
    EECON2 = 0x55;           // Write 55h
    EECON2 = 0xAA;           // Write AAh
    EECON1bits.WR = 1;       // Set WR bit to begin write
    INTCONbits.GIE = 1;      // Enable interrupts
    
    EECON1bits.WREN = 0;     // Disable writes
}

unsigned char EEPROM_Read(unsigned char address) {
    EEADR = address;         // Set up address register
    EECON1bits.EEPGD = 0;    // Point to Data memory
    EECON1bits.RD = 1;       // EEPROM Read
    return EEDATA;           // Return data
}