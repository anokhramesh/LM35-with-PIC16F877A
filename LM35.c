/*
 * File:Interfacing LM35 with PIC16F877A
 * Author:Anokhramesh
 *Created on August 23, 2022, 10:05 AM
 */
#include <xc.h>
#define _XTAL_FREQ 20000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & BOREN_OFF & LVP_ON & CPD_OFF & WRT_OFF & CP_OFF);
#define EN PORTDbits.RD2// defining Ports
#define RW PORTDbits.RD1
#define RS PORTDbits.RD0
unsigned int a,b,c,d,e,f;// defining variables
unsigned int temp,adc;
float temperature,adc1;

void delay (unsigned int i)// calling delay function
{
while(i--);
}
void lcd_command(unsigned char a)//calling LCD COMMAND function
{
PORTC=a;
EN=1;
RS=0;
RW=0;
delay(500);
EN=0;
}
void chr(unsigned char b)
{
PORTC=b;
EN=1;
RS=1;
RW=0;
delay(500);
EN=0;
}
void str(const char *d,char n)
{
char o;
for(o=0;o<n;o++)
{
chr(d[o]);
delay(500);    
}
}
void lcd_intialise()
{
lcd_command(0x38);//LCD Command for 
lcd_command(0x06);//LCD Command for 
lcd_command(0x0c);//LCD Command for 
lcd_command(0x01);//LCD Command for 
}
void interrupt adc_conv()
{
   if(PIR1bits.ADIF==1) //ADIF Flag will become HIGH
   {
    adc=(ADRESH<<8);   // temp =  0x 0000 0000 0000 0000  & (ADRESH<<8) = 0x 0000 0011 0000 0000 & temp = 0x 0000 0011 0000 0000
	adc=adc+ADRESL;   // temp =  0x 0000 0011 1111 1111
    PIR1bits.ADIF=0; //clearing the ADIF flag
   }
}

void main(void) 
{
   INTCONbits.GIE=1;//global interrupt bit enable
    INTCONbits.PEIE=1;//peripheral interrupt bit enable
    PIE1bits.ADIE=1;//Analog to Digital converter  bit  enable
    
	TRISD=0X00;//set PortD as OUTPUT
	TRISC=0X00;//set PortC as OUTPUT
	
	PORTC=PORTD=0X00;//initial value for port C and D are LOW
	lcd_intialise();//LCD Initialization function
    
	lcd_command(0x80);//command for display on First raw first column
	str("TEMP:=",6);//display string with 6 character
	
    ADCON0=0X41;//control Register configuration
    ADCON1=0X8E;
    while(1)
	{ 
      ADCON0=ADCON0|(0X04);//Enable GO/Done bit of ADCON for ADC Conversion
      
       adc1 = adc / 2.046;  // temperature value finding
       
       temperature = adc1 * 100;  // fetching decimal place numbers
      
        lcd_command(0x89);    // 0123
		a=(int)temperature/10;   // 012
		b=(int)temperature%10;  // 3
		c=a%10;        // 2
		d=a/10;       // 01
		e=d%10;       // 1
		f=d/10;       // 0
		chr(f+0x30);
		chr(e+0x30);
        chr('.');
		chr(c+0x30);
		chr(b+0x30);
    }
    return;
}
