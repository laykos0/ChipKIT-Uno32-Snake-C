#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

/* Random number generator within specified boundaries */
int random_number(int min, int max)
{
  return (rand() % (max - min + 1) + min);
}

/* Checks if a square with the bottom right corner at (r,c) is ON */
char check_square(int r, int c)
{
  return read_pixel(r, c) && read_pixel(r - 1, c) && read_pixel(r, c - 1) && read_pixel(r - 1, c - 1);
}

/* Checks if a square with the bottom right corner at (r,c) is OFF */
char square_available(int r, int c)
{
  return !((read_pixel(r, c) || read_pixel(r - 1, c) || read_pixel(r, c - 1) || read_pixel(r - 1, c - 1)));
}

// Color part based on the location of rightbottom of 2x2 square (Row r, Column c)
void color_part(int r, int c, char status)
{
  color_pixel(r, c, status);
  color_pixel(r - 1, c, status);
  color_pixel(r, c - 1, status);
  color_pixel(r - 1, c - 1, status);
}

/* Generates food on specified board number */
void generate_food(int boardNumber)
{
  char generated = 0; // Tracks if food has been generated
  if (boardNumber == 1)
  {
    generated = 0;
    while (!generated)
    { // Avoid generating on snake/obstacles, randomize until correct spot found
      foodOneRow = random_number(2, 30);
      foodOneColumn = random_number(2, 30);
      if (square_available(foodOneRow, foodOneColumn))
      {
        generated = 1;
      }
    }
    color_part(foodOneRow, foodOneColumn, 1); // Draw food
  }
  else if (boardNumber == 2)
  {
    generated = 0;
    while (!generated)
    { // Avoid generating on snake/obstacles, randomize until correct spot found
      foodTwoRow = random_number(2, 30);
      foodTwoColumn = random_number(34, 62);
      if (square_available(foodTwoRow, foodTwoColumn))
      {
        generated = 1;
      }
    }
    color_part(foodTwoRow, foodTwoColumn, 1); // Draw food
  }
  else
  {
    return;
  }
}

void generate_obstacles(int boardNumber, int obstacleNumber)
{
  int random = random_number(1, 2);
  char generated = 0; // Tracks if obstacle has been generated
  int tempRow, tempColumn;
  int count = 0;
  if (boardNumber == 1)
  {
    while (count < obstacleNumber)
    { // How many obstacles ::
      tempRow = random_number(3, 25);
      tempColumn = random_number(3, 25);
      random = random_number(1, 2);
      // Generate obstacle within game board avoiding snake starting position
      if (tempRow < 12 || tempRow > 16 || tempColumn < 12 || tempColumn > 17)
      {
        if (random == 1)
        { // Draw vertical obstacle
          color_pixel(tempRow, tempColumn, 1);
          color_pixel(tempRow + 1, tempColumn, 1);
          color_pixel(tempRow + 2, tempColumn, 1);
          color_pixel(tempRow + 3, tempColumn, 1);
        }
        else
        { // Draw horizontal obstacle
          color_pixel(tempRow, tempColumn, 1);
          color_pixel(tempRow, tempColumn + 1, 1);
          color_pixel(tempRow, tempColumn + 2, 1);
          color_pixel(tempRow, tempColumn + 3, 1);
        }
        count++;
      }
    }
  }
  else if (boardNumber == 2)
  {
    while (count < obstacleNumber)
    { // How many obstacles ::
      // Generate obstacle within game board
      tempRow = random_number(3, 25);
      tempColumn = random_number(35, 57);
      random = random_number(1, 2);
      // Generate obstacle within game board avoiding snake starting position
      if (tempRow < 12 || tempRow > 16 || tempColumn < 44 || tempColumn > 49)
      {
        if (random == 1)
        { // Draw vertical obstacle
          color_pixel(tempRow, tempColumn, 1);
          color_pixel(tempRow + 1, tempColumn, 1);
          color_pixel(tempRow + 2, tempColumn, 1);
          color_pixel(tempRow + 3, tempColumn, 1);
        }
        else
        { // Draw horizontal obstacle
          color_pixel(tempRow, tempColumn, 1);
          color_pixel(tempRow, tempColumn + 1, 1);
          color_pixel(tempRow, tempColumn + 2, 1);
          color_pixel(tempRow, tempColumn + 3, 1);
        }
        count++;
      }
    }
  }
  else
  {
    return;
  }
}

/* Used in checking the collision, verify if pixel belongs to food */
char check_food(int r, int c, int foodR, int foodC)
{
  return ((r == foodR && c == foodC) || (r == foodR - 1 && c == foodC) || (r == foodR && c == foodC - 1) || (r == foodR - 1 && c == foodC - 1));
}

