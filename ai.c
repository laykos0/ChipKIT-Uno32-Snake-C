#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

void player_AI(int level)
{
  int a; // Snake direction to be set
  int proper = random_number(0, level);
  //  proper = 1;
  if (proper)
  {

    // If snake is not in the correct row => Set a to UP/DOWN accordingly
    if (foodTwoRow != snakeTwoX)
    {
      if (snakeTwoX > foodTwoRow)
      {
        a = 3;
      }
      else
      {
        a = 2;
      }
    }

    // If snake is not in the correct column => Set a to LEFT/RIGHT accordingly
    if (foodTwoColumn != snakeTwoY)
    {
      if (snakeTwoY > foodTwoColumn)
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }

    // Check for conflicting direction changes and correct a accordingly
    if ((directionTwo * a == 4) && (a != 2))
    {
      a = random_number(2, 3);
    }
    else if (directionTwo * a == 6)
    {
      if (random_number(0, 1))
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }
  }
  else if (!proper && (directionTwo == 1 || directionTwo == 4))
  {
    a = 2;
  }
  else
  {
    a = 4;
  }
  directionTwo = a;
}

void AI_hard(int level)
{
  int a; // Snake direction to be set
  int proper = random_number(0, 60);
  //  proper = 1;
  if (proper)
  {

    // If snake is not in the correct row => Set a to UP/DOWN accordingly
    if (foodTwoRow != snakeTwoX)
    {
      if (snakeTwoX > foodTwoRow)
      {
        a = 3;
      }
      else
      {
        a = 2;
      }
    }

    // If snake is not in the correct column => Set a to LEFT/RIGHT accordingly
    if (foodTwoColumn != snakeTwoY)
    {
      if (snakeTwoY > foodTwoColumn)
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }

    // Check for conflicting direction changes and correct a accordingly
    if ((directionTwo * a == 4) && (a != 2))
    {
      a = random_number(2, 3);
    }
    else if (directionTwo * a == 6)
    {
      if (random_number(0, 1))
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }
  }
  else if (!proper && (directionTwo == 1 || directionTwo == 4))
  {
    a = 2;
  }
  else
  {
    a = 4;
  }

  // Avoid obstacles when going RIGHT
  if (a == 1)
  {
    if ((read_pixel(snakeTwoX, snakeTwoY + 1) || read_pixel(snakeTwoX - 1, snakeTwoY + 1)) && !(((snakeTwoY + 2) == foodTwoColumn) && snakeTwoX == foodTwoRow))
    {
      if (directionTwo == 2)
      {
        a = 2;
      }
      else
      {
        a = 3;
      }
    }
  }

  // Avoid obstacles when going DOWN
  if (a == 2)
  {
    if ((read_pixel(snakeTwoX + 1, snakeTwoY) || read_pixel(snakeTwoX + 1, snakeTwoY - 1)) && !(((snakeTwoX + 2) == foodTwoRow) && snakeTwoY == foodTwoColumn))
    {
      if (directionTwo == 4)
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }
  }

  // Avoid obstacles when going UP
  if (a == 3)
  {
    if ((read_pixel(snakeTwoX - 2, snakeTwoY) || read_pixel(snakeTwoX - 2, snakeTwoY - 1)) && !(((snakeTwoX - 2) == foodTwoRow) && snakeTwoY == foodTwoColumn))
    {
      if (directionTwo == 4)
      {
        a = 4;
      }
      else
      {
        a = 1;
      }
    }
  }

  // Avoid obstacles when going LEFT
  if (a == 4)
  {
    if ((read_pixel(snakeTwoX, snakeTwoY - 2) || read_pixel(snakeTwoX - 1, snakeTwoY - 2)) && !(((snakeTwoY - 2) == foodTwoColumn) && snakeTwoX == foodTwoRow))
    {
      if (directionTwo == 2)
      {
        a = 2;
      }
      else
      {
        a = 3;
      }
    }
  }

  directionTwo = a;
}
