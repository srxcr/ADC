# PIC18F4550 ADC to LCD Display Interface

> **Quick Start:** This project demonstrates real-time analog-to-digital conversion on a PIC18F4550. A potentiometer connected to AN0 (RA0) is sampled by the ADC, and the digital value (0-255) is displayed on a 16×2 LCD in real-time.

## 💡 Project Description

This educational project demonstrates the interface between the Analog-to-Digital Converter (ADC) module of a PIC18F4550 microcontroller and a 16×2 character LCD display. The analog input voltage applied to RA0 (AN0) is converted into a digital value using the on-chip ADC, and the result is displayed in real-time on the LCD. This is an excellent project for learning ADC fundamentals, LCD interfacing, and analog signal processing.

---

## 📁 Repository Structure

```
ADC/
├── main.c                    # Main program with ADC and LCD functions
├── lcd.c / lcd.h            # LCD driver library (if separate)
├── adc.c / adc.h            # ADC configuration and conversion (if separate)
├── README.md                 # This file
└── [Additional source files]
```

---

## 🛠️ Hardware Components

* **PIC18F4550 Microcontroller** (USB-capable variant of PIC18F4520)
* **16×2 Character LCD Display** (LM016L or equivalent, parallel interface)
* **10kΩ Potentiometer** (for analog input variation)
* **10μF Capacitor** (power supply decoupling)
* **10kΩ Resistor** (LCD contrast control)
* **5V Power Supply** (stable, ≥500mA)
* **Connecting Wires** (22 AWG recommended)
* **Breadboard or PCB** (for circuit assembly)

### Circuit Connections

| Component | Connection | Notes |
|-----------|-----------|-------|
| **LCD Data Pins** | PORTD (RD0–RD7) | 8-bit parallel mode |
| **LCD RS Pin** (Register Select) | RC0 | High: Data, Low: Command |
| **LCD EN Pin** (Enable) | RC1 | Pulse to latch data |
| **LCD RW Pin** (Read/Write) | GND | Always write mode |
| **LCD V0 Pin** (Contrast) | 10kΩ pot center | Adjust for optimal display |
| **Analog Input (Channel 0)** | RA0 (AN0) | Primary ADC channel |
| **Potentiometer Wiper** | RA0 (AN0) | Varies 0V–5V |
| **Potentiometer Ends** | +5V and GND | Voltage divider |
| **Reference Voltage (Vref+)** | +5V | Maximum ADC input |
| **Reference Voltage (Vref-)** | GND | Minimum ADC input |

### Detailed Wiring Diagram

```
16×2 LCD Module:
┌──────────────────────────────┐
│ 1  2  3  4  5  6  7  8  9 10 │
│GND +5V V0 RS EN RW D0 D1 D2 D3│
└──────────────────────────────┘
 │   │    │  │  │  │  │  │  │  │
 │   │    │  │  │  │  └──┴──┴──┴──► RD0-RD3 (PORTD lower nibble)
 │   │    │  │  │  └─────────────► GND (Write only)
 │   │    │  │  └────────────────► RC1 (Enable pulse)
 │   │    │  └─────────────────► RC0 (Register Select)
 │   │    └──────────────────► 10kΩ pot center (Contrast)
 │   └─────────────────────► +5V
 └───────────────────────► GND

Potentiometer (Contrast Adjustment):
    +5V
     │
    [10kΩ Pot]
     │ (wiper to LCD pin 3)
    GND

Analog Input (Potentiometer for ADC):
    +5V
     │
    [10kΩ Pot]
     │ (wiper to RA0/AN0)
    GND

PIC18F4550 Connections:
RA0 ────────────── Potentiometer wiper (Analog input, AN0)
RC0 ────────────── LCD RS pin
RC1 ────────────── LCD EN pin
RD0-RD7 ────────── LCD D0-D7 pins
VSS ────────────── GND (common ground)
VDD ────────────── +5V supply
```

---

