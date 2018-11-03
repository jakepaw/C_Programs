/*
This program implements the cat command and allows for 4 arguments to be passed for different functions
-b
-e
-n
-s
then does the appropriate function based off the arguments.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void oops(char *, char *);

int main(int argc, char *argv[]){
    
    bool b,e,n,s = false; /* bool type to use for switch (could use 1's and 0's but boolean is good)*/
    char opt;
    int lineNumber = 1;
    int currentFile = 1;

    while((opt=getopt(argc, argv, "bens"))!=-1){
	     if(opt)
	        currentFile = 2;
	    switch(opt){
                case 'b': 
		    b = true;
		   // printf("made it to b"); debugging 
                    break;
                case 'e': 
		    //printf("made it to e");
		    e = true;
                    break;
                case 'n': 
		    //printf("made it to n");
		    n = true;
                    break;
                case 's': 
		    //printf("made it to s");
		    s = true;
                    break;
                default: 
                exit(1); // 
            }
        }
 
    for(currentFile; currentFile < argc; currentFile++){  // accepts a file at a time, until we get to argc 
        FILE *file;
        char *current;
        //char current[buffer]; this is not th corrent way to make the buffer
        long fsize;
        char previous = '\n'; //setting initial character for previous

        if ((file = fopen(argv[currentFile], "r")) == NULL) // open with read permissions onlu
            oops("Cannot open ", argv[1]);
   	
        
        fseek(file, 0, SEEK_END); // the next 4 lines are how we will grab the buffer size from the file and then make the correct size
        fsize = ftell(file);
	    fseek(file, 0, SEEK_SET);
        current = (char*)malloc(fsize+1);
    
        while(fread(current, sizeof(char), 1, file)){
            
            if(b && previous == '\n' && *current != previous){ // handles the b case
                printf("\t%d ", lineNumber);
                lineNumber++;
                }
                else if(n && previous == '\n'){ // handles the n case
                printf("\t%d ", lineNumber);
                lineNumber++;
                }
            
                    if(e && *current == '\n') // handles the e case
                    printf("$");
                
                        if(!s)
                        printf("%c", *current);
                        else if(s && previous == '\n' && *current == previous){ // handles the s case
                        }
            else
                printf("%c", *current); // if no opts, just cat the file to output 
                previous = *current; // previous if now the current character
            }
            printf("\n"); // new line to seperate files
        // close our file (s)
        fclose(file); 
    }   
    return 0;

}

void oops(char *s1, char *s2) // outputing an error 
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}
