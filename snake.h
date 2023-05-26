/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   This file modified 2022-03-03 by Jakub Rybak

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from displayfunc.c */
void display_image(int x, uint8_t *data, int image_width); // Displays image from data of custom width
void display_init(void);                                   // Initializes the display
void display_string(int line, char *s);                    // Displays string
void display_update(int line, int letter_number, int letter_orline);
// letter=0,line=1
void quicksleep(int cyc);
char *itoaconv(int num);
uint8_t spi_send_recv(uint8_t data);

void display_side(int x, int line);

// Specific pixel access from displayfunc.c
void color_pixel(int row, int column, char status); // Changes status on specified pixel in the first half of the screen
char read_pixel(int row, int column);               // Reads status from specified pixel in the first half of the screen
void clear_display();                               // Clears contents of data and updates the whole display

/* Declare main function in snakemain.c */
int main(void);

/* Declare main game functions from snake.c */
void enable_interrupt(void); // Enables interrupts
void user_isr(void);         // Handles interrupts
void game_loop(void);        // Main game loop, handles input

/* Declare snake functions from snakefunctions.c */
int random_number(int min, int max);                                // Generates random number between min and max
void generate_food(int boardNumber);                                // Generates food on specified board
void generate_obstacles(int boardNumber, int obstacleNumber);       //  Generates obstales on specified board
void track_snake(int length, int row, int column, int snakenumber); // Tracks parts of snake
void color_part(int r, int c, char status);                         // Colors specific part of snake
void move_snake(int snakeNumber);                                   // Moves snake depending on the specified number
void check_collision(int r, int c, int snakeNumber);
char check_food(int r, int c, int foodR, int foodC);

/* I/O Functinality from snakeio.c */
int getbtns(void); // Get input from buttons
int getsw(void);   // Get input from switches

/* START PAGE*/
void start_page(void);
void start_display(int button);
int start_choise(int buttonStatus);
void start_diff(void);

/*-----Scoreboard-----*/
void add_score(int player);
void scoreto_c(int s[]);
void between_score(int player);
void scoreboard(void);
void side_score(void);
void each_int(int s);
void who_won(void);

/*-----AI-----*/
void player_AI(int level);
void AI_hard(int level);

// ====== Variables section  =====

/* Declare data from snake.c */
int gameSpeed;
int timeoutcount;

// Parameters for snake one
int snakeX;
int snakeY;
int snakeOneLength;
int directionOne; // 1 := RIGHT | 2 := DOWN | 3 := UP | 4 := LEFT
int snakeOneRow[900];
int snakeOneColumn[900];

// Parameters for snake two
int snakeTwoX;
int snakeTwoY;
int snakeTwoLength;
int directionTwo; // 1 := RIGHT | 2 := DOWN | 3 := UP | 4 := LEFT
int snakeTwoRow[900];
int snakeTwoColumn[900];

// Parameters for food
int foodOneRow;
int foodTwoRow;
int foodOneColumn;
int foodTwoColumn;

/* Declare data from start.c */
int playernumber;
int difficulty;

/* Declare data from main.c */
int dead;
int dead1;
int dead2;
int seed;

/* Declare data from scoreboard.c */
int changescreen;
int score[3];
int highscore[3];
char highname[3][10];
char current_name[10];
char score_char[9];

/* Declare data from snakedata.c */
// Declare bitmap array containing font
extern const uint8_t const font[128 * 8];

// Declare bitmap arras for game boards and start screens
extern uint8_t icon[256];
extern uint8_t startTwo[128];
extern uint8_t startOne[128];

// Declare text buffer for display output
extern char textbuffer[4][16];