## 💻 Software Requirements

* **MPLAB X IDE** (v5.0 or later)
* **XC8 Compiler** (v2.0 or later)
* **PIC18F4550 Header Files** (`<xc.h>`)
* **Embedded C Programming Language**
* **Proteus Design Suite** (optional, for simulation)

---

## 🚀 How It Works

### ADC Conversion Process

1. **Initialization:** ADC is configured to use AN0 (RA0) as the input channel
2. **Sampling:** The microcontroller samples the analog voltage at regular intervals
3. **Conversion:** The analog voltage is converted to an 8-bit digital value (0-255)
4. **Display:** The digital value is sent to the LCD via PORTD (RD0-RD7)
5. **Repeat:** The process loops continuously for real-time display

### Voltage-to-Digital Conversion

With a 5V reference:
- **ADC Resolution** (8-bit): 256 levels
- **Step Size:** 5V / 256 = 19.53 mV per count
- **Formula:** Digital Value = (Analog Voltage / 5V) × 255

**Examples:**
- 0V input → Digital value: 0
- 2.5V input → Digital value: ~127
- 5V input → Digital value: 255

---

## 📝 Code Overview

### ADC Configuration

The ADC requires configuration of three registers:

**ADCON0 Register** (ADC Control 0):
```
Bit 7-6: VCFG<1:0> = 00 (VSS/VDD as reference)
Bit 5-2: CHS<3:0> = 0000 (AN0 channel selected)
Bit 1:   GO = 0 (Start conversion bit)
Bit 0:   ADON = 1 (ADC module enable)
```

**ADCON1 Register** (ADC Control 1):
```
Bit 3-0: PCFG<3:0> = 1110 (RA0 as analog input, others digital)
         This configures which pins are analog vs digital
```

**ADCON2 Register** (ADC Control 2):
```
Bit 7:   ADFM = 1 (Right-justified result)
Bit 6-4: ACQT<2:0> = 010 (Acquisition time ~4 TAD)
Bit 3-0: ADCS<3:0> = 0100 (Clock divider for TAD timing)
```

### ADC Initialization Code Example

```c
void ADC_Init(void) {
    ADCON0 = 0x01;  // AN0 selected, ADC enabled
    ADCON1 = 0x0E;  // RA0 as analog input
    ADCON2 = 0xA4;  // Right-justified, TAD timing
    
    // Wait for acquisition time
    __delay_us(20);
}
```

### ADC Conversion Sequence

```c
unsigned char ADC_Read(void) {
    ADCON0bits.GO = 1;           // Start conversion
    while(ADCON0bits.GO);         // Wait for conversion to complete
    return ADRESH;                // Return 8-bit result
}
```

### LCD Initialization

LCD requires:
1. Power-on delay (~15ms)
2. Function set command (8-bit mode, 2 lines, 5×8 font)
3. Display on/off control
4. Clear display
5. Entry mode set

```c
void LCD_Init(void) {
    __delay_ms(15);               // Power-on delay
    LCD_Command(0x38);            // Function set: 8-bit, 2 lines
    LCD_Command(0x0C);            // Display on, cursor off
    LCD_Command(0x01);            // Clear display
    LCD_Command(0x06);            // Entry mode: increment cursor
}
```

### Main Loop Structure

```c
void main(void) {
    unsigned char adc_value;
    
    ADC_Init();
    LCD_Init();
    
    while(1) {
        adc_value = ADC_Read();   // Read ADC
        LCD_DisplayValue(adc_value); // Show on LCD
        __delay_ms(250);          // Update every 250ms
    }
}
```

### Data Display Function

```c
void LCD_DisplayValue(unsigned char value) {
    unsigned char hundreds, tens, ones;
    
    // Convert binary to decimal digits
    hundreds = value / 100;
    tens = (value % 100) / 10;
    ones = value % 10;
    
    // Send to LCD with ASCII offset
    LCD_Data(hundreds + 0x30);
    LCD_Data(tens + 0x30);
    LCD_Data(ones + 0x30);
}
```

