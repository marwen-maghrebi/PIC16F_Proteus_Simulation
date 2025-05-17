/* File: main.c
* Author: Marwen Maghrebi
* 
* Description: 
* This code demonstrates interrupt-driven ADC reading and UART communication on a PIC microcontroller.
* It blinks LEDs, reads analog voltage from a potentiometer, and sends the voltage readings via UART.
* Additionally, it handles an external button interrupt to trigger an LED blink and UART message.
*/
 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
 
// Configuration bits
#pragma config FOSC = HS        // High-Speed Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
 
#define _XTAL_FREQ 20000000  // Define oscillator frequency for delay
 
// Function Prototypes
void init_config(void);
void UART_send_string(const char* str);
void __interrupt() ISR(void);
 
void main(void) {
    init_config();
 
    uint16_t adc_value = 0;
    float voltage = 0.0;
    char buffer[32];
 
    while (1) {
        // Blink four LEDs every two seconds
        PORTD = 0x0F;  // Turn on LEDs RD0, RD1, RD2, RD3
        __delay_ms(2000);
        PORTD = 0x00;  // Turn off LEDs
        __delay_ms(2000);
 
        // Read ADC value
        ADCON0bits.GO_DONE = 1;  // Start ADC conversion
        while (ADCON0bits.GO_DONE);  // Wait for conversion to finish
        adc_value = ((uint16_t)(ADRESH << 8)) + ADRESL;  // Cast to uint16_t to avoid warning
 
        // Convert ADC value to voltage
        voltage = (adc_value * 5.0) / 1023.0;  // Assuming Vref = 5V and 10-bit ADC resolution
 
        // Send voltage value via UART
        sprintf(buffer, "Voltage: %.2f V\r\n", voltage);
        UART_send_string(buffer);
    }
}
 
void init_config(void) {
    // Port configurations
    TRISD = 0x00;  // Set PORTD as output for LEDs
    TRISA = 0x01;  // Set RA0 as input for ADC
    TRISB = 0x01;  // Set RB0 as input for button interrupt
 
    // ADC configuration
    ADCON0 = 0x41;  // ADC ON, Channel 0 (RA0/AN0), Fosc/8 as conversion clock
    ADCON1 = 0x8E;  // Right justify result, set Vref+ to Vdd and Vref- to Vss, AN0 as analog, rest as digital
 
    // UART configuration
    TXSTAbits.SYNC = 0;  // Asynchronous mode
    TXSTAbits.BRGH = 1;  // High speed
    SPBRG = 129;  // Baud rate 9600 for 20 MHz
    RCSTAbits.SPEN = 1;  // Enable serial port
    TXSTAbits.TXEN = 1;  // Enable transmission
 
    // Interrupt configuration
    INTCONbits.GIE = 1;  // Enable global interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.INTE = 1;  // Enable RB0/INT external interrupt
    OPTION_REGbits.INTEDG = 1;  // Interrupt on rising edge
 
    // Initialize LEDs state
    PORTD = 0x00;
}
 
void UART_send_string(const char* str) {
    while (*str) {
        while (!TXSTAbits.TRMT);  // Wait until transmit buffer is empty
        TXREG = *str++;  // Transmit character
    }
}
 
void __interrupt() ISR(void) {
    if (INTCONbits.INTF) {
        // External interrupt occurred
        PORTD = 0x00;  // Turn off the four LEDs
        PORTDbits.RD4 = 1;  // Turn on interrupt-specific LED (RD4)
        __delay_ms(500);
        PORTDbits.RD4 = 0;  // Turn off interrupt-specific LED (RD4)
 
        // Send UART message indicating interrupt execution
        UART_send_string("Interrupt executed\r\n");
 
        // Clear the interrupt flag
        INTCONbits.INTF = 0;
    }
}