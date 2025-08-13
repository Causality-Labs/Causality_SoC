# ARM Cortex-M0 DesignStart SoC on FPGA

This repository contains the HDL (Hardware Description Language) and software source code for a complete System-on-Chip (SoC) implementation featuring an ARM Cortex-M0 DesignStart processor core on an FPGA platform.

## Project Overview

This SoC implementation demonstrates a fully functional embedded system with the ARM Cortex-M0 processor interfaced to various peripherals through the AMBA AHB-Lite bus. The system is designed to run on a **Basys 3 FPGA development board** (Xilinx Artix-7) and provides a comprehensive platform for embedded systems development and education.

### Key Features

- **ARM Cortex-M0 DesignStart** processor core
- **AHB-Lite bus** interconnect system
- **Multiple peripherals** with interrupt support
- **Comprehensive software examples** in C and Assembly
- **FPGA-optimized** design for Basys 3 board
- **Educational platform** for learning embedded systems

## Hardware Architecture

### Processor Core
- **ARM Cortex-M0 DesignStart**: 32-bit RISC processor
- **50 MHz** system clock (from 100 MHz board clock with 2x divider)
- **Harvard architecture** with separate instruction and data buses
- **Thumb instruction set** only
- **2 interrupt priority levels**

### Memory System
- **On-chip BRAM**: Configurable size (default 16KB, expandable to 1GB)
- **Memory-mapped peripherals**: All peripherals accessible via memory addresses
- **Memory initialization**: Supports loading programs from `code.hex` file

### Bus Architecture
- **AHB-Lite protocol**: AMBA 3 compliant
- **Address decoder**: Routes transactions to appropriate slaves
- **Bus multiplexer**: Handles multiple slave responses
- **Pipelined operation**: Address and data phases separated

## Peripheral Set

### Memory Map

| Peripheral | Base Address | Address Range | Description |
|------------|-------------|---------------|-------------|
| **BRAM** | `0x00000000` | `0x00000000 - 0x00FFFFFF` | On-chip Block RAM |
| **VGA** | `0x50000000` | `0x50000000 - 0x503FFFFF` | VGA display controller |
| **UART** | `0x51000000` | `0x51000000 - 0x51FFFFFF` | Serial communication |
| **Timer** | `0x52000000` | `0x52000000 - 0x52FFFFFF` | Programmable timer |
| **GPIO** | `0x53000000` | `0x53000000 - 0x53FFFFFF` | General Purpose I/O |
| **7-Segment** | `0x54000000` | `0x54000000 - 0x54FFFFFF` | 7-segment display |

### Detailed Peripheral Descriptions

#### 1. VGA Display Controller (`AHBVGASYS`)
- **Text console**: 80×30 character display
- **Graphics buffer**: Programmable resolution (2×2, 4×4, 8×8 pixel scaling)
- **Font ROM**: Built-in character generator
- **Color support**: 8-bit RGB (3-3-2 format)
- **Dual-port memory**: Separate console and image regions
- **Resolution register**: `0x5003BF24`

**Key Features:**
- Automatic scrolling text console
- Pixel-level graphics control
- Multiple resolution modes
- Hardware cursor management

#### 2. UART Controller (`AHBUART`)
- **Configurable baud rates**: 9600 to 115200 bps
- **8-bit data**: No parity, 1 stop bit
- **FIFO buffers**: Both transmit and receive
- **Interrupt support**: TX/RX complete interrupts
- **Status flags**: TX ready, RX ready, parity error

**Register Map:**
- `+0x00`: Data Register
- `+0x04`: Status Register  
- `+0x08`: Baud Rate Register

#### 3. Timer/Counter (`AHBTIMER`)
- **32-bit counter**: Up/down counting
- **Multiple modes**: Free-run, periodic, compare, PWM, capture
- **Prescaler support**: 1, 16, 256 division ratios
- **Interrupt generation**: Compare match, overflow
- **PWM output**: Configurable duty cycle

**Register Map:**
- `+0x00`: Load Value Register
- `+0x04`: Current Value Register
- `+0x08`: Control Register
- `+0x0C`: Clear Register
- `+0x10`: Compare Register
- `+0x14`: Capture Register