---

## ✨ Features

* **Real-Time ADC Conversion** – Continuous sampling and display
* **8-Bit Precision** – 256 discrete levels (0-255)
* **LCD Interfacing** – Parallel 8-bit mode for speed
* **Voltage Measurement** – Direct display of analog input value
* **Simple Embedded C Implementation** – Easy to understand and modify
* **Educational Foundation** – Perfect for learning ADC concepts
* **Modular Code Structure** – Separate ADC and LCD functions

---

## 💡 Understanding ADC (Analog-to-Digital Conversion)

### What is ADC?

Analog-to-Digital Conversion is the process of converting a continuous analog signal (voltage) into discrete digital values that a microcontroller can process. This enables measurement of real-world analog sensors.

**Why ADC is Important:**
- Measures sensor outputs (temperature, light, pressure, etc.)
- Processes variable voltage signals
- Enables analog signal acquisition
- Foundation for data acquisition systems

### ADC Resolution

**Resolution** determines how finely the ADC can represent an analog value:

- **8-bit ADC:** 2^8 = 256 levels, 5V/256 ≈ 19.53 mV per step
- **10-bit ADC:** 2^10 = 1024 levels, 5V/1024 ≈ 4.88 mV per step
- **12-bit ADC:** 2^12 = 4096 levels, 5V/4096 ≈ 1.22 mV per step

**Higher resolution = more accurate measurements, but slower conversion**

### PIC18F4550 ADC Features

| Feature | Specification |
|---------|---|
| **Channels** | 13 multiplexed analog inputs (AN0-AN12) |
| **Resolution** | 10-bit (but we use upper 8 bits) |
| **Conversion Time** | ~12 TAD (Tab ADC clock cycles) |
| **Reference Options** | VSS/VDD or external Vref+/Vref- |
| **Clock Source** | Selectable dividers for TAD timing |

### ADC Conversion Steps

1. **Select Channel:** Configure ADCON0 to select AN0
2. **Set Reference:** Configure ADCON1 (we use VSS/VDD)
3. **Configure Timing:** Set ADCON2 for proper TAD and acquisition time
4. **Start Conversion:** Set GO bit in ADCON0
5. **Wait:** Monitor GO bit (clears when conversion completes)
6. **Read Result:** Read ADRESH (upper 8 bits) or ADRES (10 bits)

### Why Upper 8 Bits Only?

The PIC18F4550 ADC produces a 10-bit result (0-1023), but this project displays only 8 bits (0-255):
- **Simplifies display** (3 decimal digits instead of 4)
- **Sufficient resolution** for most applications
- **Faster display updates**
- **Upper 8 bits** capture the most significant information

---

## ▶️ Steps to Use

### 1. **Setup Development Environment**
   - Download and install MPLAB X IDE
   - Install XC8 Compiler
   - Ensure PIC18F4550 support is enabled

### 2. **Create Project**
   - Open MPLAB X IDE
   - Create a new MPLAB X project for PIC18F4550
   - Select XC8 as the compiler

### 3. **Add Source Code**
   - Copy the source code into the project
   - Ensure LCD and ADC initialization functions are included
   - Set oscillator frequency (typically 4 MHz or 20 MHz)

### 4. **Compile**
   - Build the project using XC8 compiler
   - Check for any errors or warnings
   - Generate the HEX file

### 5. **Program Microcontroller**
   - Connect PIC programmer (PICkit 3/4 or equivalent)
   - Load the generated HEX file
   - Program the PIC18F4550

### 6. **Assemble Circuit**
   - Build the circuit on breadboard or PCB
   - Double-check all LCD connections (RS, EN, D0-D7)
   - Verify potentiometer connections to RA0 and power rails
   - Ensure common ground between PIC and LCD

