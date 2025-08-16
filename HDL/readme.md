# HDL (Hardware Description Language) Implementation

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

## Core Components

### 1. Top-Level Integration (`AHBLITE_SYS.v`)
- **System orchestration**: Integrates all components into a complete SoC
- **Clock management**: 50 MHz system clock derived from 100 MHz board clock
- **Reset generation**: Synchronized reset distribution to all modules
- **I/O mapping**: Connects internal signals to FPGA pins via constraints
- **Interrupt routing**: Manages IRQ signals from peripherals to processor

**Key Features:**
- ARM Cortex-M0 DesignStart processor instantiation
- AHB-Lite bus infrastructure setup
- All peripheral instantiations and connections
- External I/O pin assignments for Basys 3 board

### 2. ARM Processor Core (`CortexM0-DS/`)
- **Official ARM IP**: ARM Cortex-M0 DesignStart processor
- **Industry-standard**: Same core used in commercial microcontrollers
- **Complete implementation**: Including NVIC, debug interface, and power management
- **Processor visibility**: Internal register monitoring for debugging

**Architecture:**
- 32-bit RISC processor with Thumb instruction set
- Harvard architecture (separate instruction/data buses)
- 2-level interrupt priority with NVIC
- Debug access port (DAP) for JTAG/SWD debugging

### 3. AHB-Lite Bus System (`AHB_BUS/`)

#### Address Decoder (`AHBDCD.v`)
- **Function**: Routes processor transactions to appropriate peripherals
- **Memory map implementation**: Decodes 32-bit addresses to peripheral select signals
- **Scalable design**: Supports up to 10 slave devices plus unmapped region

**Memory Map Implementation:**
```verilog
// Address decode ranges (from AHBDCD.v)
assign HSEL_S0 = (HADDR[31:24] == 8'h00);  // 0x00000000 - BRAM Memory
assign HSEL_S1 = (HADDR[31:24] == 8'h50);  // 0x50000000 - VGA Display  
assign HSEL_S2 = (HADDR[31:24] == 8'h51);  // 0x51000000 - UART Serial
assign HSEL_S3 = (HADDR[31:24] == 8'h52);  // 0x52000000 - Timer/Counter
assign HSEL_S4 = (HADDR[31:24] == 8'h53);  // 0x53000000 - GPIO I/O
assign HSEL_S5 = (HADDR[31:24] == 8'h54);  // 0x54000000 - 7-Segment
```

#### Bus Multiplexer (`AHBMUX.v`)
- **Function**: Multiplexes read data and ready signals from peripherals back to processor
- **Pipelined operation**: Handles AHB-Lite address/data phase separation
- **Error handling**: Provides DEADBEEF pattern for unmapped addresses

## Peripheral Controllers

### 4. Memory Controller (`AHB_BRAM/AHB2BRAM.v`)
- **Configurable size**: Default 16KB, expandable to 1GB via parameter
- **Program loading**: Supports `code.hex` file initialization
- **Full AHB compliance**: Proper address/data phase handling
- **Byte-level access**: Supports 8-bit, 16-bit, and 32-bit transactions

**Key Features:**
- Dual-port block RAM implementation
- Memory initialization from hex files
- Byte lane decoding for partial word writes
- Zero wait-state operation

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
- Configurable resolution register at offset `0x3BF24`

### 6. UART Serial Controller (`AHB_UART/AHBUART.v`)
- **Full-duplex communication**: Independent TX/RX with FIFO buffers
- **Configurable baud rates**: 9600 to 115200 bps
- **Interrupt support**: TX complete and RX ready interrupts
- **Hardware components**:
  - **Baud rate generator** (`baudgen.v`)
  - **FIFO buffers** (`fifo.v`)
  - **TX engine** (`uart_tx.v`)
  - **RX engine** (`uart_rx.v`)

**Register Map:**
- `+0x00`: Data Register (TX/RX)
- `+0x04`: Status Register (flags)
- `+0x08`: Baud Rate Register

