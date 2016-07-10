/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"
#include "NECRemote/NECRemote.c"
#include "PanelDisplay/PanelDisplay.c"

volatile int displayValue;
volatile char entryPosition;
volatile char runTimer;
volatile char runCountdown;

void handleRemote(void*);

int main()
{
  int prevDisplayValue;
  prevDisplayValue=displayValue=0;
  entryPosition=1;
  runTimer=0;
  runCountdown=0;
  initIR(27);
  initPanel(8,10,12);
  simpleterm_close();
  int soundBeep=0;

  int *cog = cog_run(&handleRemote, 60);
  
  DIRA |= 1 << 11;
 
  panelWriteNumber(displayValue);
  while(1)
  {
    if(displayValue!=prevDisplayValue){panelWriteNumber(displayValue);prevDisplayValue=displayValue;}
    if(soundBeep)
      {
        for(int i=0;i<10;i++)
        {
        freqout(16,30,800);
        pause(50);
        }
        soundBeep=0;
      }
    while(runCountdown)
    {
      panelWriteNumber(displayValue);
      displayValue--;        
      if(displayValue<=0)
      {
        soundBeep=1;
        runCountdown=0;        
       }
      pause(998);
    }      
  }  
}

void handleRemote(void *data)
{
  initIR(27);
  simpleterm_open();
  print("handleRemote started\n");
  while(1)
  {
    int32_t button = get_button();
    //print("button: 0x%x\n",button);
    switch(button)
    {
      case KEY_1:
      displayValue*=10;
      displayValue+=1;
      entryPosition++;;
      break;
      case KEY_2:
      displayValue*=10;
      displayValue+=2;
      entryPosition++;
      break;
      case KEY_3:
      displayValue*=10;
      displayValue+=3;
      entryPosition++;;
      break;
      case KEY_4:
      displayValue*=10;
      displayValue+=4;
      entryPosition++;
      break;
      case KEY_5:
      displayValue*=10;
      displayValue+=5;
      entryPosition++;;
      break;
      case KEY_6:
      displayValue*=10;
      displayValue+=6;
      entryPosition++;
      break;
      case KEY_7:
      displayValue*=10;
      displayValue+=7;
      entryPosition++;;
      break;
      case KEY_8:
      displayValue*=10;
      displayValue+=8;
      entryPosition++;
      break;
      case KEY_9:
      displayValue*=10;
      displayValue+=9;
      entryPosition++;;
      break;
      case KEY_0:
      displayValue*=10;
      displayValue+=0;
      entryPosition++;
      break;
      case KEY_RUN:
      runCountdown=!runCountdown;
      break;
      case KEY_FCN:
      displayValue=0;
      runCountdown=0;
      break;
    }      
  }    
}  