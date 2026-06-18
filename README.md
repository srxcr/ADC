# PIC18F4550 ADC to LCD Display Interface

## 💡 Project Description

This project demonstrates how to interface the ADC module of a PIC18F4550 microcontroller with a 16x2 LCD. The analog input voltage applied to RA0 (AN0) is converted into a digital value using the built-in ADC and displayed as a three-digit decimal number on the LCD screen.

---

## 🛠️ Hardware Components

* PIC18F4550 Microcontroller
* 16x2 Character LCD (LM016L or equivalent)
* 1kΩ Potentiometer
* 5V Power Supply
* Connecting Wires
* Proteus Simulation Software (Optional)

### Circuit Connections

* LCD Data Pins (D0–D7) → PORTD (RD0–RD7)
* LCD RS Pin → RC0
* LCD EN Pin → RC1
* LCD RW Pin → GND
* Analog Input → RA0 (AN0)
* Potentiometer Wiper → RA0 (AN0)

---

## 💻 Software Requirements

* MPLAB X IDE
* XC8 Compiler
* PIC18F4550 Header Files
* Embedded C Programming Language

---

## 🚀 How to Use

1. Open the project in MPLAB X IDE.
2. Copy the source code into a new project.
3. Compile the code using the XC8 compiler.
4. Generate the HEX file.
5. Load the HEX file into the PIC18F4550.
6. Connect the circuit according to the schematic.
7. Power the circuit and observe the ADC value displayed on the LCD.

---

## ✨ Features

* ADC conversion using PIC18F4550
* LCD interfacing through PORTD
* Real-time display of ADC values
* Simple and beginner-friendly Embedded C code
* Suitable for learning ADC and LCD interfacing concepts

---


## 📸 Circuit Diagram
![image alt](https://github.com/manubraj/PIC-Microcontroller-ADC-to-LCD-Project/blob/a4d539b627b5499f9e04a47c3be94f372bcf1327/ACESS%20GRANTED.png)

## ⚠️ Notes

* The program reads the ADC result from the ADRESH register.
* Delay values may need adjustment depending on the oscillator frequency.
* ADC accuracy depends on the reference voltage and power supply stability.
* This implementation displays only the upper 8 bits of the ADC result.

---

## 📚 Applications

* ADC Learning Projects
* Sensor Monitoring Systems
* Voltage Measurement Applications
* Embedded Systems Education
* PIC Microcontroller Practice Projects

---

## 📄 License

This project is intended for educational and learning purposes. Feel free to modify and improve the code for academic and personal use.

---

## 🙏 Acknowledgements

Developed and tested using:

* PIC18F4550 Microcontroller
* MPLAB X IDE
* XC8 Compiler
* Proteus Design Suite
* LM016L 16x2 LCD Module