### 7. Timer/Counter Controller (`AHB_TIMER/AHBTIMER.v`)
- **Multiple operating modes**: Free-run, periodic, compare, PWM, capture
- **Flexible clocking**: Prescaler support (1, 16, 256 division)
- **Interrupt generation**: Compare match and overflow events
- **PWM output**: Configurable duty cycle for motor control

**Register Set:**
- Load Value, Current Value, Control, Clear
- Compare Register for match detection
- Capture Register for input timing

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

**Features:**
- Individual digit registers (DIGIT1-DIGIT4)
- Display enable/disable control
- Automatic refresh timing
- Decimal point control

## Design Methodology

### Industry-Standard Practices
1. **Hierarchical design**: Clean module boundaries and interfaces
2. **Parameterized modules**: Configurable for different applications
3. **Proper reset handling**: Synchronous reset release
4. **Clock domain management**: Single-clock design with proper timing
5. **Lint-clean code**: Following Verilog best practices

### AHB-Lite Compliance
- **Address/Data phases**: Proper pipelined operation
- **Wait states**: HREADY signal handling
- **Error responses**: HRESP signal implementation
- **Burst support**: INCR4, INCR8, INCR16 transactions
- **Protection attributes**: HPROT signal handling

### FPGA Optimization
- **Resource utilization**: Efficient use of LUTs, FFs, and BRAM
- **Timing closure**: Meeting setup/hold requirements
- **Power optimization**: Clock gating where appropriate
- **Vendor primitives**: Using Xilinx-optimized components

## Development Tools and Flow

### Design Tools
- **Vivado Design Suite**: Complete FPGA development environment
- **Synthesis**: RTL to netlist conversion with optimization
- **Implementation**: Place and route for Artix-7 FPGA
- **Timing analysis**: Static timing analysis for performance verification

### Constraints and I/O
- **Pin assignments**: `basys_3_constraints.xdc` maps signals to FPGA pins
- **Timing constraints**: Clock definitions and I/O timing
- **I/O standards**: LVCMOS33 for board compatibility

## Memory Map Summary

| Peripheral | Base Address | Size | Description |
|------------|-------------|------|-------------|
| **BRAM Memory** | `0x00000000` | 16KB (default) | Program and data storage |
| **VGA Controller** | `0x50000000` | 16MB | Graphics and text display |
| **UART Serial** | `0x51000000` | 16MB | Serial communication |
| **Timer/Counter** | `0x52000000` | 16MB | Timing and PWM generation |
| **GPIO I/O** | `0x53000000` | 16MB | LED and switch control |
| **7-Segment** | `0x54000000` | 16MB | Numerical display |

## Educational Value

This HDL implementation provides hands-on experience with:

### Digital Design Concepts
- **Bus protocols**: AMBA AHB-Lite standard implementation
- **Memory systems**: Address decoding and data routing
- **I/O interfacing**: Real-world peripheral design
- **System integration**: Connecting multiple complex components

### FPGA Development
- **Synthesis optimization**: Resource utilization and timing
- **Constraint management**: I/O and timing specifications
- **Debug techniques**: Hardware debugging with logic analyzers
- **Verification methods**: Simulation and hardware validation

### Professional Skills
- **Industry standards**: ARM processor integration
- **Modular design**: Reusable and maintainable code
- **Documentation**: Proper commenting and specifications
- **Version control**: Project management practices

## Extension Opportunities

The modular design makes it easy to:

1. **Add new peripherals**: SPI, I2C, PWM, ADC controllers
2. **Expand memory**: External SDRAM or DDR interfaces
3. **Enhance graphics**: Higher resolution or color depth
4. **Implement networking**: Ethernet MAC or wireless interfaces
5. **Add security**: Cryptographic accelerators or secure boot

## Getting Started

1. **Open Vivado project**: Load `Cortex-M0_SoC.xpr`
2. **Examine hierarchy**: Start with `AHBLITE_SYS.v` top-level
3. **Understand peripherals**: Study individual AHB_* modules
4. **Run synthesis**: Generate and analyze timing reports
5. **Program FPGA**: Download bitstream to Basys 3 board

This HDL implementation represents a complete, professional-quality embedded system that bridges academic learning with industry practice, providing an excellent foundation for understanding how real embedded systems are designed and implemented.
