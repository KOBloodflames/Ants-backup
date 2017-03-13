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
		world = send_orders(world);
		printf("go\n");
		fprintf(stderr, "Turn %d , end = %s \n" , world.turn , world.end ? "true" : "false" ) ;
		fflush(stdout);
		fprintf(stderr, "\n");
		print_map(world);
	}
	print_map(world);
	world = cleanup_map(world);
}

/*void DFS(vertex i)
{
    visited[i]=1;
    for each w adjacent to i
        if(!visited[w])
            DFS(w);
}*/

worldmap find_route(worldmap w, int Y, int X, int Yn, int Xn, int ant)
{
	int i = Y;
	int j = X;
	w.ants[ant].targetx = Xn;
	w.ants[ant].targety = Yn;
	fprintf(stderr,"searching for a way to food on (%d,%d)",Xn, Yn);
	if(Yn > Y){
		if(w.map[i+1][j].type != CELL_WATER){
			printf("o %d %d S\n", i, j);
			fprintf(stderr,"order: o %d %d S\n",i, j);
			w = update_ants(w,i,j,i+1,j, ant);
			w.ants[ant].moving = 1;
			return w;
		}
		else if(Xn > X){
			if(w.map[i][j+1].type != CELL_WATER){
				printf("o %d %d E\n", i, j);
				fprintf(stderr,"order: o %d %d E\n",i, j);
				w = update_ants(w,i,j,i,j+1, ant);
				w.ants[ant].moving = 1;
				return w;
			}
		}else if(Xn < X){
			if(w.map[i][j-1].type != CELL_WATER){
				printf("o %d %d W\n", i, j);
				fprintf(stderr,"order: o %d %d W\n",i, j);
				w = update_ants(w,i,j,i,j-1, ant);
				w.ants[ant].moving = 1;
				return w;
			}
		}else{
			return w;
		}
	}else if(Yn < Y){
		if(w.map[i-1][j].type != CELL_WATER){
			printf("o %d %d N\n", i, j);
			fprintf(stderr,"order: o %d %d N\n",i, j);
			w = update_ants(w,i,j,i-1,j, ant);
			w.ants[ant].moving = 1;
			return w;
		}
		else if(Xn > X){
			if(w.map[i][j+1].type != CELL_WATER){
				printf("o %d %d E\n", i, j);
				fprintf(stderr,"order: o %d %d E\n",i, j);
				w = update_ants(w,i,j,i,j+1, ant);
				w.ants[ant].moving = 1;
				return w;
			}
		}else if(Xn < X){
			if(w.map[i][j-1].type != CELL_WATER){
				printf("o %d %d W\n", i, j);
				fprintf(stderr,"order: o %d %d W\n",i, j);
				w = update_ants(w,i,j,i,j-1, ant);
				w.ants[ant].moving = 1;
				return w;
			}
		}else{
			return w;
		}

	}
	else if(Xn > X){
		if(w.map[i][j+1].type != CELL_WATER){
			printf("o %d %d E\n", i, j);
			fprintf(stderr,"order: o %d %d E\n",i, j);
			w = update_ants(w,i,j,i,j+1, ant);
			w.ants[ant].moving = 1;
			return w;
		}
	}else if(Xn < X){
		if(w.map[i][j-1].type != CELL_WATER){
			printf("o %d %d W\n", i, j);
			fprintf(stderr,"order: o %d %d W\n",i, j);
			w = update_ants(w,i,j,i,j-1, ant);
			w.ants[ant].moving = 1;
			return w;
		}
	}else{
		return w;
	}	
}


worldmap send_orders(worldmap w){
			//fprintf(stderr, "checking row %d and col %d with type %d and owner %d\n", i,j,w.map[i][j].type,w.map[i][j].owner);
	for(int z = 0; z < w.antnr; z++){
		if(w.ants[z].alive == 1){
			if(w.ants[z].moving == 0){
				int i = w.ants[z].y;
				int j = w.ants[z].x;					
				int route_found = 0;
				for(int a = -2; a <= 2; a++){
					for(int b = -2; b<=2; b++){
						if(w.map[i+a][j+b].type ==  CELL_FOOD || (w.map[i+a][j+b].type ==  CELL_HILL && w.map[i+a][j+b].owner !=  0)){
							w = find_route(w,i,j,i+a,j+a, z);
							if(w.ants[z].moving == 1){break;}	
						}
					}
					if(w.ants[z].moving == 1){break;}	
				}
				if(w.ants[z].moving == 0){
					if(w.map[i][(j-1)%w.cols].type != CELL_WATER){ 
						printf("o %d %d W\n", i, j);
						fprintf(stderr,"order: o %d %d W\n",i, j);
						w = update_ants(w,i,j,i,j-1, z);
					
					}				
					else if(w.map[(i-1)%w.rows][j].type != CELL_WATER){ 
						printf("o %d %d N\n", i, j);
						fprintf(stderr,"order: o %d %d N\n", i, j);
						w = update_ants(w,i,j,i-1,j,z);
					}
					else if(w.map[i][(j+1)%w.cols].type != CELL_WATER){ 
						printf("o %d %d E\n", i, j);
						fprintf(stderr,"order: o %d %d E\n", i, j);
						w = update_ants(w,i,j,i,j+1,z);
					}
					else if(w.map[(i+1)%w.rows][j].type != CELL_WATER){ 
						printf("o %d %d S\n", i, j);
						fprintf(stderr,"order: o %d %d S\n", i, j);
						w = update_ants(w,i,j,i+1,j,z);
					
					}
				}
			}
			else{
				w = find_route(w,w.ants[z].y,w.ants[z].x,w.ants[z].targety, w.ants[z].targetx, z);
			}
		}
	}
	return w;	
}