#### 4. GPIO Controller (`AHBGPIO`)
- **16-bit I/O**: Bidirectional pins
- **Direction control**: Per-pin input/output configuration
- **Mask register**: Selective pin updates
- **Interrupt support**: Level/edge triggered
- **Switch/LED interface**: Connected to board I/O

**Register Map:**
- `+0x00`: Data Register
- `+0x04`: Direction Register
- `+0x08`: Mask Register

#### 5. 7-Segment Display (`AHB7SEGDEC`)
- **4 digits**: Independent control
- **BCD decoder**: Automatic decimal display
- **Hex mode**: Raw segment control
- **Display enable**: Individual digit control
- **Multiplexed output**: Time-division display

**Register Map:**
- `+0x00`: Digit 1 Register
- `+0x04`: Digit 2 Register
- `+0x08`: Digit 3 Register
- `+0x0C`: Digit 4 Register
- `+0x10`: Display Mode Register

## Interrupt System

### Interrupt Vector Table
| IRQ # | Source | Priority | Handler |
|-------|--------|----------|---------|
| 0 | Timer | Configurable | `Timer_Handler` |
| 1 | UART | Configurable | `UART_Handler` |
| 2 | GPIO | Configurable | `GPIO_Handler` |

### NVIC Configuration
- **2-bit priority**: 4 priority levels
- **Interrupt enable**: Per-IRQ enable/disable
- **Pending flags**: Status and clear functions
- **Priority grouping**: Configurable preemption

## SoC Integration

### Top-Level Module (`AHBLITE_SYS`)
The main SoC integration module that connects all components:

- **Cortex-M0 Integration**: Instantiates the ARM processor core
- **Clock Management**: Generates 50 MHz system clock from 100 MHz input
- **Reset Logic**: Synchronized reset distribution
- **Bus Infrastructure**: Address decoder and multiplexer
- **Peripheral Connections**: All AHB-Lite slaves connected to the bus

## Hardware Design Files

### HDL Structure
```
HDL/
├── AHBLITE_SYS.v                 # Top-level SoC integration
├── basys_3_constraints.xdc       # FPGA pin assignments and timing
├── testbench.v                   # System verification testbench
├── update_bitstream.tcl          # Software update scripts
├── update_bitstream_header.tcl   # Memory initialization update
├── AHB_BRAM/
│   └── AHB2BRAM.v               # Block RAM AHB-Lite interface
├── AHB_BUS/
│   ├── AHBDCD.v                 # Address decoder
│   └── AHBMUX.v                 # Bus multiplexer
├── AHB_VGA/
│   ├── AHBVGASYS.v              # VGA system controller
│   ├── vga_console.v            # Text console engine
│   ├── vga_image.v              # Graphics buffer controller
│   ├── vga_sync.v               # VGA timing generator
│   ├── counter.v                # Timing counters
│   ├── dual_port_ram_sync.v     # Dual-port memory
│   └── font_rom.v               # Character font ROM
├── AHB_UART/
│   ├── AHBUART.v                # UART AHB-Lite interface
│   ├── uart_rx.v                # UART receiver
│   ├── uart_tx.v                # UART transmitter
│   ├── baudgen.v                # Baud rate generator
│   └── fifo.v                   # FIFO buffers
├── AHB_TIMER/
│   ├── AHBTIMER.v               # Timer AHB-Lite interface
│   └── prescaler.v              # Clock prescaler
├── AHB_GPIO/
│   └── AHBGPIO.v                # GPIO AHB-Lite interface
├── AHB_7SEG/
│   └── AHB7SEGDEC.v             # 7-segment display controller
└── CortexM0-DS/
    ├── CORTEXM0INTEGRATION.v     # Cortex-M0 wrapper
    ├── cortexm0ds_logic.v        # Core processor logic
    └── ReadMe.txt                # ARM core documentation
```

### Design Methodology
- **Modular Architecture**: Each peripheral is a self-contained AHB-Lite slave
- **Parameterizable Design**: Configurable memory sizes and peripheral features
- **FPGA Optimization**: Block RAM usage and clock domain management
- **Verification**: Comprehensive testbench for system validation

