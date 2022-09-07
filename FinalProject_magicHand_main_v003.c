/*
 * File:   FinalProject_magicHand_main_v003.c
 * Author: Louis Diomande, Alex Hawkins, Nathan Holzem, Rafe Covin
 *
 * Created on April 20, 2022, 4:56 PM
 */


#include "xc.h"
#include "I2CDisplay.h"
#include "AtoDC.h"
#include "stdio.h"




void setup()
{
_RCDIV=0;//Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
AD1PCFG=0xffff; //sets all pins to digital I/O
// AD1PCFG is going to be overridden by the libraries 

}


//used to translate fingers binary digits  to actual numerical values 
char ASL_value(int f0, int f1, int f2, int f3, int f4)
{
    char A='N'; // default values == combination has no translation
    
    
    if (f0==1 && f1==1 && f2==1 && f3==0 && f4==1)
        A='1';
    
    if (f0==1 && f1==1 && f2==0 && f3==0 && f4==1)
        A='2';
    
    if (f0==1 && f1==1 && f2==0 && f3==0 && f4==0)
        A='3';
    
    if (f0==0 && f1==0 && f2==0 && f3==0 && f4==1)
        A='4';
    
    if (f0==0 && f1==0 && f2==0 && f3==0 && f4==0)
        A='5';
    
    if (f0==1 && f1==0 && f2==0 && f3==0 && f4==1)
        A='6';
    
    if (f0==0 && f1==1 && f2==0 && f3==0 && f4==1)
        A='7';
    
    if (f0==0 && f1==0 && f2==1 && f3==0 && f4==1)
        A='8';
    
    if (f0==0 && f1==0 && f2==0 && f3==1 && f4==1)
        A='9';
    
    return A;     
}

 

void DispValues(){
    
    // set all chars to 0 has all fingers are straight at the beginning 
    char A='0';
    char B='0';
    char C='0';
    char D='0';
    char E='0';
    char Y='0';
        
    
        Y=ASL_value(finger0(),finger1(),finger2(),finger3(),finger4());
        //assign the corresponding numerical value to Y
        
        
        if (finger0())
            A='1';
        
        if (finger1())
            B='1';
        
        if (finger2())
            C='1';
        
        if (finger3())
            D='1';
        
        if (finger4())
            E='1';
        
        
        //display the binary digits associated to each finger 
        //display the numerical value that corresponds to the fingers binary sequence
        lcd_setCursor(2,0);
        lcd_printChar(Y);
        lcd_setCursor(2,2);
        lcd_printChar(E);
        lcd_setCursor(2,3);
        lcd_printChar(D);
        lcd_setCursor(2,4);
        lcd_printChar(C);
        lcd_setCursor(2,5);
        lcd_printChar(B);
        lcd_setCursor(2,6);
        lcd_printChar(A);
    
}

int main(void) {
    
    setup();
    lcd_init();
    adc_init();
    timer1_init();
    initBuffer();
    
    
    
    
    
    while(1){
        
        DispValues();
        }
    
    
    return 0;
}
