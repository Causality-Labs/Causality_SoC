#define System_Tick_Frequency                   50000000
#define Timer_Prescaler                         16                          //Timer Prescaler, options are: 16, 1 
#define Timer_Load_Value_For_One_Sec            (System_Tick_Frequency/Timer_Prescaler)

#define DISPLAY_WIDTH 100
#define DISPLAY_HEIGHT 120

#define RED 0xE0
#define GREEN 0x1C
#define BLUE 0x03
#define WHITE 0xFF
#define BLACK 0x00


#define LEFT 0x61
#define RIGHT 0x64
#define DOWN 0x73
#define UP 0x77
#define RESET 0x72
#define QUIT 0x71
#define PAUSE 0x20

void SoC_init(void);
void clear_screen(void);
void delay(int value);