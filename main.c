#include "header.h"

int main(void)
{
    initCurses();
    gameSetup();
    pthread_t t[2];
    pthread_create(&t[0], NULL, playerLoop, NULL);
    pthread_create(&t[1], NULL, render, NULL);

    pthread_join(t[1], NULL);

    terminateCurses();

    return 0;
}