// #include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <curses.h>
#include <string.h>

#define clrscr() printf("\e[1;1H\e[2J")
#define P1_JUMP 'w'
#define P1_LEFT 'a'
#define P1_RIGHT 'd'
#define P1_ATTACK 'e'
#define P1_DEFEND 's'
#define P2_JUMP 'i'
#define P2_LEFT 'j'
#define P2_RIGHT 'l'
#define P2_ATTACK 'u'
#define P2_DEFEND 'k'
#define ARENA_WIDTH 50
#define ARENA_HEIGHT 10
// #define TOTAL_THREAD 3

// static struct termios old, current;

typedef struct
{
	int x;
	int health;
	int power;
	int damage;
	int defend;
	char karakter[3][4];
} player;

player players[2];
WINDOW* mainwin;
int max_y, max_x;
char input;

// void initTermios(int echo);
// void resetTermios(void);
// char getch(void);
void initCurses();
void terminateCurses();

void gameSetup();
void characterSetup(int n);
void *playerLoop();
void *powerCooldown(void *playerNumber);
void movement(char input);
void *attackAnimation(void *playerNumber);
void attack(int n);
void *defend(void *playerNumber);
void *render();
int msleep(long msec);