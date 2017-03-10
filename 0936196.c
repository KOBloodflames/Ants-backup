#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "0936196.h"

worldmap world;
game_settings settings;

void main(void)
{
	fprintf(stderr,"Initializing world!\n");
	read_initialization(&settings);
	world.map = NULL;
	world.rows = settings.rows;
	world.cols = settings.cols;
	world.end = 0;
	world.turn = 0;
	fprintf(stderr,"Initializing map!\n");
	world = initialize_map(world);
	printf("go\n");
	fflush(stdout);
	fprintf(stderr, "Processed initialion\n");
	while(!world.end){
		world = read_turn(world);
		//print_map(world);
		send_orders(world);
		printf("go\n");
		fprintf(stderr, "Turn %d , end = %s \n" , world.turn , world.end ? "true" : "false" ) ;
		fflush(stdout);
		fprintf(stderr, "\n");
		print_map(world);
	}
	print_map(world);
	world = cleanup_map(world);
}


void send_orders(worldmap w){
	for(int i = 0; i < w.rows; i++){
		for(int j = 0; j < w.cols; j++){
			//fprintf(stderr, "checking row %d and col %d with type %d and owner %d\n", i,j,w.map[i][j].type,w.map[i][j].owner);
			if((w.map[i][j].type == CELL_ANT||w.map[i][j].type == CELL_ANT_ON_HILL )&& w.map[i][j].owner == 0){
				fprintf(stderr, "the ant on row %d and col %d is mine!\n", j,i);
				/*if(w.map[i][(j-1)%w.cols].type == CELL_DIRT || w.map[i][(j-1)%w.cols].type == CELL_FOOD || w.map[i][(j-1)%w.cols].type == CELL_HILL){ 
					printf("o %d %d W\n", i, j);
					fprintf(stderr,"order: o %d %d N\n",i, j);
				}				
				else if(w.map[(i-1)%w.rows][j].type == CELL_DIRT || w.map[(i-1)%w.rows][j].type == CELL_FOOD || w.map[(i-1)%w.rows][j].type == CELL_HILL){ 
					printf("o %d %d N\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}
				else if(w.map[i][(j+1)%w.cols].type == CELL_DIRT || w.map[i][(j+1)%w.cols].type == CELL_FOOD || w.map[i][(j+1)%w.cols].type == CELL_HILL){ 
					printf("o %d %d E\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}
				else if(w.map[(i+1)%w.rows][j].type == CELL_DIRT || w.map[(i+1)%w.rows][j].type == CELL_FOOD || w.map[(i+1)%w.rows][j].type == CELL_HILL){ 
					printf("o %d %d S\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}*/
				if(w.map[i][(j-1)%w.cols].type != CELL_WATER){ 
					printf("o %d %d W\n", i, j);
					fprintf(stderr,"order: o %d %d N\n",i, j);
				}				
				else if(w.map[(i-1)%w.rows][j].type != CELL_WATER){ 
					printf("o %d %d N\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}
				else if(w.map[i][(j+1)%w.cols].type != CELL_WATER){ 
					printf("o %d %d E\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}
				else if(w.map[(i+1)%w.rows][j].type != CELL_WATER){ 
					printf("o %d %d S\n", i, j);
					fprintf(stderr,"order: o %d %d N\n", i, j);
				}
			}
		}
	}	

}


