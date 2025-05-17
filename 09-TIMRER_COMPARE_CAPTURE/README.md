# PIC16F877 CCP Applications: Capture and Compare Mode Projects

This document covers two independent projects demonstrating the use of **CCP1 (Capture/Compare/PWM)** features of the **PIC16F877A microcontroller**:

1. **Capture Mode** — Detects external events and toggles an LED on specific capture values  
2. **Compare Mode** — Generates a 1Hz square wave output using a timer and compare match logic

Each project focuses on different use cases, providing practical insight into real-time signal handling and timing generation.

---

## 📘 Project 1: CCP Capture Mode – Event-Triggered LED Toggle

### Description  
This project configures the **CCP1 module in Capture Mode** to detect external events. When the captured value equals **9**, it toggles an LED connected to **RC3**. The captured value is displayed on **PORTB**, and the real-time value of **Timer1** is displayed on **PORTD**.

### Hardware Requirements  
![Capture Mode Circuit](circuit_capture.png)  
- PIC16F877A Microcontroller  
- LED (connected to RC3)  
- External signal source (pulse to CCP1)  
- 4MHz Crystal Oscillator + 2x 22pF Capacitors  
- 5V Power Supply

### Circuit Overview  
- **Input**: CCP1 pin (for external signal)  
- **Output**:  
  - RC3 → LED (toggles on captured value == 9)  
  - PORTB → Captured CCPR1 value  
  - PORTD → TMR1 current count  

### Software Flow  
- **Timer1** is configured in **Counter Mode**, counting external events  
- **CCP1** captures the TMR1 value on an incoming pulse  
- If the value = 9 → LED toggles  
- Captured value is sent to PORTB  
- TMR1 value is continuously shown on PORTD

### Benefits  
- Demonstrates real-time event detection  
- Provides visual and port-based debugging  
- Practical for input signal analysis and measurement systems

---

## 📘 Project 2: CCP Compare Mode – 1Hz Square Wave Generator

### Description  
This project configures the **CCP1 module in Compare Mode** to generate a **1Hz square wave** by toggling the **RC4** pin. Timer1 is reset on every compare match to maintain accurate timing.

### Hardware Requirements  
- PIC16F877A Microcontroller  
- LED or Oscilloscope (connected to RC4)  
- 4MHz Crystal Oscillator + 2x 22pF Capacitors  
- 5V Power Supply

### Circuit Overview  
- **Output**:  
  - RC4 → 1Hz square wave (toggled via CCP1 match)  

### Software Flow  
- **Timer1** is configured in **Timer Mode** with no prescaler  
- **CCP1** is set in Compare Mode  
- CCPR1 is loaded with a value that triggers an event every 1 second  
- On interrupt, RC4 toggles and TMR1 resets  

### Benefits  
- Generates accurate time-based square waves  
- Useful for system clocks, heartbeat signals, or blinking indicators  
- Offloads timing responsibility to hardware (no busy loops)

---

## Common Software Tools  
- **MPLAB X IDE** (v5.50+)  
- **XC8 Compiler** (v2.36+)  
- **Proteus 8.15+** (for simulation and testing)

---

## Troubleshooting Guide

| Symptom                     | Cause                            | Fix                                |
|-----------------------------|-----------------------------------|-------------------------------------|
| LED does not toggle         | Capture or Compare not triggered | Check input source or compare value |
| PORTB or PORTD show zeros   | Timer misconfigured              | Ensure correct TMR1 settings        |
| No waveform on RC4          | ISR not executing                | Confirm interrupts and CCP config   |
| Wrong square wave frequency | Incorrect compare value          | Recalculate based on oscillator     |

---

## License  
**MIT License** — Free to use with attribution
