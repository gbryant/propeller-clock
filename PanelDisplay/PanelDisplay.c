/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h" 


volatile int CLOCK_PIN;// 8
volatile int  DATA_PIN;// 10
volatile int  DIMM_PIN;// 12

volatile unsigned char panelDigits[] = {0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};


void panelWriteNumber(int number);
void panelPushDigit(int digit);
void initPanel(int dataPin, int clockPin, int dimmPin);
/*
int main()
{
  low(DIMM_PIN);
  low(DATA_PIN);
  low(CLOCK_PIN);


 int myCounter=0;
  while(1)
  {
    panelWriteNumber(myCounter);
    myCounter++;
    if(myCounter>9999){myCounter=0;}
    pause(0);
    
  }  
}
*/

void initPanel(int clockPin, int dataPin, int dimmPin)
{
CLOCK_PIN=clockPin;
DATA_PIN=dataPin;
DIMM_PIN=dimmPin;
low(DIMM_PIN);
low(DATA_PIN);
low(CLOCK_PIN);
}

void panelPushDigit(int digit)
{
  if(digit>=0&&digit<=9)
  {
  unsigned char panelChar;
  int panelData;
  
   panelChar=panelDigits[digit];
   for(int i=0;i<8;i++)
   {
     panelData = panelChar & 0x01;
     if(panelData==1){high(DATA_PIN);}else{low(DATA_PIN);}
     high(CLOCK_PIN);
     low(CLOCK_PIN);
     panelChar=panelChar>>1;
   }  
 }        
}



void panelWriteNumber(int number)
{
  low(DIMM_PIN);
  panelPushDigit(number%10);
  panelPushDigit((number%100)/10);
  panelPushDigit((number%1000)/100);
  panelPushDigit(number/1000);
  high(DIMM_PIN);
}  
