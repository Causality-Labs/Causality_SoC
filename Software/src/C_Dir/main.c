//------------------------------------------------------------------------------------------------------
// Program SoC using C
// 1)Input from 8-bit switch and output to LEDs
// 2)Input characters from keyboard (UART) and output to the terminal
// 3)A counter is incremented from 1 to 10, and displayed on the VGA monitor
//------------------------------------------------------------------------------------------------------

#define AHB_VGA_BASE                0x50000000

#define AHB_UART_BASE               0x51000000
#define AHB_UART_BAUD               (AHB_UART_BASE + 0x08)
#define AHB_UART_PARITY             (AHB_UART_BASE + 0x0C)

#define AHB_TIMER_BASE          0x52000000
#define AHB_TIMER_CTL          (AHB_TIMER_BASE + 0x08)
#define AHB_TIMER_CLR          (AHB_TIMER_BASE + 0x0C)

#define AHB_GPIO_BASE               0x53000000

#define AHB_7SEG_BASE               0x54000000

#define NVIC_INT_ENABLE         0xE000E100
#define NVIC_INT_PRIORITY0  0xE000E400

// UART Baudrate configurations
#define B9600    326
#define B19200   162
#define B38400   81
#define B57600   54
#define B115200  27

volatile unsigned int counter = 0x31;


void UART_ISR()
{
    unsigned int c;

    c = *(volatile unsigned int *)AHB_UART_BASE;
    *(volatile unsigned int *)AHB_UART_BASE = c;

}


void Timer_ISR()
{

    if (counter > 0x3A) {
        counter = 0x31;  // Reset if corrupted
        // Write error pattern to show corruption occurred
        *(volatile unsigned int*) (AHB_7SEG_BASE + 0x08) = 0x0E;  // 'E' for Error
        *(volatile unsigned int*) (AHB_TIMER_BASE+0x0C) = 1;	//Clear timer interrupt request

        return;
    }
    else {
        *(volatile unsigned int*) (AHB_7SEG_BASE + 0x08) = 0x0A;
    }

	*(volatile unsigned int*) AHB_VGA_BASE = counter;		//print the counter value to VGA
	*(volatile unsigned int*) AHB_7SEG_BASE = counter-0x30;		//print the 7-segment display
	*(volatile unsigned int*) AHB_VGA_BASE = ' ';				//print space

	counter++;
	if (counter >= 0x3A) {
        *(volatile unsigned int*) (AHB_TIMER_BASE+8) = 0;	//Stop timer if counter reaches 9
    }

    *(volatile unsigned int*) (AHB_TIMER_BASE+0x0C) = 1;	//Clear timer interrupt request

    return;
}

//////////////////////////////////////////////////////////////////
// Main Function
//////////////////////////////////////////////////////////////////

int main(void) {
    
    // ALL VARIABLE DECLARATIONS MUST BE AT THE TOP IN C89/C90
    unsigned int simple_counter = 0;
    unsigned int digit = 0;
    
    // Initialize global variable since we bypassed C runtime
    //counter = 0x31;  // Make sure counter is initialized to '1'
    
    // IMMEDIATELY write distinctive pattern to prove main() was called
    //counter = 0x31; 

    *(volatile unsigned int*) AHB_7SEG_BASE = 0x01;                            // DIGIT1 = C
    *(volatile unsigned int*) (AHB_7SEG_BASE +0x04) = 0x02;                    // DIGIT2 = D  
    *(volatile unsigned int*) (AHB_7SEG_BASE +0x08) = 0x03;                    // DIGIT3 = E
    *(volatile unsigned int*) (AHB_7SEG_BASE +0x0C) = 0x00;                    // DIGIT4 = F

    *(volatile unsigned int*) AHB_TIMER_BASE = 50000000;           // Timer load register: 1 second at 50MHz
    *(volatile unsigned int*) AHB_TIMER_CTL = 0x03;                // PERIODIC mode + prescaler + enable
    
    // UART setup
    *(volatile unsigned int*) AHB_UART_BAUD = B115200;
    *(volatile unsigned int*) AHB_UART_PARITY = 0;

    // Interrupt setup
    *(volatile unsigned int*) NVIC_INT_PRIORITY0 = 0x00004000;     // Priority: IRQ0(Timer): 0x00, IRQ1(UART): 0x40
    *(volatile unsigned int*) NVIC_INT_ENABLE = 0x00000003;        // Enable interrupts for UART and timer
    
    // Enable global interrupts (critical!)
    //__asm("CPSIE i");
    
    // Simple infinite loop with counting - should now work with timer interrupts too
    while(1) {
        //Count faster to see if main loop is running
        simple_counter++;
        if (simple_counter > 5000000) {  // Much faster count to see if main is running
            // Cycle through digits 0-9 on the rightmost display to show main is running
            *(volatile unsigned int*) (AHB_7SEG_BASE +0x0C) = digit;
            digit++;
            if (digit >= 10)
            {
                digit = 0;
            }
            simple_counter = 0;
        }
    }
}