### 7. **Power and Test**
   - Apply 5V power supply
   - Adjust LCD contrast potentiometer for visibility
   - Rotate potentiometer connected to RA0
   - Observe ADC values change on LCD display (0-255)

---

## ✅ Verification & Troubleshooting

### Expected Behavior

- **At Power-Up:** LCD displays "ADC Value: 0" or similar
- **Rotating Pot:** LCD value changes from 0-255 smoothly
- **At 0V:** Display shows 0
- **At 2.5V:** Display shows approximately 127-128
- **At 5V:** Display shows 255
- **Update Rate:** LCD updates every 250ms (adjustable)

### Troubleshooting Guide

| Issue | Possible Cause | Solution |
|-------|-------|----------|
| **LCD shows garbage** | Contrast not adjusted; initialization failed | Adjust LCD contrast pot; verify initialization sequence |
| **LCD blank** | No power; wrong connections | Check VDD/VSS; verify RC0/RC1/RD0-D7 connections |
| **ADC stuck at 0** | AN0 not configured; input not connected | Verify ADCON1 = 0x0E; check potentiometer connections |
| **ADC stuck at 255** | Input rail floating; connection issue | Verify potentiometer wiper to RA0; check ground connections |
| **Values don't update** | ADC not running; GO bit stuck | Verify ADCON0 = 0x01; check ADC clock timing |
| **Erratic readings** | Noise on analog input; poor power supply | Add 0.1μF capacitor near RA0; use stable 5V supply |
| **Wrong voltage calibration** | Reference voltage not 5V; calculation error | Measure actual reference voltage; recalibrate formula |
| **LCD segments missing** | PORTD pin not connected; open circuit | Test continuity; rebuild connections |

### Advanced Troubleshooting

**1. Verify ADC Clock Timing**
   - TAD (ADC clock cycle) must be within 0.7-25 μs
   - For 20 MHz oscillator: ADCS = 0100 gives TAD ≈ 1 μs ✓

**2. Check Analog Input Filtering**
   - Add 0.1μF capacitor between RA0 and GND to reduce noise
   - Reduces high-frequency interference

**3. LCD Timing Issues**
   - Add delays between LCD commands: `__delay_us(100)`
   - Some LCD modules need longer enable pulse duration

**4. Power Supply Stability**
   - ADC accuracy depends on stable 5V reference
   - Use regulated power supply or 7805 regulator
   - Add 10μF bulk + 0.1μF ceramic capacitors near VDD

---

## 📸 Circuit Diagram

```
PIC18F4550 Pinout (40-pin DIP):
          ┌─────────────────┐
     MCLR │1            40  │ VSS
      RA0 │2            39  │ RD7 ──► LCD D7
      RA1 │3            38  │ RD6 ──► LCD D6
      RA2 │4            37  │ RD5 ──► LCD D5
      RA3 │5            36  │ RD4 ──► LCD D4
      RA4 │6            35  │ RD3 ──► LCD D3
      RA5 │7            34  │ RD2 ──► LCD D2
      VSS │8            33  │ RD1 ──► LCD D1
      VDD │9            32  │ RD0 ──► LCD D0
      RB0 │10           31  │ RC7
      RB1 │11           30  │ RC6
      RB2 │12           29  │ RC5
      RB3 │13           28  │ RC4
      RB4 │14           27  │ RC3
      RB5 │15           26  │ RC2
      RB6 │16           25  │ RC1 ──► LCD EN
      RB7 │17           24  │ RC0 ──► LCD RS
      VSS │18           23  │ RE2
      VDD │19           22  │ RE1
      OSC │20           21  │ RE0
          └─────────────────┘

Detailed Connection Diagram:
                    PIC18F4550
                    ┌─────────┐
         +5V ─────┤VDD (9,19)
         GND ─────┤VSS (8,18)
                  │
    Potentiometer │ RA0 (2) ─── AN0 (Analog Input)
    (Wiper) ─────┤
                  │ RC0 (24) ─── LCD RS
         LCD ─────┤ RC1 (25) ─── LCD EN
                  │ RD0-RD7 (32,1-7) ─── LCD D0-D7
                  └─────────────┘
```

