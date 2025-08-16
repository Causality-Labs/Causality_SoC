# ARM Cortex-M0 System-on-Chip (SoC) on FPGA

A complete embedded system implementation featuring an **ARM Cortex-M0 DesignStart** processor running on FPGA hardware, designed for educational purposes and demonstrating real-world embedded systems design principles.

## Project Overview

This project implements a fully Micro Controller Unit on a **Basys 3 FPGA development board**. It demonstrates how modern embedded systems are built by integrating a professional ARM processor with custom peripherals through industry-standard bus protocols.

### What This System Includes

**ARM Cortex-M0 Processor Core**
- Official ARM DesignStart IP (free evaluation version)
- 32-bit RISC architecture with Thumb instruction set
- Industry-standard processor used in billions of IoT devices
- Complete NVIC (Nested Vectored Interrupt Controller)

**Professional Bus Architecture**
- AHB-Lite (AMBA 3.0) bus protocol
- Memory-mapped peripheral architecture
- Address decoder and bus multiplexer
- Industry-standard interconnect system

**Rich Peripheral Set**
- VGA Display Controller
- UART
- Timer
- GPIO
- 7-Segment Display
- Block RAM

**Complete Software Stack**
- ARM CMSIS (Cortex Microcontroller Software Interface Standard)
- Hardware Abstraction Layer (HAL) for easy programming
- Interrupt service routines and real-time programming

## Demonstration Software: Snake Game

The system includes a complete **Snake game** implementation that showcases all system capabilities:

### Game Features
- **Real-time graphics** rendering on VGA display
- **Keyboard input** via UART serial terminal
- **Collision detection** (walls and self-collision)
- **Interrupt-driven** responsive controls
- **Game state management** (play, pause, game over)

## Target Hardware: Basys 3 FPGA Board

### Why Basys 3?
- **Educational focus** - Designed for learning digital design
- **Affordable** - Under $150, accessible to students
- **Rich I/O** - VGA, USB, LEDs, switches, 7-segment displays
- **Adequate resources** - Artix-7 FPGA with plenty of logic and memory
- **Tool support** - Works with free Vivado WebPACK

## Development Environment

### Hardware Design (HDL)
- **Vivado Design Suite** (free WebPACK edition)
- **Verilog HDL** for all peripheral implementations
- **ARM Cortex-M0 DesignStart** IP (free from ARM)
- **Constraint files** for Basys 3 pin mapping

### Software Development
- **Keil uVision IDE** for embedded development
- **C and Assembly** programing the SoC
- **ARM CMSIS** standard for consistent APIs
- **Custom Hardware Abstraction Layer** for portable code

## Project Structure

```
Intro_SoC/
├── HDL/                           # Hardware Description Language
│   ├── AHBLITE_SYS.v             # Top-level SoC integration
│   ├── CortexM0-DS/               # ARM Cortex-M0 processor IP
│   ├── AHB_BRAM/                  # Memory controller
│   ├── AHB_VGA/                   # VGA display system
│   ├── AHB_UART/                  # Serial communication
│   ├── AHB_TIMER/                 # Timer/counter peripheral
│   ├── AHB_GPIO/                  # General purpose I/O
│   ├── AHB_7SEG/                  # 7-segment display
│   └── AHB_BUS/                   # Bus interconnect
├── Software/                      # Embedded software
│   ├── app/                       # Snake game application
│   ├── SoC_hal/                   # Hardware abstraction layer
│   ├── bsp/                       # Board support package
│   ├── core/                      # ARM CMSIS headers
```

## Educational Value

### Hardware Concepts
- **FPGA design methodology** and synthesis
- **Bus protocols** and system integration
- **Memory-mapped I/O** and address decoding
- **Clocking and reset** design
- **Timing constraints** and closure

### Software Concepts
- **Embedded C programming** on real hardware
- **Interrupt programming** and real-time constraints
- **Hardware abstraction** and driver development
- **ARM Cortex-M architecture** and instruction set
- **Memory management** and linker scripts

## Quick Start Guide

### Prerequisites
- Basys 3 FPGA development board
- Vivado Design Suite (free WebPACK)
- VGA monitor and USB cable
- Keil uVision

### Running the Demo
1. **Program FPGA**: Load the provided bitstream using Vivado
2. **Connect hardware**: VGA monitor and USB cable to PC
3. **Open terminal**: 9600 baud, 8N1 settings
4. **Play Snake**: Use W/A/S/D keys to control the snake
5. **Watch display**: Game graphics appear on VGA monitor

### Building from Source
1. **Open Vivado project**: `Cortex-M0_SoC.xpr`
2. **Generate bitstream**: Synthesis and implementation
3. **Program FPGA**: Download bitstream to board
4. **Optional**: Modify software and regenerate `code.hex`

## Applications and Extensions

### Educational Use
- **Digital systems courses** - Complete SoC design example
- **Embedded programming courses** - Real hardware platform
- **Computer architecture courses** - ARM processor study
- **FPGA design courses** - Practical implementation project

### Research and Development
- **Custom peripheral development** - Add SPI, I2C, PWM, etc.
- **Performance studies** - Analyze bus utilization and timing
- **Security research** - Implement hardware security features
- **AI/ML acceleration** - Add custom compute units

### Project Extensions
- **Custom peripheral development** - Add AHB compataible SPI and I2C peripheral
- **Sensors**: Interface with temperature, motion, or other sensors

## Professional Relevance

This project demonstrates **industry-standard practices**:

- **ARM processor architecture** - Used in billions of devices
- **AMBA bus protocols** - Standard in professional SoCs
- **Hardware abstraction** - Essential for portable embedded software
- **Interrupt-driven programming** - Required for real-time systems
- **Memory-mapped I/O** - Universal in embedded systems
- **FPGA prototyping** - Common in semiconductor development

## Conclusion

This ARM Cortex-M0 SoC project provides a complete, hands-on introduction to embedded systems design. It bridges the gap between theoretical knowledge and practical implementation, using professional tools and industry-standard architectures.

Whether you're a student learning embedded systems, an educator teaching digital design, or a professional exploring FPGA-based prototyping, this project offers a comprehensive platform for understanding how modern embedded systems really work.

