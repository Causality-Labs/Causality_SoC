# HDL Implementation

This directory contains the complete Verilog HDL implementation of the ARM Cortex-M0 System-on-Chip (SoC), targeting the Basys 3 FPGA development board. The design follows industry-standard practices using the AMBA AHB-Lite bus protocol to create a professional embedded system.

## System Architecture Overview

The SoC is built around a **hierarchical, modular design** that demonstrates real-world embedded system architecture:

```
┌─────────────────────────────────────────────────────────────────┐
│                     AHBLITE_SYS.v (Top Level)                  │
│  ┌─────────────────┐    ┌─────────────────────────────────────┐ │
│  │   ARM Cortex-M0 │    │          AHB-Lite Bus               │ │
│  │   DesignStart   │◄──►│     (AHBDCD + AHBMUX)              │ │
│  │   INTEGRATION   │    │                                     │ │
│  └─────────────────┘    └─────────────┬───────────────────────┘ │
│                                       │                         │
│                     ┌─────────────────┼─────────────────────────┤
│                     │                 │                         │
│  ┌──────────────────▼──┐  ┌───────────▼──┐  ┌─────────────────▼─┐ │
│  │    AHB2BRAM         │  │   AHBVGA     │  │    AHBUART        │ │
│  │  (Memory Controller)│  │ (VGA Display)│  │ (Serial Comm)     │ │
│  └─────────────────────┘  └──────────────┘  └───────────────────┘ │
│                                                                  │
│  ┌─────────────────────┐  ┌──────────────┐  ┌───────────────────┐ │
│  │    AHBTIMER         │  │   AHBGPIO    │  │   AHB7SEGDEC      │ │
│  │ (Timer/Counter/PWM) │  │ (GPIO I/O)   │  │ (7-Segment Disp)  │ │
│  └─────────────────────┘  └──────────────┘  └───────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```
## Memory Map Summary

| Peripheral | Base Address| Description |
|------------|-------------|-------------|
| **BRAM Memory** | `0x00000000` | Program and data storage |
| **VGA Controller** | `0x50000000` | Graphics and text display |
| **UART** | `0x51000000` | Serial communication |
| **Time** | `0x52000000` | Timing and PWM generation |
| **GPIO** | `0x53000000` | General Purpose Input Output|
| **7-Segment** | `0x54000000` | Numerical display |


## Core Components

### 1. Top-Level Integration (`AHBLITE_SYS.v`)
- **System orchestration**: Integrates all components into a complete SoC
- **Clock management**: 50 MHz system clock derived from 100 MHz board clock
- **Reset generation**: Synchronized reset distribution to all modules
- **I/O mapping**: Connects internal signals to FPGA pins via constraints
- **Interrupt routing**: Manages IRQ signals from peripherals to processor

### 2. ARM Processor Core (`CortexM0-DS/`)
- **Official ARM IP**: ARM Cortex-M0 DesignStart processor
- **Industry-standard**: Same core used in commercial microcontrollers
- **Complete implementation**: Including NVIC, debug interface, and power management
- **Processor visibility**: Internal register monitoring for debugging

### 3. AHB-Lite Bus System (`AHB_BUS/`)

#### Address Decoder (`AHBDCD.v`)
- **Function**: Routes processor transactions to appropriate peripherals
- **Memory map implementation**: Decodes 32-bit addresses to peripheral select signals
- **Scalable design**: Supports up to 10 slave devices plus unmapped region

#### Bus Multiplexer (`AHBMUX.v`)
- **Function**: Multiplexes read data and ready signals from peripherals back to processor
- **Pipelined operation**: Handles AHB-Lite address/data phase separation
- **Error handling**: Provides DEADBEEF pattern for unmapped addresses

## Peripherals

### 4. Memory Controller (`AHB_BRAM/AHB2BRAM.v`)
- **Configurable size**: Default 16KB, expandable to 1GB via parameter
- **Program loading**: Supports `code.hex` file initialization
- **Full AHB compliance**: Proper address/data phase handling
- **Byte-level access**: Supports 8-bit, 16-bit, and 32-bit transactions

### 5. VGA Display Controller (`AHB_VGA/AHBVGASYS.v`)
- **Dual-mode operation**: Text console + graphics framebuffer
- **Multiple resolutions**: 2×2, 4×4, and 8×8 pixel scaling modes
- **Hardware components**:
  - **VGA timing generator** (`vga_sync.v`)
  - **Text console** (`vga_console.v`) - 80×30 character display
  - **Graphics framebuffer** (`vga_image.v`) - pixel-level control
  - **Font ROM** (`font_rom.v`) - character generator
  - **Dual-port memory** (`dual_port_ram_sync.v`)

**Graphics Capabilities:**
- 640×480 VGA output @ 60 Hz
- 8-bit color (3-3-2 RGB format)
- Separate text and graphics regions
- Hardware scrolling for text console

### 6. UART Serial Controller (`AHB_UART/AHBUART.v`)
- **Full-duplex communication**: Independent TX/RX with FIFO buffers
- **Configurable baud rates**: 9600 to 115200 bps
- **Interrupt support**: TX complete and RX ready interrupts
- **Hardware components**:
  - **Baud rate generator** (`baudgen.v`)
  - **FIFO buffers** (`fifo.v`)
  - **TX engine** (`uart_tx.v`)
  - **RX engine** (`uart_rx.v`)

### 7. Timer/Counter Controller (`AHB_TIMER/AHBTIMER.v`)
- **Multiple operating modes**: Free-run, periodic, compare, PWM, capture
- **Flexible clocking**: Prescaler support (1, 16, 256 division)
- **Interrupt generation**: Compare match and overflow events
- **PWM output**: Configurable duty cycle for motor control

### 8. GPIO Controller (`AHB_GPIO/AHBGPIO.v`)
- **16-bit bidirectional I/O**: Individual pin direction control
- **Mask register**: Selective pin updates without read-modify-write
- **Interrupt capability**: Level and edge-triggered interrupts
- **Direct board connection**: LEDs, switches, and expansion I/O

### 9. 7-Segment Display Controller (`AHB_7SEG/AHB7SEGDEC.v`)
- **4-digit display**: Independent control of each digit
- **Encoding support**: Both hexadecimal (0-F) and decimal (0-9) modes
- **Multiplexed operation**: Time-division multiplexing for 4 digits
- **Hardware decoder**: Automatic 7-segment pattern generation

### Constraints and I/O
- **Pin assignments**: `basys_3_constraints.xdc` maps signals to FPGA pins
- **Timing constraints**: Clock definitions and I/O timing
- **I/O standards**: LVCMOS33 for board compatibility