/* This function handles the collision of the snake with food/walls/obstacles/itself */
void check_collision(int r, int c, int snakeNumber)
{
  // Set food comparison parameters for respective snake
  int foodR, foodC;
  if (snakeNumber == 1)
  {
    foodR = foodOneRow;
    foodC = foodOneColumn;
  }
  else if (snakeNumber == 2)
  {
    foodR = foodTwoRow;
    foodC = foodTwoColumn;
  }
  else
  {
    return;
  }

  // Check for collision with food
  if (check_food(r, c, foodR, foodC))
  {
    color_part(foodR, foodC, 0);
    if (snakeNumber == 1)
    {
      generate_food(1);
      snakeOneLength++;
    }
    else if (snakeNumber == 2)
    {
      generate_food(2);
      snakeTwoLength++;
    }
  }
  else if (read_pixel(r, c))
  { // End the game in case of collision with other objects
    if (snakeNumber == 1)
      dead1 = 1;
    else
      dead2 = 1;
  }
  else
  {
    return;
  }
  if (dead1 && dead2)
  {
    dead = 1;
  }
}

/* This function is used in snake movement and handles tracking of the snake pieces */
void track_snake(int length, int row, int column, int snakenumber)
{
  int i = length - 1;
  if (snakenumber == 1)
  {
    while (i > 0)
    {
      snakeOneRow[i] = snakeOneRow[i - 1];
      snakeOneColumn[i] = snakeOneColumn[i - 1];
      i--;
    }
    snakeOneRow[0] = row;
    snakeOneColumn[0] = column;
  }
  else if (snakenumber == 2)
  {
    while (i > 0)
    {
      snakeTwoRow[i] = snakeTwoRow[i - 1];
      snakeTwoColumn[i] = snakeTwoColumn[i - 1];
      i--;
    }
    snakeTwoRow[0] = row;
    snakeTwoColumn[0] = column;
  }
}

/* This function is used to move the snake */
void move_snake(int snakeNumber)
{
  if (snakeNumber == 1)
  {
    if (snakeOneLength > 1)
    {
      color_part(snakeOneRow[snakeOneLength - 1], snakeOneColumn[snakeOneLength - 1], 0);
      color_part(snakeOneRow[snakeOneLength - 2], snakeOneColumn[snakeOneLength - 2], 1);
    }
    else
    {
      color_part(snakeX, snakeY, 0);
    }
    switch (directionOne)
    {
    case 1: // RIGHT
      snakeY++;
      check_collision(snakeX, snakeY, 1);
      check_collision(snakeX - 1, snakeY, 1);
      break;
    case 2: // DOWN
      snakeX++;
      check_collision(snakeX, snakeY, 1);
      check_collision(snakeX, snakeY - 1, 1);
      break;
    case 3: // UP
      snakeX--;
      check_collision(snakeX - 1, snakeY, 1);
      check_collision(snakeX - 1, snakeY - 1, 1);
      break;
    case 4: // LEFT
      snakeY--;
      check_collision(snakeX, snakeY - 1, 1);
      check_collision(snakeX - 1, snakeY - 1, 1);
      break;
    default:
      return;
    }
    color_part(snakeX, snakeY, 1);
  }
  if (snakeNumber == 2)
  {
    if (snakeTwoLength > 1)
    {
      color_part(snakeTwoRow[snakeTwoLength - 1], snakeTwoColumn[snakeTwoLength - 1], 0);
      color_part(snakeTwoRow[snakeTwoLength - 2], snakeTwoColumn[snakeTwoLength - 2], 1);
    }
    else
    {
      color_part(snakeTwoX, snakeTwoY, 0);
    }
    switch (directionTwo)
    {
    case 1: // RIGHT
      snakeTwoY++;
      check_collision(snakeTwoX, snakeTwoY, 2);
      check_collision(snakeTwoX - 1, snakeTwoY, 2);
      break;
    case 2: // DOWN
      snakeTwoX++;
      check_collision(snakeTwoX, snakeTwoY, 2);
      check_collision(snakeTwoX, snakeTwoY - 1, 2);
      break;
    case 3: // UP
      snakeTwoX--;
      check_collision(snakeTwoX - 1, snakeTwoY, 2);
      check_collision(snakeTwoX - 1, snakeTwoY - 1, 2);
      break;
    case 4: // LEFT
      snakeTwoY--;
      check_collision(snakeTwoX, snakeTwoY - 1, 2);
      check_collision(snakeTwoX - 1, snakeTwoY - 1, 2);
      break;
    default:
      return;
    }
    color_part(snakeTwoX, snakeTwoY, 1);
  }
}