## Target Platform

### Basys 3 FPGA Board
- **FPGA**: Xilinx Artix-7 XC7A35T-1CPG236C
- **Logic Resources**: 33,280 logic cells, 1,800 Kbits Block RAM
- **Clock**: 100 MHz input oscillator
- **I/O Interfaces**:
  - 16 LEDs (for GPIO output)
  - 16 slide switches (for GPIO input)  
  - 4×7-segment display (multiplexed)
  - VGA connector (RGB 3-3-2 format)
  - USB-UART bridge (for serial communication)
  - 5 push buttons (including reset)

### Pin Assignments (`basys_3_constraints.xdc`)
- **System Clock**: W5 (100 MHz, LVCMOS33)
- **Reset Button**: T17 (BTNR, active high)
- **LEDs**: U16, E19, U19, V19, W18, U15, U14, V14, V13
- **VGA RGB**: G19-J19 (Red), N18-L18 (Blue), J17-G17 (Green)
- **VGA Sync**: P19 (Hsync), R19 (Vsync)
- **7-Segment**: Multiplexed 4-digit display with common anode
- **UART**: Connected via USB bridge (auto-enumerated COM port)

### Resource Utilization
The SoC implementation uses approximately:
- **Logic Utilization**: ~15% of available LUTs and FFs
- **Block RAM**: 16KB default (expandable based on MEMWIDTH parameter)
- **I/O Pins**: 35+ pins for peripherals and debug interfaces
- **Clock Domains**: Single 50 MHz domain (divided from 100 MHz input)

## Architecture Applications

This SoC architecture serves as an excellent reference design for:

### System-on-Chip Design
- **Bus Protocol Implementation**: AHB-Lite interconnect design
- **Memory Subsystem**: On-chip BRAM with configurable addressing
- **Peripheral Integration**: Multiple slave devices on shared bus
- **Clock and Reset Management**: Synchronized system control

### FPGA Design Techniques
- **Resource Optimization**: Efficient use of Block RAM and logic
- **Timing Closure**: 50 MHz operation with proper constraints
- **Pin Planning**: Complete I/O interface management
- **Design Verification**: Testbench and simulation methodology

### Educational Platform
- **Digital System Design**: Understanding of processor-peripheral communication
- **Bus Protocols**: Hands-on experience with AMBA standards
- **Embedded Systems**: Complete hardware/software co-design platform
- **FPGA Development**: Industry-standard tools and methodologies

## Hardware Extensions

Potential architectural enhancements for this SoC platform:

### Additional Peripherals
- **SPI Controller**: High-speed serial interface for external devices
- **I2C Master**: Multi-device communication bus
- **PWM Controllers**: Multiple-channel pulse width modulation
- **ADC Interface**: Analog-to-digital conversion capability

### Memory Expansion
- **External SDRAM**: Off-chip memory interface controller
- **Cache System**: Instruction and data cache implementations
- **DMA Controller**: Direct memory access for high-throughput transfers

### Advanced Features  
- **Networking**: Ethernet MAC controller
- **Audio Processing**: I2S interface and audio codec support
- **Security**: Cryptographic accelerator modules
- **Power Management**: Clock gating and dynamic voltage scaling

### System Enhancements
- **Multi-core Support**: Additional processor cores
- **Debug Interface**: JTAG/SWD debug access port
- **Performance Monitoring**: Hardware performance counters
- **Error Detection**: ECC memory and fault injection

## Quick Start

### Hardware Setup
1. **Load FPGA**: Program Basys 3 board with provided bitstream
2. **Connect Peripherals**: VGA monitor, USB cable for UART
3. **Power On**: Board auto-configures with default software

### Basic Operation
- **LEDs**: Show GPIO output (connected to switches)
- **7-Segment**: Displays timer/counter values
- **VGA**: Text console and graphics output
- **UART**: Serial communication at 115200 baud
- **Reset**: Use BTNR to restart system

For software development and detailed build instructions, refer to the Software directory documentation.
