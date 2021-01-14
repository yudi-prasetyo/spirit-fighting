#include "header.h"

pthread_mutex_t mutex;
int end = 0;
int winner = -1;

void initCurses()
{
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    clear();
    noecho();
    curs_set(0);
    cbreak();
    if ((curs_set(0)) == 1) {
        fprintf(stderr, "Error menyembuniykanb cursor \n");
        exit(EXIT_FAILURE);
    }	
    keypad(mainwin, TRUE);
    box( mainwin, ACS_VLINE, ACS_HLINE );
    getmaxyx(mainwin, max_y, max_x);
    refresh();
}

void terminateCurses()
{
    curs_set(1);
    clear();
    refresh();
    resetty();	
    endwin();
}

void gameSetup()
{
    players[0].health = players[1].health = 100;
    players[0].power = players[1].power = 0;
    players[0].defend = players[1].defend = 0;
    players[0].damage = players[1].damage = 5;
    players[0].x = 0;
    players[1].x = max_x;
    characterSetup(0);
    characterSetup(1);
}

void characterSetup(int n)
{
    players[n].karakter[0][0] = ' ';
    players[n].karakter[0][1] = 'o';
    players[n].karakter[0][2] = ' ';
    players[n].karakter[0][3] = ' ';
    players[n].karakter[1][0] = 47 + (n * 45);
    players[n].karakter[1][1] = '|';
    players[n].karakter[1][2] = 92 - (n * 45);
    players[n].karakter[1][3] = ' ';
    players[n].karakter[2][0] = 47 + (n * 45);
    players[n].karakter[2][1] = ' ';
    players[n].karakter[2][2] = 92 - (n * 45);
    players[n].karakter[2][3] = ' ';
}

void *playerLoop()
{
    while ((input != 'q') && (winner == -1))
    {
        input = wgetch(mainwin);
        movement(input);
        
        if (players[0].power == 100)
        {
            int n = 0;
            pthread_t thread_power;
            pthread_create(&thread_power, NULL, powerCooldown, &n);
        }
        if (players[1].power == 100)
        {
            int n = 1;
            pthread_t thread_power;
            pthread_create(&thread_power, NULL, powerCooldown, &n);
        }
        
        if (players[0].health == 0)
            winner = 1;
        if (players[1].health == 0)
            winner = 0;
    }
}

void *powerCooldown(void *playerNumber)
{
    int n = *(int*) playerNumber;
    players[n].damage = 10;
    while (players[n].power > 0)
    {
        players[n].power -= 1;
        msleep(50);
    }
    players[n].damage = 5;
}

void movement(char input)
{
    if ((input == P1_RIGHT) && (players[0].x + 7 < players[1].x))
        players[0].x++;
    else if ((input == P2_LEFT) && (players[0].x + 7 < players[1].x))
        players[1].x--;
    else if ((input == P1_LEFT) && (players[0].x >= 0))
        players[0].x--;
    else if ((input == P2_RIGHT) && (players[1].x <= max_x))
        players[1].x++;
    else if (input == P1_ATTACK)
    {
        pthread_t attack_thread;
        int n = 0;
        pthread_create(&attack_thread, NULL, attackAnimation, &n);
        pthread_join(attack_thread, NULL);
        if (players[1].x - players[0].x - 7 == 0)
            attack(n);
    }
    else if (input == P2_ATTACK)
    {
        pthread_t attack_thread;
        int n = 1;
        pthread_create(&attack_thread, NULL, attackAnimation, &n);
        pthread_join(attack_thread, NULL);
        if (players[1].x - players[0].x - 7 == 0)
            attack(n);
    }
    else if (input == P1_DEFEND)
    {
        pthread_t defend_thread;
        int n = 0;
        pthread_create(&defend_thread, NULL, defend, &n);
        pthread_join(defend_thread, NULL);
    }
    else if (input == P2_DEFEND)
    {
        pthread_t defend_thread;
        int n = 1;
        pthread_create(&defend_thread, NULL, defend, &n);
        pthread_join(defend_thread, NULL);
    }
}

void *attackAnimation(void *playerNumber)
{
    int n = *(int*) playerNumber;
    players[n].karakter[0][0] = ' ';
    players[n].karakter[0][1] = 'o';
    players[n].karakter[0][2] = '_';
    players[n].karakter[0][3] = ' ';
    players[n].karakter[1][0] = '-';
    players[n].karakter[1][1] = '|';
    players[n].karakter[1][2] = '_';
    players[n].karakter[1][3] = '_';
    players[n].karakter[2][0] = 60 + (n * 2);
    players[n].karakter[2][1] = ' ';
    players[n].karakter[2][2] = ' ';
    players[n].karakter[2][3] = ' ';
    msleep(500);
    characterSetup(n);
}

void attack(int n)
{
    int enemy = (n + 1) % 2;
    if (players[enemy].defend == 0)
    {
        players[enemy].health -= players[n].damage;
        players[n].power += 10;
        if (enemy == 0)
        {
            if (players[enemy].x > 5)
                players[enemy].x -= 5;
            else
                players[enemy].x -= players[enemy].x;            
        }
        else if (enemy == 1)
        {
            if (players[enemy].x < max_x - 5)
                players[enemy].x += 5;
            else
                players[enemy].x += max_x - players[enemy].x;            
        }
    }
}

void *defend(void *playerNumber)
{
    int n = *(int*) playerNumber;
    int enemy = (n + 1) % 2;
    players[n].defend = 1;
    players[n].karakter[0][0] = ' ';
    players[n].karakter[0][1] = 'o';
    players[n].karakter[0][2] = '_';
    players[n].karakter[0][3] = '|';
    players[n].karakter[1][0] = 47 + (n * 45);
    players[n].karakter[1][1] = '|';
    players[n].karakter[1][2] = ' ';
    players[n].karakter[1][3] = ' ';
    players[n].karakter[2][0] = 47 + (n * 45);
    players[n].karakter[2][1] = ' ';
    players[n].karakter[2][2] = 92 - (n * 45);
    players[n].karakter[2][3] = ' ';
    msleep(1000);
    players[n].defend = 0;
    characterSetup(n);
}

void *render()
{
    while ((input != 'q') && (winner == -1))
    {
        clear();
        move(1, 0);
        printw("Health  :   %d  %d\n", players[0].health, players[1].health);
        move(2, 0);
        printw("Power   :   %d  %d\n", players[0].power, players[1].power);
        move(3, 0);
        printw("Damage  :   %d  %d\n", players[0].damage, players[1].damage);
        printw("Defend  :   %d  %d\n", players[0].defend, players[1].defend);
        // printw("X  :   %d  %d\n", players[0].x, players[1].x);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                move(max_y - 3 + i, players[0].x + j);
                addch(players[0].karakter[i][j]);
            }

            int y_2 = players[1].x - 4;
            for (int j = 0; j < 4; j++)
            {
                move(max_y - 3 + i, y_2 + j);
                addch(players[1].karakter[i][3 - j]);
            }
        }
        msleep(1000 / 60);
    }

    if (winner != -1)
    {
        clear();
        printw("Pemenangnya adalah Player %d (Tekan 'q' untuk keluar)\n", winner + 1);
        while (input != 'q')
            input = wgetch(mainwin);
    }
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}