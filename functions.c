#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "0936196.h"


void print_map(worldmap w){
	char ant_letters[10] = {'a','b','c','d','e','f','g','h','i','j'};
	char ant_hill_letters[10] = {'A','B','C','D','E','F','G','H','I','J'};
	for(int i = 0; i < w.rows; i++){
	printf("m ");
		for(int j = 0; j < w.cols; j++){
			switch(w.map[i][j].type){
			case CELL_DIRT:
				printf(".");
				break;
			case CELL_WATER:
				printf("%%");
				break;
			case CELL_ANT:
				printf("%c",ant_letters[w.map[i][j].owner]);
				break;
			case CELL_FOOD:
				printf("*");
				break;
			case CELL_HILL:
				printf("%d",w.map[i][j].owner);
				break;
			case CELL_ANT_ON_HILL:
				printf("%c",ant_hill_letters[w.map[i][j].owner]);
				break;
																					
			}
		}
	printf("\n");
	}
}

void read_initialization(){
	char *line = NULL;
	size_t linelength = 0;
	char *key = NULL;
	int value = 0;
	while(getline(&line, &linelength, stdin) != 0){	
		int first = 1;
		int end = 0; 
		int end2 = 0;
		int start = 0;
		for(int i = 0; i < linelength; i++){ 		
			if(line[i] == ' ' || line[i] == '\0' || line[i] == '\n'){
				if(first == 1){
					end = i;
					start  = i+1;
					first = 0;
				}else{
					end2 = i;
					break;
				}
			}else if(i == (linelength-1)){
				if(end == 0){
					end = i+1;
					end2 = end;
				}else{
					end2 = i+1;
				}
			}	
		}
		if (end2-end == 1){
			end2 = end;
		}
		printf("end 2:%d\nend 1:%d\n", end2, end);
		key = (char *) malloc(sizeof(char)*end);
		for(int i = 0; i < end; i++){
			//printf("I can go through the first loop!");
			key[i] = line[i];
		}
		//printf("Linelenght: %zu, end: %d, end2: %d, string: %s, totalstring: %s.\n",linelength,end,end2,key,line);
		if(strcmp(key,"ready") == 0){
			free(key);
			break;	
		}
		int multiplier = 1;
		value = 0;
		for(int i = end2 - 2; i > end ; i--){
			//printf("I can go through the second loop!");
			value += multiplier*(line[i]-'0'); //-'0'?
			multiplier *= 10;
			//printf("%d * %d = intermediate value: %d\n", line[i] - '0', multiplier, value);
		}
	printf("key: %s\nvalue: %d\n", key, value);
	free(key);	
	}
	free(line);
}
