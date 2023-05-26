/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   This file modified 2022-03-03 by Jakub Rybak

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "snake.h"   /* Declatations for snake */

/* Declare a helper function which is local to this file */
static void num32asc(char *s, int);

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc)
{
  int i;
  for (i = cyc; i > 0; i--)
    ;
}

uint8_t spi_send_recv(uint8_t data)
{
  while (!(SPI2STAT & 0x08))
    ;
  SPI2BUF = data;
  while (!(SPI2STAT & 1))
    ;
  return SPI2BUF;
}

/* Display initialization, for reference see: chipKIT Basic I/O Shield reference manual */
void display_init(void)
{
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  quicksleep(10);
  DISPLAY_ACTIVATE_VDD;
  quicksleep(1000000);

  spi_send_recv(0xAE);
  DISPLAY_ACTIVATE_RESET;
  quicksleep(10);
  DISPLAY_DO_NOT_RESET;
  quicksleep(10);

  spi_send_recv(0x8D);
  spi_send_recv(0x14);

  spi_send_recv(0xD9);
  spi_send_recv(0xF1);

  DISPLAY_ACTIVATE_VBAT;
  quicksleep(10000000);

  spi_send_recv(0xA1);
  spi_send_recv(0xC8);

  spi_send_recv(0xDA);
  spi_send_recv(0x20);

  spi_send_recv(0xAF);
}

/* Displays string */
void display_string(int line, char *s)
{
  int i;
  if (line < 0 || line >= 4)
    return;
  if (!s)
    return;

  for (i = 0; i < 16; i++)
    if (*s)
    {
      textbuffer[line][i] = *s;
      s++;
    }
    else
      textbuffer[line][i] = ' ';
}

/* Displays image of custom width specified by image_width */
void display_image(int x, uint8_t *data, int image_width)
{
  // int image_width := parameter specifying width of the image to be displayed
  int i, j;

  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(x & 0xF);
    spi_send_recv(0x10 | ((x >> 4) & 0xF));

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < image_width; j++)
      spi_send_recv(~data[i * image_width + j]);
  }
}

void display_update(int line, int letter_number, int letter_orline)
{
  // int line, int letter := parameter to determine which letter/line to reverse color
  // int letter_orLine := used to determine whether o recolor a letter or a line selected
  int i, j, k;
  // i := line number (0-3) | j := letter number in a line (0-15) | k := each letter 8pixel wide.
  int c;
  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(0x0);
    spi_send_recv(0x10);

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 16; j++)
    {
      c = textbuffer[i][j];
      if (c & 0x80)
        continue;
      if ((i != line) || ((!letter_orline) && (j != letter_number)))
      {
        for (k = 0; k < 8; k++)
          spi_send_recv(font[c * 8 + k]); // When no letter/line chosen display as normal
      }
      else
      {
        for (k = 0; k < 8; k++)
          spi_send_recv(~font[c * 8 + k]); // When letter/line chosen display inverse color
      }
    }
  }
}

/* Function similar to display_update, half of the width | int x := determine where text starts */
void display_side(int x, int line)
{
  int i, j, k;
  int c;
  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(x & 0xF);
    spi_send_recv(0x10 | ((x >> 4) & 0xF));

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 8; j++)
    {
      c = textbuffer[i][j];

      if (i != line)
      {
        for (k = 0; k < 8; k++)
          spi_send_recv(font[c * 8 + k]);
      }
      else
      {
        for (k = 0; k < 8; k++)
          spi_send_recv(~font[c * 8 + k]);
      }
    }
  }
}

