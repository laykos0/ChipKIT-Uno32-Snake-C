#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

int getsw(void)
{
  // Shift by 8 := Extract 8-11| AND with 0xf := Remove unnecesary bits
  return ((PORTD >> 8) & 0xf);
}

int getbtns(void)
{
  // Shift by 5 := Extract 5-7| AND with 0x7 := Remove unnecesary bits | Shift by 1 to the LEFT
  int buttons1 = ((PORTD >> 5) & 0x7) << 1;
  // Shift by 1 := Extract 1| AND with 0x1 := Remove unnecesary bits
  int buttons2 = ((PORTF >> 1) & 0x1);
  return buttons1 + buttons2;
}
