/*
list content of directory or directories, if not args use .
else list files in args
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX 64
bool a, r, s = false;
bool d = false;

void do_ls( char []);
void do_swap( char [][MAX], int);
void show( char[][MAX], int);


int main(int argc, char *argv[]){
	if(argc > 1){
		if(argv[1][0] == '-'){
			for(int p = 1; p < strlen(argv[1]); p++){
	         	switch(argv[1][p]){
	        	case 'a': 
			    a = true;
			   // printf("made it to a"); debugging 
	            break;
	            case 'r': 
			    //printf("made it to r");
			    r = true;
	            break;
	            case 's': 
			    //printf("made it to s");
			    s = true;
	            break;
	            //default: 
	            //exit(1); // 
	            }
	        }
	    }
   			else
	        d=true;
	}
	if ( argc <= 2 && !d)
		do_ls( ".");
	else{
		while (--argc){
				if (argc == 2 && d){
					do_ls( *++argv );
				}
				else{
					do_ls( *++argv );
				}
		}
	}
}		
void do_ls( char dirname[]){
	DIR *dir_ptr; //the dir.... with the DIR type and point to the directory
	struct dirent *direntp; // each entry.... this 
	int files = 0;
	int i, j, b, z, x;
	char directory[MAX][MAX];
	char temp[MAX];

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )// opens if not NULL
		fprintf(stderr, "ls1: cannot open %s\n", dirname ); // error out if cannot access
	else
	{	
		while ( (direntp = readdir( dir_ptr) ) != NULL){ // reads contents if not NULL
			strcpy(directory[files], direntp->d_name ); // prints out the name inside of the struct direntp (name of directory)
			files++;
		}
	if (s || r)
		do_swap(directory, files);
	else if (a || !a)
		show(directory, files);
    }

		closedir(dir_ptr); // close the directory
	
}

void do_swap( char directory[MAX][MAX], int files){ //if the opt is s or r then this will happen
	int j, b;
	char temp[MAX];
	for (j = 0; j < files; j++)
			for (b = j+1; b < files; b++){
				int backorforth = strcmp(&directory[j][0], &directory[b][0]); // getting the decider of s or r
				if (backorforth > 0 && s || backorforth < 0 && r) //comparing
				{
					strcpy(temp, directory[j]);         // swapping for the next 3 lines
					strcpy(directory[j], directory[b]);
					strcpy(directory[b], temp);
				}
			}
	show(directory, files); // print if you get here
}

void show(char directory[MAX][MAX], int files ){ // printing out the directory contents
	for(int i = 0; i < files; i++){
    	if(a && directory[i][0] == '.' )
    	printf("%s\n", directory[i]);
    		else if (directory[i][0] != '.' ){
    			printf("%s\n", directory[i]);
    		}
	}
}