---

## ⚠️ Notes

* **8-Bit vs 10-Bit:** This implementation uses only the upper 8 bits (ADRESH). For 10-bit accuracy, read both ADRESH and ADRESL.
* **ADC Accuracy:** Depends on stable 5V reference voltage and clean power supply.
* **Delay Adjustment:** Timing values depend on oscillator frequency (assumes 4 MHz or 20 MHz).
* **Acquisition Time:** Minimum 2 TAD must elapse before conversion starts for accurate sampling.
* **Noise Sensitivity:** Analog inputs are susceptible to electrical noise; use bypass capacitors and shielded wiring.
* **Channel Switching:** If switching between channels, allow adequate settling time (ACQ time).
* **LCD Display Speed:** Update frequency can be increased to 100ms for faster response (may exceed ADC conversion speed).

### Performance Considerations

- **ADC Conversion Time:** ~12 TAD ≈ 12 μs (at 20 MHz)
- **ADC Update Rate:** Maximum ~83 kHz (with no other processing)
- **LCD Transmission Time:** ~1 ms for 3-character data
- **Total Loop Time:** ~10-15 ms typical (allows for other tasks)

---

## 📚 Applications

* **ADC Learning Projects** – Foundation for analog signal processing
* **Sensor Monitoring Systems** – Temperature, pressure, light sensors
* **Voltage Measurement Applications** – Battery monitoring, power measurement
* **Data Acquisition Systems** – Multi-channel analog logging
* **Embedded Systems Education** – PIC18F ADC fundamentals
* **Environmental Monitoring** – Weather stations, environmental sensors
* **Instrumentation** – Analog-based measurement systems
* **Control Systems** – Feedback from analog sensors

---

## 🔗 Related Projects in This Series

- [INTERUPT - External Interrupt Control](https://github.com/srxcr/INTERUPT) – Master interrupt fundamentals
- [USART-LCD - Serial Communication & Display](https://github.com/srxcr/USART-LCD) – Advanced display control
- [Dual-Motor-Control - PWM & Motor Driving](https://github.com/srxcr/Dual-Motor-Control-with-LCD) – Advanced output control
- [Smart-EV-Charging-Station](https://github.com/srxcr/Smart-EV-Charging-Station) – Real-world embedded systems
- [Crane-Collision-Safety-System](https://github.com/srxcr/Crain_Collition_Saftey_Sys) – Safety-critical applications

---

## 🎯 Next Steps After This Project

1. **Extend to 10-bit resolution** – Read both ADRESH and ADRESL for full precision
2. **Multi-channel ADC** – Sample multiple analog inputs and display results
3. **Add filtering algorithms** – Implement moving average to reduce noise
4. **Serial output** – Send ADC values via USART to computer for logging
5. **Temperature sensor integration** – Use LM35 or similar analog temperature sensor
6. **Data logging** – Store ADC readings in EEPROM for later analysis
7. **Real-time graphing** – Send data to Processing or Python for visualization

---

## 📄 License

This project is intended for **educational and learning purposes**. Users are free to modify and enhance the project for academic and personal use. No commercial use without proper attribution.

---

## 🙏 Acknowledgements

Developed and tested using:

* **PIC18F4550 Microcontroller** – Microchip Technology
* **MPLAB X IDE** – Microchip Technology
* **XC8 Compiler** – Microchip Technology
* **LM016L 16×2 LCD Module** – Character display standard
* **Proteus Design Suite** – Labcenter Electronics (simulation)

---

## 📞 Support & Contribution

For questions, issues, or improvements:
- Review the troubleshooting section above
- Consult Microchip's PIC18F4550 datasheet (ADC module section)
- Check MPLAB X IDE documentation
- Refer to LCD manufacturer's datasheet for timing specifications

**Happy Learning! 🚀**