/* Changes status on specified pixel in the first half of the screen */
void color_pixel(int row, int column, char status)
{
  uint8_t select; // uint8_t select := used in pixel selection

  if (status)
  {
    switch (row % 8)
    {
    case 0:
      select = 0xFE; // 0b11111110
      break;
    case 1:
      select = 0xFD; // 0b11111101
      break;
    case 2:
      select = 0xFB; // 0b11111011
      break;
    case 3:
      select = 0xF7; // 0b11110111
      break;
    case 4:
      select = 0xEF; // 0b11101111
      break;
    case 5:
      select = 0xDF; // 0b11011111
      break;
    case 6:
      select = 0xBF; // 0b10111111
      break;
    case 7:
      select = 0x7F; // 0b01111111
      break;
    default:
      return;
    }
  }
  else
  {
    switch (row % 8)
    {
    case 0:
      select = 0x01; // 0b00000001
      break;
    case 1:
      select = 0x02; // 0b00000010
      break;
    case 2:
      select = 0x04; // 0b00000100
      break;
    case 3:
      select = 0x08; // 0b00001000
      break;
    case 4:
      select = 0x10; // 0b00010000
      break;
    case 5:
      select = 0x20; // 0b00100000
      break;
    case 6:
      select = 0x40; // 0b01000000
      break;
    case 7:
      select = 0x80; // 0b10000000
      break;
    default:
      return;
    }
  }

  if (row < 0 || row > 31 || column < 0 || column > 63)
  {
    return;
  }
  else if (row <= 7)
  {
    if (status)
    {
      icon[column] &= select;
    }
    else
    {
      icon[column] |= select;
    }
  }
  else if (row <= 15)
  {
    if (status)
    {
      icon[column + 64] &= select;
    }
    else
    {
      icon[column + 64] |= select;
    }
  }
  else if (row <= 23)
  {
    if (status)
    {
      icon[column + 128] &= select;
    }
    else
    {
      icon[column + 128] |= select;
    }
  }
  else
  {
    if (status)
    {
      icon[column + 192] &= select;
    }
    else
    {
      icon[column + 192] |= select;
    }
  }
}

/* Reads status from specified pixel in the first half of the screen */
char read_pixel(int row, int column)
{
  char status;    // char status := return value
  uint8_t select; // uint8_t select := used in pixel selection

  switch (row % 8)
  {
  case 0:
    select = 0x01; // 0b00000001
    break;
  case 1:
    select = 0x02; // 0b00000010
    break;
  case 2:
    select = 0x04; // 0b00000100
    break;
  case 3:
    select = 0x08; // 0b00001000
    break;
  case 4:
    select = 0x10; // 0b00010000
    break;
  case 5:
    select = 0x20; // 0b00100000
    break;
  case 6:
    select = 0x40; // 0b01000000
    break;
  case 7:
    select = 0x80; // 0b10000000
    break;
  default:
    return;
  }

  if (row < 0 || row > 31 || column < 0 || column > 63)
  {
    return 0;
  }
  else if (row <= 7)
  {
    status = icon[column] & select;
    return !status;
  }
  else if (row <= 15)
  {
    status = icon[column + 64] & select;
    return !status;
  }
  else if (row <= 23)
  {
    status = icon[column + 128] & select;
    return !status;
  }
  else
  {
    status = icon[column + 192] & select;
    return !status;
  }
}

/* Resets contents of icon | Clears display by printing empty icon on first and second half of the display */
void clear_display()
{
  int count = 0;
  count = 0;
  while (count < 256)
  {
    icon[count] = 0xFF;
    count++;
  }
  display_image(0, icon, 64);
  display_image(64, icon, 64);
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc(char *s, int n)
{
  int i;
  for (i = 28; i >= 0; i -= 4)
    *s++ = "0123456789ABCDEF"[(n >> i) & 15];
}

/*
 * itoa
 *
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 *
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 *
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 *
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 *
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 *
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 *
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 *
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.

* The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ (24)
char *itoaconv(int num)
{
  register int i, sign;
  static char itoa_buffer[ITOA_BUFSIZ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ITOA_BUFSIZ - 1] = 0; /* Insert the end-of-string marker. */
  sign = num;                       /* Save sign. */
  if (num < 0 && num - 1 > 0)       /* Check for most negative integer */
  {
    for (i = 0; i < sizeof(maxneg); i += 1)
      itoa_buffer[i + 1] = maxneg[i];
    i = 0;
  }
  else
  {
    if (num < 0)
      num = -num;        /* Make number positive. */
    i = ITOA_BUFSIZ - 2; /* Location for first ASCII digit. */
    do
    {
      itoa_buffer[i] = num % 10 + '0'; /* Insert next digit. */
      num = num / 10;                  /* Remove digit from number. */
      i -= 1;                          /* Move index to next empty position. */
    } while (num > 0);
    if (sign < 0)
    {
      itoa_buffer[i] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return (&itoa_buffer[i + 1]);
}
