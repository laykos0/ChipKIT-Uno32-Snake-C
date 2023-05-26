#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for these labs */

int playernumber = 0;
int difficulty = 0;

void start_page(void)
{
  display_string(0, " SNAKE");
  display_string(1, "1PLAYER");
  display_string(2, "2PLAYER");
  display_string(3, " TOP 3");
  display_side(32, 0);
  display_image(0, startOne, 32);
  display_image(96, startTwo, 32);
}

void start_display(int b)
{ // move highlight to the choise chosen
  if (b)
  {
    display_side(32, b);
    display_image(0, startOne, 32);
    display_image(96, startTwo, 32);
  }
}

int start_choise(int buttonStatus)
{
  int a = 0;

  if (buttonStatus & 0x4)
    a = 1;
  else if (buttonStatus & 0x2)
    a = 2;
  else if (buttonStatus & 0x1)
    a = 3;

  return a;
}

void start_diff(void)
{
  // display_init();
  display_string(1, "     EASY");
  display_string(2, "    MEDIUM");
  display_string(3, "     HARD");
  display_string(0, "  DIFFICULTY");
  display_update(4, 20, 1);
  quicksleep(3000000);
}
