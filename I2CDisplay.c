// Adding Library
//    I2C lcd 8x2 display
// by: Louis Diomande
// email: dioma004@umn.edu
// date: March 30, 2022

#include "xc.h"
#include "I2CDisplay.h"

void delayms(unsigned int m)
{
    while(m-->0)
    {
        asm("repeat #15993");
        asm("nop");
    }
}

void lcd_cmd(char Package)
{
    //send start
    IFS3bits.MI2C2IF=0;
    I2C2CONbits.SEN=1;
    while(I2C2CONbits.SEN==1);
    
    //send address and write command
    IFS3bits.MI2C2IF=0;
    I2C2TRN=0b01111100;
    while(IFS3bits.MI2C2IF==0);
    
    //send high byte (always 0x00)
    IFS3bits.MI2C2IF=0;
    I2C2TRN=0b00000000;
    while(IFS3bits.MI2C2IF==0);
    
    //send package
    IFS3bits.MI2C2IF=0;
    I2C2TRN=Package;
    while(IFS3bits.MI2C2IF==0);
    
    //send STOP
    I2C2CONbits.PEN=1;
    while(I2C2CONbits.PEN==1);

}

void lcd_init(void)
{
    
    I2C2BRG=0x9D; //100kHz
    IFS3bits.MI2C2IF=0; // clear interrupt flag 
    I2C2CONbits.I2CEN=1; //Enable I2C Mode
    
    delayms(40);
    
    lcd_cmd(0b00111000); //function set 
    lcd_cmd(0b00111001); //function set, advance instruction mode
    lcd_cmd(0b00010100); //interval osc
    lcd_cmd(0b01110000); //contrast low
    lcd_cmd(0b01010110);
    lcd_cmd(0b01101100);// follower control
    
    delayms(200);
    
    lcd_cmd(0b00111000); //function set
    lcd_cmd(0b00001100); //Display on
    lcd_cmd(0b00000001);// Clear display 
    
    delayms(1);
}

void lcd_setCursor(char x,char y)
{
    if ((x==0||x>0)&&(y==0||y>0))
    {
    lcd_cmd((0x40*(x) + (y))|0b10000000);
    }
  

}


void lcd_printChar(char myChar)
{
   I2C2CONbits.SEN = 1;	//Initiate Start condition
  while(I2C2CONbits.SEN == 1);  // SEN will clear when Start Bit is complete
  
  IFS3bits.MI2C2IF=0;
  I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
  while(IFS3bits.MI2C2IF == 0);
  
   IFS3bits.MI2C2IF=0;
  I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
  while (IFS3bits.MI2C2IF == 0);
  
 IFS3bits.MI2C2IF=0;
  I2C2TRN = myChar; // 8-bits consisting of the data byte
  while( IFS3bits.MI2C2IF == 0);
  
  I2C2CONbits.PEN = 1;
  while (I2C2CONbits.PEN==1); // PEN will clear when Stop bit is complete

}

void lcd_printStr(const char s[])
{
    int size=0;
    int A=0;
    int row=2;
    int i,pos=0;
    
    while(s[A]!='\0')
    {
        A++;
        size++;
    }
    
    while(i<size)
    {          
        lcd_setCursor(row,pos++);
        lcd_printChar(s[i]);
        i++;
        
        if (i==6)
        {
         row=1;
         pos=0;
         
        } 

    }
    

}


