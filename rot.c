/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  rotation cipher executable 
 *
 *        Version:  0.0.1
 *        Created:  03/08/2015 09:54:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  johnnydiabetic 
 *   Organization:	N/A 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

// String concatenation
// Maybe i dont need to do this
char *stringcat(char *string, char *tail){
	int string_length, tail_length;
	size_t size, total_size;
	char *result;

	string_length = strlen(string);
	tail_length = strlen(tail);
	size = sizeof(char);
	
	result = malloc((string_length+tail_length)*sizeof(char));
	if (result == NULL) {
		printf("mem error!");
		return NULL; // returns NULL on error
	}

	int i;
	for (i=0;i<(tail_length+string_length); i++){
		if (i >= string_length){
			result[i] = tail[i-string_length];
		}else{
			result[i] = string[i];
		}
	}
	return result;	
}

int rot(char *filename, int rotation){
	// Setup
	FILE *fp, *ofp;
	int status;
	char *newFN = stringcat(filename, ".rot");	
	
	// open the files
	fp = fopen(filename, "r");
	if (fp == NULL){
		return -1;
	}
	ofp = fopen(newFN, "w");
	if (ofp==NULL){
		return -1;
	}

	// Rotate this shit
	char op;
	while (fscanf(fp, "%c", &op) != EOF){
		char r = (char)rotation;
		fprintf(ofp, "%c", op+=r);
	}

	// Close the file pointers
	status = fclose(fp);
	if (status != 0){
		printf("IO Error!\n");
		return status;
	}
	fclose(ofp);
	if (status != 0){
		printf("IO Error!\n");
		return status;
	}
}

int main(int argc, char *argv[]){
	int option;
	int rotation;
	char *fn; 
	char *buff;
	while ((option = getopt(argc, argv, "f:r:")) != -1){
		switch(option) {
			case 'f':
				fn = optarg;
				break;
			case 'r':
				buff = optarg;
				int len = strlen(buff);
				int sign = 1;
				
				// if negative
				if (buff[0]=='-'){
					int i;
					sign = -1;
					for (i=1; i<len; i++){
						buff[i-1] = buff[i];
					}
					buff[i-1] = '\0';
				}

				// Get the numeric part
				if (isdigit(*buff)){
					rotation = atoi(buff)*sign;
				} else{
					printf("-r flag takes a digit. Unrecognized digit.\n");
					printf("buff is: %s", buff);
					exit(1);
				}
				break;
		
			default:
				printf("Unrecognied command\n");
				exit(1);
				break;
		}
	}

	if (fn == NULL){
		printf("No file specified. Exiting...\n");
		exit(1);
	}	
	printf("filename: %s\trotation: %d\n", fn, rotation);	
	int status;
	status = rot(fn,rotation);	
	if (status == -1){
		exit(1);
	}	
	return 0;
}

