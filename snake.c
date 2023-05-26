/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2022-03-03 by Jakub Rybak

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

#include <stdlib.h>   /* Import rand() and srand() for randomization */
#include "standard.h" /* Prevents stdlib compile errors */

/* General parameters */
int gameSpeed = 5;
int timeoutcount = 0; // timeoutcount := TMR2 timeout counter

/* Parameters for snakeOne */
int snakeX = 2;       // snakeX := Row coordinate of the head of the first snake
int snakeY = 2;       // snakeY := Column cooridnate of the head of the first snake
int directionOne = 1; // directionOne:= Tracks the direction of the first snake
// directionOne :: 1 := RIGHT | 2 := DOWN | 3 := UP | 4 := LEFT
int snakeOneLength = 1;  // snakeOneLength := Tracks length of the first snake
int snakeOneRow[900];    // snakeOneRow[900] := Array used in tracking parts of the first snake
int snakeOneColumn[900]; // snakeOneColumn[900] := Array used in tracking parts of the first snake

/* Parameters for snakeTwo */
int snakeTwoX = 2;    // snakeTwoX := Row coordinate of the head of the second snake
int snakeTwoY = 34;   // snakeTwoY := Column cooridnate of the head of the first snake
int directionTwo = 1; // directionTwo := Tracks the direction of the second snake
// directionTwo :: 1 := RIGHT | 2 := DOWN | 3 := UP | 4 := LEFT
int snakeTwoLength = 1;  // snakeTwoLength := Tracks the length of the second snake
int snakeTwoRow[900];    // snakeTwoRow[900] := Array used in tracking parts of the second snake
int snakeTwoColumn[900]; // snakeTwoColumn[900] := Array used in tracking parts of the second snake

/* Parameters for food checked during collisions */
int foodOneRow = 0;    // foodOneRow := Row coordinate of first food
int foodOneColumn = 0; // foodOneColumn := Column coordinate of the first food
int foodTwoRow = 0;    // foodTwoRow := Row coordinate of the second food
int foodTwoColumn = 0; // foodTwoColumn := Column coordinate of the second food

int nofObstacles = 0;
int difflevel = 0;

/* enable_interrupt := Enables interrupts globally */
void enable_interrupt(void)
{
  asm volatile("ei"); // *Globally enable interrupts*
}

/* Interrupt Service Routine */
void user_isr(void)
{
  /* TMR2 Interrupt handler */
  if (IFS(0) & 0x100)
  {                    // Check TMR2 timeout flag by anding with 8th bit of IFS(0)
    IFSCLR(0) = 0x100; // Reset TMR2 timeout flag by clearing 8th bit of IFS(0)
    timeoutcount++;    // Increment timeoutcount every TMR2 timeout event
    if (timeoutcount == gameSpeed)
    { // Call every gameSpeed timeouts
      timeoutcount = 0;
      // Written by Litian Lei
      if (!dead1 && !dead)
      {
        move_snake(1);
        track_snake(snakeOneLength, snakeX, snakeY, 1);
      }
      if (!dead2 && !dead)
      {
        if (playernumber == 1)
        {
          if (difficulty == 1)
          {
            player_AI(difflevel);
          }
          else
          {
            AI_hard(difflevel);
          }
        }
        move_snake(2);
        track_snake(snakeTwoLength, snakeTwoX, snakeTwoY, 2);
      }
    }
  }
}

/* Snake initialization here */
void snakeinit(void)
{
  // *TIMERS*
  T2CON = 0x0; // *Clear the control register*
  TMR2 = 0x0;  // *Clear the timer register*

  T2CONSET = 0x8070;             // *Set prescale to 1:256 and start timer*
  PR2 = ((80000000 / 256) / 10); // *Sets period to 0.1s*

  // *INTERRUPTS*
  IECSET(0) = 0x100; // *Enable interrupt for TMR2 (Bit 8)*
  IPCSET(2) = 4;     // *Set interrupt priority := Bits 2,3,4 (Can use 4-31)*

  clear_display();
  int count = 0;

  /* Draws the first game board */
  count = 0;
  while (count < 32)
  {
    color_pixel(count, 0, 1);
    color_pixel(count, 31, 1);
    color_pixel(0, count, 1);
    color_pixel(31, count, 1);
    count++;
  }

  /* Draws the second game board */
  count = 0;
  while (count < 32)
  {
    color_pixel(count, 0 + 32, 1);
    color_pixel(count, 31 + 32, 1);
    color_pixel(0, count + 32, 1);
    color_pixel(31, count + 32, 1);
    count++;
  }

  /* Set up for randomization */
  srand(seed);

  /* Generate food of both game boards */
  generate_food(1);
  generate_food(2);

  /* Set initial first snake parameters */
  snakeOneLength = 1;
  snakeTwoLength = 1;
  snakeX = 16;
  snakeY = 16;
  snakeOneRow[0] = snakeX;
  snakeOneColumn[0] = snakeY;
  snakeOneRow[1] = snakeX;
  snakeOneColumn[1] = snakeY - 1;

  /* Set initial second snake parameters */
  snakeTwoX = 16;
  snakeTwoY = 48;
  snakeTwoRow[0] = snakeTwoX;
  snakeTwoColumn[0] = snakeTwoY;
  snakeTwoRow[1] = snakeTwoX;
  snakeTwoColumn[1] = snakeTwoY - 1;

  /* Set difficulty parameters */
  if (difficulty == 1)
  {
    nofObstacles = 0;
    difflevel = 6;
    gameSpeed = 3;
  }
  if (difficulty == 2)
  {
    nofObstacles = 0;
    difflevel = 8;
    gameSpeed = 1;
  }
  if (difficulty == 3)
  {
    nofObstacles = 5;
    difflevel = 60;
    gameSpeed = 1;
  }

  /* Generate obstacles on both game boards */
  generate_obstacles(1, nofObstacles);
  generate_obstacles(2, nofObstacles);

  /* Set game over parameter values */
  dead1 = 0;
  dead2 = 0;

  /* Start displaying boards and wait a bit */
  display_image(0, icon, 64);
  quicksleep(3000000);

  /* Enable interrupts globally */
  enable_interrupt();

  return;
}

/* This function is called repetitively from snakemain.c while the game is running */
void game_loop(void)
{
  /* Pooling of button and switch status */
  int buttonStatus = getbtns();
  int switchStatus = getsw();

  /* This part handles button presses */
  if (buttonStatus & 0x8 && directionOne != 1) // BTN4 := LEFT
    directionOne = 4;
  if (buttonStatus & 0x4 && directionOne != 2) // BTN3 := UP
    directionOne = 3;
  if (buttonStatus & 0x2 && directionOne != 3) // BTN2 := DOWN
    directionOne = 2;
  if (buttonStatus & 0x1 && directionOne != 4) // BTN1 := RIGHT
    directionOne = 1;

  if (playernumber == 2)
  {
    /* This part handles switch presses */
    if (switchStatus & 0x8 && directionTwo != 1) // SW4 := LEFT
      directionTwo = 4;
    if (switchStatus & 0x4 && directionTwo != 2) // SW3 := UP
      directionTwo = 3;
    if (switchStatus & 0x2 && directionTwo != 3) // SW2 := DOWN
      directionTwo = 2;
    if (switchStatus & 0x1 && directionTwo != 4) // SW1 := RIGHT
      directionTwo = 1;
  }
  display_image(0, icon, 64); // Updates display
}
