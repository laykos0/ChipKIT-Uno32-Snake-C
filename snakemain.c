/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

	 This file modified 2022-03-03 by Jakub Rybak

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"	 /* Declatations for snake */

int dead = 2; // 0. game, 1. score, 2.menu
int dead1 = 0;
int dead2 = 0;
int seed = 0;

int main(void)
{

	/*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
	while (OSCCON & (1 << 21))
		;				  /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while (OSCCON & (1 << 21))
		;		  /* Wait until PBDIV ready */
	SYSKEY = 0x0; /* Lock OSCCON */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
	SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	//----------------------】
	volatile int *t = (volatile int *)0xbf886100; // *Address of {TRISE}*

	*t &= 0xff00;	 // *Set bits 0-7 as output {TRISE}*
	TRISD |= 0x0fe0; // *Set bits 5-11 to input {TRISD}*
	//----------------------】
	//
	int highscore[] = {0, 0, 0};

	display_init();

	while (1)
	{ // Infinite program loop
		if (dead = 2)
		{ // MENU
			start_page();
			int choise1 = 0;
			while (!(getsw() & 0x1))
			{
				seed += 1; // Used in random number generation
				choise1 = start_choise(getbtns());
				start_display(choise1);
			}
			if (choise1 && (getsw() & 0x1))
			{ // Choice of game mode
				playernumber = choise1;
				if (choise1 == 3)
				{ // if check score chosen to go scoreboard
					scoreboard();
					continue;
				}
				start_diff(); // Choose diificullty page
				int choise2 = 0;
				while (!(getsw() & 0x1) || !getbtns())
				{
					choise2 = start_choise(getbtns());
					if (choise2)
						display_update(choise2, 20, 1);
				}
				if (choise2 && (getsw() & 0x1))
				{ // Choice of difficulty
					difficulty = choise2;
					dead = 0; // Start the game
				}
			}
		}
		if (!dead)
		{ // Proceed to game
			snakeinit();

			while (!dead) // Main game loop
			{
				game_loop();
				side_score(); // Display current score on the side of the boards
			}
		}
		if (dead == 1)
		{ // This part handles the score after the game ends
			who_won();
			quicksleep(3000000);
			while (!getbtns())
			{
				who_won();
			}
			add_score(1);
			if (changescreen == 1)
				between_score(1);
			if (playernumber == 2)
			{
				changescreen = 0;
				add_score(2);
				if (changescreen == 1)
					between_score(2);
			}
			if (changescreen == 2)
				scoreboard();
		}
	}
	return 0;
}
