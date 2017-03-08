#include <stdlib.h>
#include <stdio.h>
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
