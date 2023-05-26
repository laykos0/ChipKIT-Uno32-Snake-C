#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

// all player name need
int highscore[3] = {0, 0, 0};
char highname[3][10] = {{"      ----"}, {"      ----"}, {"      ----"}};
int score[3];
char current_name[10] = "      AAAA";
char score_char[9] = "         ";
int changescreen = 0;
int keepscore = 0;

void each_int(int s)
{
  score[0] = s / 100;
  score[1] = (s / 10) % 10;
  score[2] = (s % 10);
}

void scoreto_c(int s[])
{
  score_char[6] = score[0] + '0';
  score_char[7] = score[1] + '0';
  score_char[8] = score[2] + '0';
}
// scoreto_c and each_int  used to change each digit of int score to char array

// player choose name 4 letter
void add_score(int player)
{
  // change name display score
  // each_int(732);
  if (player == 1)
  {
    each_int(snakeOneLength - 1);
    scoreto_c(score);
    display_string(0, "  PLAYER1 NAME");
  }
  else
  {
    each_int(snakeTwoLength - 1);
    scoreto_c(score);
    display_string(0, "  PLAYER2 NAME");
  }
  display_string(1, current_name);
  display_string(2, "     SCORE:");
  display_string(3, score_char);
  display_update(1, 6, 0);
  int i = 0;
  int k = 0;
  while (k < 4)
  {

    while (!(getsw() & (1 << k)))
    { // first letter
      i = i % 26;
      if (getbtns() & 0x1)
        i++;
      current_name[k + 6] = i + 17 + '0';
      display_string(1, current_name);
      display_update(1, k + 6, 0);
    }
    if (getsw())
    {
      if (k == 3)
      {
        changescreen = 1;
        display_update(1, 20, 0);
      }
      else
      {
        display_update(1, k + 7, 0);
      }
    }
    k++;
  }
}

// tells if players score will make it to the top 3
void between_score(int player)
{
  // tells if on high score
  int j = 0;
  while (j < 3)
  { // from highest score to lowest check if score higher than prev top3
    if (player == 1)
    {
      if ((snakeOneLength - 1) >= highscore[j])
      {
        keepscore = 1;
        break;
      }
      j++;
    }
    else
    {
      if ((snakeTwoLength - 1) >= highscore[j])
      {
        keepscore = 1;
        break;
      }
      j++;
    }
  }
  // place the player score(if in top3) in the array of top 3 score in order from high to low number.
  if (keepscore == 1)
  {
    int i = 2;
    while (i > j)
    {
      highscore[i] = highscore[i - 1]; // swap score from high to low
      int n = 0;
      while (n < 10)
      {
        highname[i][n] = highname[i - 1][n]; // swap name with score
        n++;
      }
      i--;
    }
    int n = 0;
    while (n < 10)
    {
      highname[j][n] = current_name[n];
      n++;
    }
    if (player == 1)
      highscore[j] = snakeOneLength - 1;
    else
      highscore[j] = snakeTwoLength - 1;
    display_string(0, "");
    display_string(2, " you've made it");
    display_string(3, "  to the TOP 3");
    display_string(1, "congratulations");
    display_update(1, 0, 1);
    keepscore = 0;
  }

  else
  {
    display_string(0, "");
    display_string(2, " you are not in");
    display_string(3, "   the TOP 3");
    display_string(1, "      SAD");
    display_update(1, 0, 1);
  }
  while (!getbtns())
  {
    display_update(1, 0, 1);
  }
  changescreen = 2;
}

void scoreboard(void)
{
  // display3 highscores
  int i = 0;
  int j = 0;
  char top[16] = "                ";

  while (j < 3)
  {
    while (i < 16)
    {
      if (i < 8)
        top[i] = highname[j][i + 2]; // add name to display array
      else if (i < 13 && i > 9)
      {
        //	each_int(345);
        each_int(highscore[j]);
        scoreto_c(score);
        top[i] = score_char[i - 4]; // add score to display array
      }
      i++;
    }
    display_string(j + 1, top);
    i = 0;
    j++;
  }

  display_string(0, "  TOP 3 SCORE:  ");
  display_update(0, 0, 1);
  while (1)
  {
    if (getbtns() & 0x08)
    {
      dead = 2; // back to menu
      break;
    }
  }
}
