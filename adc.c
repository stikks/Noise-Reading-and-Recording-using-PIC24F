/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        adcmain.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24FJ128GA010
* Compiler:        MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Mark Pallones		02/25/11		first release
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with  PIC24FJ128GA010 controller
*
* DESCRIPTION:
* This code example shows on how to use ADC on PIC24F. Here I used two light up LED chasing one another through D3 TO D7 LED. 
* One I called user LED (brighter) and the other is the chased LED (dimmer). Once the chased LED hits by the user LED the chased LED
* will move to the right of user LED. Variable resistor R6 us used to move the user LED and hit the chased LED.
* Once the chased LED is in dead end (D3)and hits by user LED it will move on the first LED which is D10.
* Initially the position of chased LED is at random.
***************************************************************************************************************************************/

#include <p24FJ128GA010.h>
#include <stdlib.h>

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON &  FNOSC_FRC & POSCMOD_NONE)


#include "adcprototypes.h"

int main(void)
{
	int a,r,c;

    IOInit();               //Initialize the port
    ADCInit();              //Initialize the A/D converter
    ADCStart();				//Turn ADC on
    srand(ADC1BUF0);        //To seed the random sequence generated by rand()
    r = (0x80 >> (rand() & 0x7)); // To initiate the the position of the chased LED
    c = 0;
	while(1)
	{
	  while(!IFS0bits.AD1IF);  //waiting 
	  IFS0bits.AD1IF = 0;	
	  
	   a = ADCAvg();
       a=(0x80>>a); // move the USER LED from 0 -> leftmost LED.... 7-> rigtmost or from 7 -> rightmost LED.... 0-> leftmost LED
         while(a == r ) // as soon as the user LED hits the chased LED, generate a new one to the right.If the chased LED is on the 7th LED D3 it will back to starting LED D10
           { 
            r = a>>1;
   
          if (r>0x80){ 
           r=0x40;
	       }
          if (r<=0){
	        r=0x80;
            }
           } 
        // display the user LED and 50% dim random LED
         
        
          if ((c & 0xf) == 0) {
            
            LATA= a + r;
          }  
          else  {
            LATA = a ;
          }

        // counter to alternate loops
        c++;
     
	 } 
}


void ADCInit(void)
{
    AD1CON1 = 0x00E4; // integer format, auto conversion,sample after conversion ends             

	AD1CON2 = 0x003C; // AVDD & AVSS as references,Disable Scan mode,interrupt after 16 sample/convert sequence,16 buffer levels, use MuxA              

    AD1CON3 = 0x0D09;// AD Clock as derivative of system clock,13TAD,16 samples are collected in 1mSec,Fcy =4Mhz 1Tcy=0.26uS              

	AD1CHS = 0x0005;// Positive sample input channel for MUX A to use AN5,Negative input channel for MUX A to use VR-

	AD1PCFG = 0xFFDF; //AN5 analog pin

 	AD1CSSL	= 0x0000; // Channel Scanning Disabled

}



void IOInit(void)
{
	TRISDbits.TRISD6 = 1;			// Make PORTD<6> as input.
	TRISA = 0xFF00;					// Make PORTA<0:7> as outputs, PORTA<8:15> as inputs.
	
}

void ADCStart(void)
{

 AD1CON1bits.ADON = 1;     // Turn on the A/D converter
}

int ADCAvg(void)
{
unsigned long int temp; int avg;

temp = 	ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + 
		ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 + 
		ADC1BUF8 + ADC1BUF9 + ADC1BUFA + ADC1BUFB + 
		ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF;
avg = temp/2048;     // 2048 = 8/(16*1024)  take average, scale 10-bits to 3-bits.
return (avg);
}
