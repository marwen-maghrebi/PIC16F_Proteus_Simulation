/* File: main.c
 * Author: Marwen Maghrebi
 * Description: This project demonstrates how to control the brightness and color of an RGB LED using PWM 
 * (Pulse Width Modulation) on the PIC16F877A microcontroller. The microcontroller is set up to produce PWM signals t
 * hat control the RGB LED's colors through different duty cycles, creating a smooth fading effect.
 */
 
// Configuration bits for PIC16F877A
#pragma config FOSC = XT        // XT Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Disable
#pragma config PWRTE = OFF      // Power-up Timer Disable
#pragma config BOREN = ON       // Brown-out Reset Enable
#pragma config LVP = OFF        // Low-Voltage Programming Disable
#pragma config CPD = OFF        // Data EEPROM Code Protection Disable
#pragma config WRT = OFF        // Flash Write Protection Disable
#pragma config CP = OFF         // Flash Code Protection Disable
 
#include <xc.h>
#include <stdint.h>
 
#define _XTAL_FREQ 4000000  // Define the operating frequency of the microcontroller
 
// Define pin assignments for RGB LEDs
#define REDPin   RC0  // Red LED pin
#define GREENPin RC1  // Green LED pin
#define BLUEPin  RC2  // Blue LED pin
 
/**
 * Initialize Timer2 for PWM
 */
void PWM_Init() {
    PR2 = 255;  // Set period register for PWM period
    T2CKPS0 = 0;  // Set prescaler to 1:1
    T2CKPS1 = 0;
    TMR2ON = 1;  // Turn on Timer2
    CCP1M3 = 1;  // Configure CCP1 module for PWM mode
    CCP1M2 = 1;
}
 
/**
 * Set PWM duty cycle for each color component
 * 
 * @param red_intensity: The duty cycle for the red LED (0-255)
 * @param green_intensity: The duty cycle for the green LED (0-255)
 * @param blue_intensity: The duty cycle for the blue LED (0-255)
 */
void RGB_LED_Control(uint8_t red_intensity, uint8_t green_intensity, uint8_t blue_intensity) {
    CCPR1L = red_intensity;   // Set duty cycle for red LED using CCP1
    CCPR2L = green_intensity; // Set duty cycle for green LED using CCP2
    // Note: No CCP module available for the blue LED; additional hardware or software PWM needed.
}
 
/**
 * Main function
 */
void main(void) {
    // Configure RGB LED pins as output
    TRISC0 = 0; // Red LED
    TRISC1 = 0; // Green LED
    TRISC2 = 0; // Blue LED
 
    // Initialize PWM
    PWM_Init();
 
    // Main program loop
    while(1) {
        // Gradually increase the intensity of the red LED
        for (int i = 0; i <= 255; i++) {
            RGB_LED_Control(i, 0, 0);
            __delay_ms(10); // Delay for smooth transition
        }
        // Gradually increase the intensity of the green LED
        for (int i = 0; i <= 255; i++) {
            RGB_LED_Control(0, i, 0);
            __delay_ms(10); // Delay for smooth transition
        }
    }
}