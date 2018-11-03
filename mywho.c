/*
Author: Jacob Pawlak
Date: 09/26/2018

This program recreates the 'who' command. We get all of our information from the UTMP_FILE and read information into a struct (acting as a buffer)
From here, we are able to access the peices of the struct and print out the necessary information to recreate 'who'.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

#define SHOWHOST

void show_info(struct utmp *);
void showtime(long);

int main()
{
	struct utmp utbuf; /*read info into this location*/
	int utmpfd; /* read from this descriptor (file) */
	
	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){/* If this file is not able to be opened with read only perms*/
		perror(UTMP_FILE);
		exit(1);
}
	while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) /* read from the descriptor into utbufp (buffer) until all sessions are read */
		show_info(&utbuf);
	close(utmpfd);
	return 0;	
}

/*
This function will read from the buffer which the file info is held, requesting user information from the struct (hence the ->)
We then print out all the useful information needed from the UTMP_FILE to re-create who.
Finally, we print out the time using our time function.
*/

void show_info( struct utmp * utbufp){

	if (utbufp->ut_type != USER_PROCESS) /* if this not exist, return*/
		return;

	printf("%-9.9s", utbufp->ut_name); /* print login name*/
	printf(" ");					   /* space */
	printf("%-8.8s", utbufp->ut_line); /* print the tty (filename of terminal connected)*/
	printf("     ");
	showtime(utbufp->ut_time); /* print out the timestamp in human readable form */

	#ifdef SHOWHOST
		if ( utbufp->ut_host[0] != '\0' )
			printf(" (%s)", utbufp->ut_host);
	#endif	

	printf("\n");

}
/*
This function converts the time given by utbufp->ut_time into the same format as who using the strftime pre-built function.
I do so using a struct to hold the local time of timeval
building a buffer to hold the new version that is converted from the strftime function
we then print out the final version of time we want which matches who
*/
void showtime(long timeval){
	struct tm *ptm = localtime(&timeval); /* struct that holds the local time of timeval*/
	char buff[80]; /* buffer that hold the converted time*/
	strftime(buff, 80, "%Y-%m-%d %H:%M", ptm); /* converts the local time held in the pointer inside of struct tm to our new format*/
	printf("%s", buff); /* print out the contents of buff */
}
