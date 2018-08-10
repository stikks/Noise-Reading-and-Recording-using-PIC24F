
// PIC24FJ64GB002 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary Oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Select bit (Use default SCL1/SDA1 pins for I2C1 )
#pragma config OSCIOFNC = ON            // OSCO Pin Configuration (OSCO pin functions as clock output (CLKO))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor (Sw Disabled, Mon Disabled)
#pragma config FNOSC = FRC              // Initial Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV))
//#pragma config PLLDIV = DIV2            // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 12 (48 MHz input))


// CONFIG1
#pragma config FWDTEN = OFF              // Watchdog Timer (Watchdog Timer is enabled)
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF              // JTAG Port Enable (JTAG port is enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


