#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */

#include <stdlib.h>   /* Import rand() for random number generation */
#include "standard.h" // Prevents stdlib errors
#include "snake.h"    /* Declatations for these labs */

char player1_score[8] = "     000";
char player2_score[8] = "     000";
int win = 0;

void side_score(void)
{
  int sc1 = snakeOneLength - 1;
  int sc2 = snakeTwoLength - 1;
  player1_score[5] = (sc1 / 100) + '0';       // first digit
  player1_score[6] = ((sc1 / 10) % 10) + '0'; // 2nd digit
  player1_score[7] = (sc1 % 10) + '0';        // 3rd
  player2_score[5] = (sc2 / 100) + '0';
  player2_score[6] = ((sc2 / 10) % 10) + '0';
  player2_score[7] = (sc2 % 10) + '0';

  display_string(0, "P1:");
  display_string(1, player1_score);
  display_string(3, player2_score);
  if (playernumber == 1)
  {
    display_string(2, "PC:");
  }
  else if (playernumber == 2)
  {
    display_string(2, "P2:");
  }

  display_side(64, 5);
}

void who_won(void)
{
  if (snakeOneLength < snakeTwoLength)
    win = 2;
  else if (snakeTwoLength < snakeOneLength)
    win = 1;
  else
    win = 0;
  display_string(0, "");
  display_string(2, "");
  display_string(3, "");

  if (!win)
    display_string(1, "      DRAW");
  else if (win == 1)
    display_string(1, "  player1 won");
  else
  {
    if (playernumber == 1)
      display_string(1, "    you lose");
    else
      display_string(1, "  player2 won");
  }
  display_update(1, 20, 1);
}
