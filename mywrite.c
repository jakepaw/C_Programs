/*
Author: Jacob Pawlak
Date: Novemeber 8th 2018
Purpose: recreate the write command, allow user to talk to another user using username and pts
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <utmp.h>
// defining a checkUser function which checks if the username is correct
int checkUser(char name[], char tty[]); 

int main(int ac, char *av[]){
    
    if(ac != 3){
        printf("Please provide the correct number of args such as ->'./mywrite user tty'\n");
        return 1;
    }

    char ttystr[10];
    // if the pts arg starts with p then user provided pts most likely 
    if(av[2][0] == 'p'){
        strcpy(ttystr, "/dev/");
        strcat(ttystr, av[2]);
    }else{
        strcpy(ttystr, av[2]); // if not then you most likely got the full /dev/pts/____
    }

    if(checkUser(av[1], ttystr) == 0){
        printf("User * %s * does not exist on %s\n", av[1], ttystr);
        return 1;
    }

    //gets user information
    int fd;
    char username[BUFSIZ];
    char hostname[BUFSIZ];
    char *tty;
    // get username & host
    getlogin_r(username, sizeof(username));
    gethostname(hostname, sizeof(hostname));
    
    // get tty
    tty = ttyname(STDIN_FILENO);
    
    // get time
    time_t currentTime;
    time(&currentTime);
    struct tm *timeInfo = localtime(&currentTime);;
    char buff[80];
    
    strftime(buff, 80, "%H:%M", timeInfo);
    
    //writing the firt message and which shows user information, date and time
    char message[BUFSIZ];
    
    strcpy(message, "\nMessage from ");
    strcat(message, username);
    strcat(message, "@");
    strcat(message, hostname);
    strcat(message, " on ");
    strcat(message, tty);
    strcat(message, " at ");
    strcat(message, buff);
    strcat(message, " ... \n");
    
    char buf[BUFSIZ];
    
    
    // open connection (aka the file that is being written to (tty number) with write access)
    fd = open(ttystr, O_WRONLY);
    if(fd == -1){
        printf("Failed to connect to %s on %s.\n", av[1], ttystr); // error out if you cannot get the connection to the fd
        return 1;
    }
    
    // Send receiver connection information using write lol
    write(fd, message, strlen(message));

    while(fgets(buf, BUFSIZ, stdin) != NULL)
        write(fd, buf, strlen(buf));

    write(fd, "EOF\n", 4);
    // close connection on the Control + D
    close(fd);
    
    return 0;
}

int checkUser(char name[], char tty[]){
    int exists = 0; // kinda a boolean that will tell you if the user exists or they do not
	struct utmp current; // used to hold UTMP information
	int file = open(UTMP_FILE, O_RDONLY); // contains file location?
	//open check
	if(file  <= -1)
	{
		printf("Failed to open UTMP_FILE");
		return 1;
	}
    char condtty[1];
    condtty[0] = tty[10];
	int numBytes = sizeof(current);
	
	while((read(file, &current, sizeof(current))) == numBytes) {
		if(current.ut_line[0] != '~') { 
            if(strstr(current.ut_name, name) != 0) // comparing if the username given and the username in the utmp file exists
                exists = 1;
		}
	}
	close(file);    
    return exists; // if exists
}
