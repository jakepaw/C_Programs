/*
Author: Jscob Pawlak
Implement the marquee text scroll using the ncurses library (ncurses because more familiar with me and newer).
increase 1-99
quit with ctrl d
last program woop woop
*/

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int readIn();
void delay(int seconds);

char * buffer;
char * buff;

int main (int argc, char * argv[]){
	int length, row, col, speed;

	if(argc == 6){

		length = atoi(argv[2]);
		row    = atoi(argv[3]);
		col    = atoi(argv[4]);
		speed  = atoi(argv[5]);
		speed = 100 - speed; // make speed opposite :)
	
	}
	else{
		perror("Need more args");
		exit(2);
	}

	readIn(argv[1]);
	char * scroll;
	int text_length, i, maxx, maxy;
	text_length = strlen(buffer);

	initscr();
	curs_set(0);
	cbreak();
	clear();
	getmaxyx(stdscr, maxy, maxx);

	if(length > maxx){
		length = maxx-col;
	}

	scroll = malloc(2 * maxx + 1);

	for(i = 0; i < 2*maxx; i++){
		scroll[i] = buffer[i % text_length];
	}
	for(i = 0; i < 2*maxx; i++){
		scroll[i] = buffer[i % text_length];
	}
	scroll[2 * maxx -1] = '\0';

	i = 0;
	nodelay(stdscr, TRUE);
	system("stty intr ^d");
	while(1){
		
		mvaddnstr(row, col, &scroll[i % maxx], length);
		refresh();
		delay(speed);
		i++;

	}
	system("stty intr ^c");
	endwin();
	nocbreak();
	free(buffer); // freeing the buffer after our use to give mem bacj
	return 0;
	
}

int readIn(char * file){

	FILE * pFile;
	long lSize;
	size_t result;

	pFile = fopen ( file , "rb" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer
	result = fread (buffer, 1, lSize, pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	fclose (pFile);

	for(size_t i = 0; buffer[i]; i++){
		if( buffer[i] == '\n') buffer[i] = ' '; 
	}
	return 0;
}

void delay( int seconds ){
    long pause;
    clock_t now,then;

    pause = seconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}
