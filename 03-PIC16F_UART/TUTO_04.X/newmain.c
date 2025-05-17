/* File:   main.c
 * Author: Marwen Maghrebi
 * 
 * Description:
 * This program demonstrates UART communication between a microcontroller and a computer terminal.
 * Updated to use 16 MHz clock frequency instead of 8 MHz.
 */
#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 16000000  // Changed from 8000000 to 16000000 MHz
 
// Configuration bits (updated for 16 MHz crystal)
#pragma config FOSC = HS    // High-speed oscillator
#pragma config WDTE = OFF   // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable (PWRT disabled)
#pragma config BOREN = ON   // Brown-out Reset Enable (BOR enabled)
#pragma config LVP = OFF    // Low-Voltage (single-supply) In-Circuit Serial Programming Enable (LVP disabled)
#pragma config CPD = OFF    // Data EEPROM Memory Code Protection (Data EEPROM code protection off)
#pragma config WRT = OFF    // Flash Program Memory Write Enable (Write protection off)
#pragma config CP = OFF     // Flash Program Memory Code Protection (Code protection off)
 
// Define LED pins
#define LED1 RA0 // LED1 on RA0
#define LED2 RA1 // LED2 on RA1
 
// Function to initialize UART transmission
void UART_TX_Init(void)
{
    // Recalculate baud rate generator for 16 MHz
    // For 9600 bps: SPBRG = (Fosc / (16 * Baud Rate)) - 1
    // (16000000 / (16 * 9600)) - 1 = 103
    BRGH = 1;       // High-speed baud rate
    SPBRG = 103;    // Updated SPBRG value for 16 MHz clock
    
    // Enable Asynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    
    // Set RX-TX Pins to be in UART mode
    TRISC6 = 0;     // TX pin (output)
    TRISC7 = 1;     // RX pin (input)
    
    // Enable UART Transmission
    TXEN = 1;
    // Enable UART Reception
    CREN = 1;
}
 
// Function to check if UART transmitter buffer is empty
uint8_t UART_TX_Empty()
{
    // Check if the output (transmitter) buffer is empty
    return TRMT;
}
 
// Function to transmit a single character over UART
void UART_Write(uint8_t data)
{
    // Wait until the transmitter buffer is empty
    while (!UART_TX_Empty());
    
    // Write data to the transmitter buffer
    TXREG = data;
}
 
// Function to transmit a string over UART
void UART_Write_Text(const char* text)
{
    while (*text != '\0') {
        UART_Write(*text++);
    }
}
 
// Function to receive a single character over UART
uint8_t UART_Read(void)
{
    // Wait until data is received
    while (!RCIF);
    
    // Read and return the received data
    return RCREG;
}
 
// Function to receive a string over UART until Enter key is pressed
void UART_Read_Text(char* buffer, uint8_t max_length)
{
    uint8_t i = 0;
    char received_char;
    
    // Receive characters until Enter key is pressed or buffer is full
    do {
        received_char = UART_Read();
        if (received_char != '\r' && received_char != '\n') {
            buffer[i++] = received_char;
        }
    } while (received_char != '\r' && received_char != '\n' && i < max_length - 1);
    
    // Null-terminate the string
    buffer[i] = '\0';
}
 
// Function to initialize LEDs
void LED_Init(void)
{
    TRISA0 = 0; // Set RA0 as output (LED1)
    TRISA1 = 0; // Set RA1 as output (LED2)
    LED1 = 0;   // Turn off LED1 initially
    LED2 = 0;   // Turn off LED2 initially
}
 
// Main function
void main()
{
    // Initialize UART and LEDs
    UART_TX_Init();
    LED_Init();
    
    // Buffer to store received data
    char received_data[50];
 
    while (1) {
        // Transmit prompt message over UART
        UART_Write_Text("Please write your message and press enter \n");
       
        // Blink LED1 to indicate waiting for data
        LED1 = 1;
        __delay_ms(500);
        LED1 = 0;
      
        // Receive data via UART
        UART_Read_Text(received_data, sizeof(received_data));
        
        // Blink LED2 once to indicate data received
        LED2 = 1;
        __delay_ms(500);
        LED2 = 0;
        
        // Transmit received data back over UART with professional formatting
        UART_Write_Text("\r\n");
        UART_Write_Text("*********************************** \r\n");
        UART_Write_Text("Your message is: \n");
        UART_Write_Text(received_data);
        UART_Write_Text("\r\n");
        UART_Write_Text("********************************** \r\n");
    }
}