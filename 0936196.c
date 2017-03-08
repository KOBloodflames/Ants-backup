#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "0936196.h"

worldmap world;

void main(void)
{
	world.map = NULL;
	world.rows = 20;
	world.cols = 20;
	initialize_map(world);
	print_map(world);
	read_initialization();
	cleanup_map(world);
}




void initialize_map(){
	world.map = (cell **) malloc(world.rows * sizeof(cell*));
	for(int i = 0; i < world.rows; i++){
		world.map[i] = (cell *) malloc(world.cols * sizeof(cell));
	}
	if(world.map == NULL){
		printf("Error allocating memory");
	}else{
		for(int i = 0; i < world.rows; i++){
			for(int j = 0; j < world.cols; j++){
				world.map[i][j].type = CELL_DIRT;
				world.map[i][j].owner = 0;
			}
		}
	}
}

void cleanup_map(){
	for(int i = 0; i < world.rows; i++){
		free(world.map[i]);
	}
	free(world.map);
}
