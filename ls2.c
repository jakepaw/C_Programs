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
#include <sys/ioctl.h>

#define MAX 64
bool a, r, s = false;
bool d = false;
int max_size = 0;
struct dirent *directory[MAX];
int file = 0;

void do_ls( char []);
void do_swap(int);
void show(int);

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

	if ( ( dir_ptr = opendir( dirname ) ) == NULL );// opens if not NULL
		//fprintf(stderr, "ls1: cannot open %s\n", dirname ); // error out if cannot access
	else
	{	
		while ( (direntp = readdir( dir_ptr) ) != NULL){ // reads contents if not NULL
			if (direntp->d_name[0] != 46){
			directory[files] = direntp; // prints out the name inside of the struct direntp (name of directory)
			files++;
			if(strlen(direntp->d_name) > max_size)
				max_size = strlen(direntp->d_name);
		}
		}
	if (s || r)
		do_swap(files); 
	else if (a || !a) // not deleting bc you never know what will happen lol
		show(files);
    }
		closedir(dir_ptr); // close the directory
	
}

void do_swap(int files){ //if the arg is s or r then this will happen (bubble)
	int j, b;
	struct dirent *temp;
	for (j = 0; j < files; j++)
			for (b = j+1; b < files; b++){
				int backorforth = strcmp(directory[j]->d_name,directory[b]->d_name); // getting the decider of s or r
				if (backorforth > 0 && s || backorforth < 0 && r) //comparing
				{
					temp = directory[j];         // swapping for the next 3 lines
					directory[j] = directory[b];
					directory[b] = temp;
				}
			}
	show(files); // print if you get here
}

void show(int files ){ // printing out the directory contents
	//printf("made it\n");
	//printf("%s\n", directory[files]->d_name);
	int max_cols = 0;
	struct winsize wbuf;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &wbuf) != -1){
		//rows = wbuf.ws_row;
		max_cols = wbuf.ws_col;
		//printf("%d rows and %d cols \n", rows, cols );
	}
	int width = max_cols / (max_size +2);
	int height = files / width;
	if(files % width > 0)
            height++;

struct dirent *orderedDirectory[height][width]; // creating a struct with the height and width of the window
//below makes everything NULL before filling because it would not work without it lol
    int q, w, fileCounter = 0;
        for(q = 0; q < width; q++){
            for(w = 0; w < height; w++){
                if(file < files)
                    orderedDirectory[w][q] = NULL;
            }
        }
//filling the ordered struct with files, which puts them into the correct place
    int v, k, file = 0;
    for(v = 0; v < width; v++){
            for(k = 0; k < height; k++){
                if(file < files) //&& directory[file][0]->dname != '.')
                    orderedDirectory[k][v] = directory[file++];
        }
    }
    file = 0;
    int diff = 0;
	for (int p = 0; p < height; p++){
		for(int o = 0; o < width; o++){
			int num_space = 0;
			if(file < files && orderedDirectory[p][o] != NULL){
				if (a && *orderedDirectory[p][o]->d_name == '.') // if a and starts with '.'
				printf("%s", orderedDirectory[p][o]->d_name);
					else if (*orderedDirectory[p][o]->d_name != '.'){ // if not a and not '.'
						printf("%s", orderedDirectory[p][o]->d_name);
						if(strlen(orderedDirectory[p][o]->d_name) == max_size){ //prints s single space for if == to max_size
							printf(" ");
						}
						else{
							diff = max_size - strlen(orderedDirectory[p][o]->d_name); //print a blank to match until next column
							for (int l = 0; l < diff+1; l++)
								printf(" ");
							}	
				}
			}
			file++;
		}
		printf("\n");
	}

}
