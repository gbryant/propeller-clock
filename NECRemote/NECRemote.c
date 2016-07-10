/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"

//0x3FC0 is the device code of my BETAbrite remote

#define KEY_1 0x1
#define KEY_2 0x2
#define KEY_3 0x3
#define KEY_4 0x4
#define KEY_5 0x18
#define KEY_6 0x5
#define KEY_7 0x6
#define KEY_8 0x7
#define KEY_9 0x8
#define KEY_0 0x1C
#define KEY_RUN 0x12
#define KEY_DELETE 0x56
#define KEY_PROGRAM 0x17
#define KEY_MODE 0x15
#define KEY_FONT 0x16
#define KEY_FCN 0x14

int32_t get_code(void);
int32_t get_pair(void);
int32_t get_button(void);

// length of a valid start mark pulse
  static volatile const int _sml = 200;
//  is at least this long
// length of space at end of sequence
  static volatile const int _esl = 500;
//  is at least this long
// space less the _zth is 0 else 1
  static volatile const int _zth = 37;
// mark stuck on (shouldn't happen)
  static volatile const int _son = 1000;
// must be a power of 2 (2,4,8,16,32,64,128,256 only)
  static volatile const int _qsize = 16;
// mask 
  //static const int _qsm1 = (_qsize - 1);
  static volatile const int _qsm1 = (16 - 1);
  
  volatile uint8_t	irpin;
  
  static volatile int32_t Shr__(uint32_t a, uint32_t b) { return (a>>b); }
  
  void initIR(uint8_t pin);
  
  void initIR(uint8_t pin)
  {
  irpin = pin;
  DIRA &= ~(1<<irpin);
  }
/*
int main()
{

  int32_t code;
  
  
  irpin = 27;

  // input from pin (0 clear = input)
  DIRA &= ~(1<<irpin);

 
  while(1)
  {
    code = get_code();
    if(code==0x80000000){continue;}
    print("code: 0x%x\n",((Shr__(code, 16)) & 0xff));
  }  
}
*/

int32_t get_button(void)
{
//print("get_button() started...\n");
int32_t code;
//while(1)
{
code = get_code();
    if(code==0x80000000){return 0;}
    return ((Shr__(code, 16)) & 0xff);
	}
}

int32_t get_code(void)
{
//print("get_code() started...\n");
  int32_t	npairs, p, code, echk;
  /* 
    wait for and return the next valid ir code
    return (in hex) either 00KKDDDD new code
                    or     80000000 repeat
    where DDDD is the 16 bit device ID
          KK   is the 8  bit key code                  
   */
  while (1) {
    // count mark/space pairs
    npairs = 0;
    // shift bits to code
    code = 0;
    // wait for a start burst
	//print("waiting for start burst\n");
    waitpeq(0, ((1<<irpin)));
	//print("burst received\n");
    //  then get start pair
    p = get_pair();
	//print("p = get_pair(); finished\n");
    if (((Shr__(p, 16)) & 0xffff) < _sml) 
	{
      // glitch not a start code
      // tune this constant as needed
	  //print("glitch\n");
      continue;
    }
    while (1) {
      // for each data and stop pair
      p = get_pair();
      // first check for stop pair
      if ((p & 0xffff) > _esl) {
        // tune this constant as needed
        if (npairs == 0) {
          // repeat code
          return (int32_t)0x80000000U;
        }
        // accept code if key code == !key code
        // and a full 32 bits have been recieved
        // else start over
        // set echk to error check key byte
        //  uncomment line to ignore - some don't use this
        //  convension
        echk = -(((Shr__(code, 16)) & 0xff) == ((~(Shr__(code, 24))) & 0xff));
        // echk := true                        
        if ((npairs == 32) && (echk)) {
          return (code & 0xffffff);
        }
        break;
      }
      // count input bits as they come
      (npairs++);
      // lsb sent first so shift right
      code = Shr__(code, 1);
      if ((p & 0xffff) > _zth) {
        // tune this constant as needed
        // shift in a 1
        code = code | 0x80000000;
      }
    }
  }
  return 0;
}


int32_t get_pair(void)
{
  int32_t	iron, iroff;
  /* 
    get a single bit as a count of mark time and
    space time returned as two words in one long
   */
  // counts mark time
  iron = 0;
  // counts space time
  iroff = 0;
  while (((INA >> irpin) & 0x1) == 0) {
    (iron++);
    if (iron > _son) {
      // tune as needed
      break;
    }
  }
  while (((INA >> irpin) & 0x1) == 1) {
    (iroff++);
    if (iroff > _esl) {
      // tune as needed
      break;
    }
  }
  return ((iron << 16) | iroff);
}