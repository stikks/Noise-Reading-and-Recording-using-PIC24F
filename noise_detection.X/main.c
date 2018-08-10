/*
 * File:   main.c
 * Author: stikks
 *
 * Detects external sound
 * records sound on SD Card
 * Created on August 6, 2018, 2:54 PM
 */


#include "configbits.h"
#define PLAYBACK_LED LATBbits.LATB5
#define RECORD_LED LATBbits.LATB13
#define SYSCLK (8000000)
/*
 * Fcy = Fosc/2 = 4MHz
 * Tosc = 1/Fosc = 125ns
 * Tcy = 2 * Tosc = 250ns
 * ADCS = 15
 * TAD = TCY * (ADCS + 1)
 * TAD = 250 * (15+1) = 4uS
 * Tsamp = 15 * 4 = 60uS
 * Conversion time = 12 * TAD = 48uS
 */

// useful macros
#define IS_CONVERSION_COMPLETE_ADC1()    AD1CON1bits.DONE
#define IS_SAMPLING_ADC()         AD1CON1bits.SAMP
#define SET_SAMP_BIT_ADC()        AD1CON1bits.SAMP=1

/*
 * channel scanning disabled - AD1CSSL
 * manual sample
 * internal counter ends sampling
 * starts conversion (auto-convert)
 * AN4 assigned as analog input - AD1CHS 
 * Internal band gap reference channel disabled - AD1PCFG
 * Internal half band gap reference channel disabled - AD1PCFG
 * Input voltage regulator output reference disabled - AD1PCFG
 * RB2/AN4 is configured as analog input - AD1PCFG
 * other pins are configured as digital input - AD1PCFG
 * MUX A Multiplexer used - AD1CHS
 * VR- configured as negative input for MUX A Multiplexer - AD1CHS
 * AN4 configured as positive input for  MUX A Multiplexer - AD1CHS
 * Auto-sample Time bits Tsamp = 15*TAD - AD1CON3
 * Conversion clock select bits 2 * Tcy - AD1CON3
 * Clock derived from system clock - AD1CON3
 * initialize ADC register  
 */
//void initADC() {
//    AD1PCFG = 0xFFEF;
//    AD1CHS = 0x004;
//    AD1CON3 = 0x0F01;       
//    AD1CON2 = 0x003C;
//    AD1CON1 = 0x00E4; 
//    AD1CSSL	= 0x0000;
//}

/*
 * manual conversion
 * initialize ADC
 */
void initADC() {
    AD1PCFG = 0xFFEF;
    AD1CON1 = 0;
    AD1CSSL = 0x0000;
    AD1CON2 = 0;
    AD1CON3 = 0x1F02; 
}

/*
 * initialize I/O Pins
 */
void initIO() {
    TRISBbits.TRISB14 = 1;  // set RB14 as input for record
    TRISBbits.TRISB15 = 1;  // set RB15 as input for playback
    TRISBbits.TRISB2 = 1;
    
    TRISBbits.TRISB13 = 0;  // set RB13 as output for record LED
    TRISBbits.TRISB5 = 0;   // set RB5 as output for playback LED
}

/*
 * initialize SPI
 */
void initSPI() {
    
}

/*
 * read from ADC
 */
int analogRead() {
    AD1CON1bits.ADON = 1; // 1. start ADC
    AD1CON1bits.SAMP = 1; // 2. start sampling
    
    while( AD1CON1bits.SAMP );      // wait until acquisition is done
    while( ! AD1CON1bits.DONE );    // wait until conversion done
 
    return ADC1BUF0;                // result stored in ADC1BUF0
}

/*
 * record noise
 */
void doRecord() {
    RECORD_LED = 1;
    PLAYBACK_LED = 0;
    analogRead();
}

/*
 * playback recorded noise
 */
void doPlayback() {
    RECORD_LED = 0;
    PLAYBACK_LED = 1;
    AD1CON1bits.ADON = 1; // 1.stop ADC
}



int main(void) {
    
    initIO();
    initADC();
//    CLKDIVbits.RCDIV0 = 0;      //clock divider to 0 (8MHz))
    OSCCONbits.SOSCEN = 0;      //Disables the secondary oscillator
    PLAYBACK_LED = 0;           //turns Playback LED off
    RECORD_LED = 0;             //turns Record LED off
    
    while(1) {
        
        // if record button pressed
        if (PORTBbits.RB14) {
            doRecord();
        }
        
        if (PORTBbits.RB15) {
            doPlayback();
        }
    }
    
    return 0;
}
