/* File: main.c
 * Author: Marwen Maghrebi
 * Description:
 * Timer-based LED toggling with UART monitoring.
 * This code initializes multiple interrupt counters and toggles LEDs connected to different pins of 
 * the PIC16F877A microcontroller based on specific time intervals. Additionally, it implements UART 
 * communication to send a message periodically via serial transmission.
 */
 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
 
// Configuration bits (assuming a PIC16F877A microcontroller)
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config DEBUG = OFF
 
// Define the system clock frequency
#define _XTAL_FREQ 8000000 // 8 MHz
 
// Global variables for the interrupt counts
volatile uint8_t interrupt_count1 = 0;
volatile uint16_t interrupt_count2 = 0;
volatile uint16_t interrupt_count3 = 0;
volatile uint16_t interrupt_count4 = 0;
volatile uint16_t interrupt_count5 = 0;
 
// UART initialization function
void UART_Init(void) {
    TRISC6 = 0; // TX pin set as output
    TRISC7 = 1; // RX pin set as input
 
    TXSTAbits.SYNC = 0;  // Asynchronous mode
    TXSTAbits.BRGH = 1;  // High-speed mode
 
    SPBRG = 51; // Baud rate 9600 for 8 MHz clock
 
    RCSTAbits.SPEN = 1;  // Enable serial port
    TXSTAbits.TXEN = 1;  // Enable transmission
}
 
// UART transmit function
void UART_Transmit(char data) {
    while (!TXIF); // Wait until the transmit buffer is empty
    TXREG = data;  // Transmit the data
}
 
// UART send string function
void UART_SendString(const char *str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}
 
void __interrupt() ISR() {
    if (TMR2IF) { // Check if Timer2 overflow interrupt flag is set
        TMR2IF = 0; // Clear the interrupt flag
        
        // Increment the interrupt counts
        interrupt_count1++;
        interrupt_count2++;
        interrupt_count3++;
        interrupt_count4++;
        interrupt_count5++;
        
        // Toggle LEDs based on the specified intervals
        if (interrupt_count1 >= 100) { // 10 seconds (assuming 100 ms overflow time)
            PORTBbits.RB0 ^= 1; // Toggle LED on RB0
            interrupt_count1 = 0;
        }
        if (interrupt_count2 >= 200) { // 20 seconds
            PORTBbits.RB1 ^= 1; // Toggle LED on RB1
            interrupt_count2 = 0;
        }
        if (interrupt_count3 >= 300) { // 30 seconds
            PORTBbits.RB2 ^= 1; // Toggle LED on RB2
            interrupt_count3 = 0;
        }
        if (interrupt_count4 >= 400) { // 40 seconds
            PORTBbits.RB3 ^= 1; // Toggle LED on RB3
            interrupt_count4 = 0;
        }
    }
}
 
void main(void) {
    // Configure RB0, RB1, RB2, and RB3 as output
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    // Turn off LEDs initially
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
 
    // Configure Timer2
    T2CONbits.T2CKPS = 0b11; // Prescaler 1:16
    PR2 = 124; // Load Period Register (PR2)
    TMR2 = 0; // Clear Timer2 register
    T2CONbits.TMR2ON = 1; // Turn on Timer2
 
    // Enable Timer2 interrupt
    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupt
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts
 
    // Initialize UART
    UART_Init();
 
    uint32_t loop_counter = 0;
    char buffer[32];
 
    while (1) {
        // Increment loop counter
        loop_counter++;
        
        // Create the message string
        sprintf(buffer, "LOOP EXECUTE %lu\r\n", loop_counter);
        
        // Send the message via UART
        UART_SendString(buffer);
        
        // Delay to prevent flooding the UART
        __delay_ms(1000);
    }
}