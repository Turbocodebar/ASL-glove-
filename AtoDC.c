// Adding Library
//    I2C lcd 8x2 display
// by: Louis Diomande
// email: dioma004@umn.edu
// date: March 30, 2022

#include "xc.h"
#include "AtoDc.h"
#include "stdio.h"
#include "I2CDisplay.h"
#include "string.h"


void adc_init()
{
    
TRISAbits.TRISA0 = 1; //should be inputs by default, but just for safety..
TRISAbits.TRISA1 = 1;
TRISBbits.TRISB15 = 1;
TRISBbits.TRISB14 = 1;
TRISBbits.TRISB13 = 1;



// setup I/O for AN0, AN1, AN9, AN10, AN11
// should be I/O by default given the initialization function
AD1PCFGbits.PCFG0 = 0;
AD1PCFGbits.PCFG1=0; 
AD1PCFGbits.PCFG9=0;
AD1PCFGbits.PCFG10=0;
AD1PCFGbits.PCFG11=0;


AD1CON2bits.VCFG = 0b000; //Use AVDDA(3-3V) and AVSS (OV) as max min
AD1CON3bits.ADCS=1; //1 You want TAD >= 75ns (Tcy= 62.5ns) =2*Tcy
AD1CON1bits.SSRC= 0b010; //sample on timer3 events
AD1CON3bits.SAMC=0b00001; // You want at least 1 auto sample time bit
AD1CON1bits.FORM = 0b00;//data output form

AD1CON1bits.ASAM=1;// sampling immediately after last conversion
AD1CON2bits.SMPI= 0b0000; //interrupts at the completion of each sample sequence
AD1CON1bits.ADON=1;// // turn on the ADC


_AD1IF = 0;//clear IF
_AD1IE= 1;//enable Interrupt


TMR3=0;//setup timer3
T3CON= 0;
T3CONbits.TCKPS= 0b10;
PR3 =3124; //1s/80=12.5 ms or 80Hz
T3CONbits.TON=1; //turns on timer3

}

//the function below picks a channel because the pic24 ADC can only sample 
//one channel at the time 
void Select_ch(char x)
{
    if(x==0)
      AD1CHSbits.CH0SA=0b0000;//select AN0
        
    if(x==1)
      AD1CHSbits.CH0SA=0b0001;//select AN1
        
    if(x==2)
      AD1CHSbits.CH0SA=0b1001;//select AN9
  
    if(x==3)
      AD1CHSbits.CH0SA=0b1010;//select AN10
        
    if(x==4)
      AD1CHSbits.CH0SA=0b1011;//select AN11
}

#define BUFSIZE 8 //this variable sets your sample size 
//a sample size that is too high could compromise your design 
//because the moving average would take too long to be calculated 

//initialize buffers for each sensors(5 in this case for each finger)
int adc_buffer0[BUFSIZE];
int adc_buffer1[BUFSIZE];
int adc_buffer2[BUFSIZE];
int adc_buffer3[BUFSIZE];
int adc_buffer4[BUFSIZE];

int buffer_index=0;// all the buffers use the same index 

//initialize all the vales in the buffer to 0;
void initBuffer()
{
    int i;
    for (i=0;i<BUFSIZE;i++)
        adc_buffer0[i]=0;
        adc_buffer1[i]=0;
        adc_buffer2[i]=0;
        adc_buffer3[i]=0;
        adc_buffer4[i]=0;
}

void putVal(int ADCvalue, char ch)
{

 //select the right array depending on the channel currently being read by the ADC 
    
    if (ch==0)
        adc_buffer0[buffer_index]=ADCvalue;
        
        
    if (ch==1)
        adc_buffer1[buffer_index]=ADCvalue;
        
        
    if (ch==2)
      adc_buffer2[buffer_index]=ADCvalue;
       
        
    if (ch==3)
      adc_buffer3[buffer_index]=ADCvalue;
        
        
    if (ch==4)
     adc_buffer4[buffer_index]=ADCvalue;
        
    
    if(buffer_index>=BUFSIZE)
        buffer_index=0;
}

int getAvg(int A[])
{
    
    //compute the average for n elements (n=BUFSIZE)
    int j;
    unsigned long int tot=0;
    for (j=0;j<BUFSIZE;j++)
    {
        tot= tot + A[j];
    }
    
    return tot/BUFSIZE;
}

volatile char channel =0;
void __attribute__((interrupt,auto_psv)) _ADC1Interrupt (void)
{
    putVal(ADC1BUF0,channel++); //insert the values recorded in the array
    //changes the channel after reading a pin
    
    //post increment on the buffer index so that each buffer is filled one after
    //the other but always at the same rate 
    if (channel>4){
        channel=0;
        buffer_index++;
    }
    
    
    Select_ch(channel);// set the ADC so that it reads the next cahnnel 
    _AD1IF = 0;//clear IF
}

void timer1_init()
{
    TMR1=0;//setup timer1
    T1CON= 0;
    T1CONbits.TCKPS= 0b10;
    PR1 =24999;//100ms
    T1CONbits.TON=1; 
    
    _T1IE=1;
}


// The following functions just set each finger to binary digit given an intermediate value
// The in between value chosen depends on each finger given the application 
//all of the fingers will not be bending the same way so it has to be assigned 
//carefully after checking the voltage change between straight and bent.

int digitalValShort0(float A)
{
    if (A>2.75)
        return 1;
    else
        return 0;
}
int digitalValLong1(float A)
{
    if (A>2.25) 
        return 1;
    else
        return 0;
}
int digitalValLong2(float A)
{
    if (A>2) 
        return 1;
    else
        return 0;
}
int digitalValShort3(float A)
{
    if (A>2.6) 
        return 1;
    else
        return 0;
}
int digitalValShort4(float A)
{
    if (A>2.5) 
        return 1;
    else
        return 0;
}


volatile int F0=0;
volatile int F1=0;
volatile int F2=0;
volatile int F3=0;
volatile int F4=0;

volatile int overFlow=0;
void __attribute__((interrupt,auto_psv)) _T1Interrupt (void)
{
    
    
    _T1IF=0;//rest the interrupt flag
    overFlow++;
    
    //converts float values to binary values
    F0 = digitalValShort0((3.3/1024)*getAvg(adc_buffer0));
    F1 = digitalValLong1((3.3/1024)*getAvg(adc_buffer1));
    F2 = digitalValLong2((3.3/1024)*getAvg(adc_buffer2));
    F3 = digitalValShort3((3.3/1024)*getAvg(adc_buffer3));
    F4 = digitalValShort4((3.3/1024)*getAvg(adc_buffer4));
    
    
    
    //*debugging purpose*
    //change the adc_buuferx to check different voltage change for each finger
    
    
        //char str [20];
        //sprintf(str,"%6.4fV", (3.3/1024)*getAvg(adc_buffer4));
        //lcd_printStr(str);   
}

//the following functions just return the digital values so that those can be used 
//outside of the library.
int finger0()
{
    int A=F0;
    return A;
}

int finger1()
{
    int A=F1;
    return A;
}

int finger2()
{
    int A=F2;
    return A;
}

int finger3()
{
    int A=F3;
    return A;
}

int finger4()
{
    int A=F4;
    return A;
}

//
