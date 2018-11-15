/*
Jacob Pawlak
November 14 2018
Prime with signals :)
*/
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

int run = 1;
int prime;

int isPrime(int);
void interrupt(int);
int tty_mode(int how);
int set_cr_noecho_mode();
int set_cr_nocan_mode();

int main(int argc, char argv[]){
    
    tty_mode(0);
    signal(SIGINT, interrupt);
    int counter, current;
    counter = 3;
    while(run){
        set_cr_noecho_mode();
        if(isPrime(counter)==1){
            prime = counter;
        }
        counter++;
    }
    return 0;
    tty_mode(1); // restore 
}

int isPrime(int num){
    int i;
    for(i = 2; i <= (num/2); i++){
        if((num % i) == 0){
            return 0;
        }
    }
    return 1;
}

void interrupt(int signum){
    char response;
    tty_mode(1); // restore so we can print and see our char
    printf("%d\tQuit [y/n]? ", prime);
    set_cr_nocan_mode(); // set echo and return off
    response = getchar();
    tty_mode(1); // restore
    printf("\n"); // makes a new line and it looks clean
    if(response == 'y' || response == 'Y'){ // only take responees like this
        run = 0;
    }
}

int set_cr_noecho_mode(){
    struct  termios ttystate;

    tcgetattr( 0, &ttystate);       /* read curr. setting   */
    ttystate.c_lflag        &= ~ICANON; /* no buffering     */
    ttystate.c_lflag        &= ~ECHO;   /* no echo either   */
    ttystate.c_cc[VMIN]     =  1;       /* get 1 char at a time */
    tcsetattr( 0 , TCSANOW, &ttystate); /* install settings */
}

int set_cr_nocan_mode(){
    struct  termios ttystate;

    tcgetattr( 0, &ttystate);       /* read curr. setting   */
    ttystate.c_lflag        &= ~ICANON; /* no buffering     */
    //ttystate.c_lflag        &= ~ECHO;   /* no echo either   */
    ttystate.c_cc[VMIN]     =  1;       /* get 1 char at a time */
    tcsetattr( 0 , TCSANOW, &ttystate); /* install settings */
}

int tty_mode(int how){
    static struct termios original_mode;
    static int            original_flags;
    if ( how == 0 ){ // get original mode (so you can no echo and ICANON)
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else { // put original modes back!!
        tcsetattr(0, TCSANOW, &original_mode); 
        fcntl( 0, F_SETFL, original_flags); 
    }
}
