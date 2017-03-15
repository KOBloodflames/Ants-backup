#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		List_print(world.ants);
		printf("go\n");
		fprintf(stderr, "Turn %d , end = %s \n" , world.turn , world.end ? "true" : "false" ) ;
		fflush(stdout);
		fprintf(stderr, "\n");
		print_map(world);
	}
	print_map(world);
	world = cleanup_map(world);
}



worldmap find_route(worldmap w, int Y, int X, int Yn, int Xn, Node *current)
{
	int i = Y;
	int j = X;
	current->item->targetx = Xn;
	current->item->targety = Yn;
	fprintf(stderr,"searching for a way to (%d,%d)\n",Xn, Yn);
	if(Yn > Y){
		if(w.map[i+1][j].type != CELL_WATER && w.map[i+1][j].type != CELL_ANT){
			printf("o %d %d S\n", i, j);
			fprintf(stderr,"order: o %d %d S\n",i, j);
			current->item->lastmove = S;
			w = update_ants(w,i,j,i+1,j, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}
		else if(Xn > X){
			if(w.map[i][j+1].type != CELL_WATER && w.map[i][j+1].type != CELL_ANT){
				printf("o %d %d E\n", i, j);
				fprintf(stderr,"order: o %d %d E\n",i, j);
				current->item->lastmove = E;
				w = update_ants(w,i,j,i,j+1, current);
				current->item->moving = 1;
				current->item->water = 0;
				return w;
			}
		}else if(Xn < X){
			if(w.map[i][j-1].type != CELL_WATER && w.map[i][j-1].type != CELL_ANT){
				printf("o %d %d W\n", i, j);
				fprintf(stderr,"order: o %d %d W\n",i, j);
				current->item->lastmove = W;
				w = update_ants(w,i,j,i,j-1, current);
				current->item->moving = 1;
				current->item->water = 0;
				return w;
			}
		}else{
			fprintf(stderr,"path blocked!\n");
			current->item->moving = 0;
			return w;
		}
	}else if(Yn < Y){
		if(w.map[i-1][j].type != CELL_WATER&&w.map[i-1][j].type != CELL_ANT){
			printf("o %d %d N\n", i, j);
			fprintf(stderr,"order: o %d %d N\n",i, j);
			current->item->lastmove = N;
			w = update_ants(w,i,j,i-1,j, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}
		else if((Xn > X)&&(w.map[i][j+1].type != CELL_WATER&&w.map[i][j+1].type != CELL_ANT)){
			printf("o %d %d E\n", i, j);
			fprintf(stderr,"order: o %d %d E\n",i, j);
			current->item->lastmove = E;
			w = update_ants(w,i,j,i,j+1, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}else if((Xn < X)&&(w.map[i][j-1].type != CELL_WATER&&w.map[i][j-1].type != CELL_ANT)){
			printf("o %d %d W\n", i, j);
			fprintf(stderr,"order: o %d %d W\n",i, j);
			current->item->lastmove = W;
			w = update_ants(w,i,j,i,j-1, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}else{
			fprintf(stderr,"path blocked!\n");
			current->item->moving = 0;
			return w;
		}

	}
	else if(Xn > X){
		if(w.map[i][j+1].type != CELL_WATER&&w.map[i][j+1].type != CELL_ANT){
			printf("o %d %d E\n", i, j);
			fprintf(stderr,"order: o %d %d E\n",i, j);
			current->item->lastmove = E;
			w = update_ants(w,i,j,i,j+1, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}
	}else if(Xn < X){
		if(w.map[i][j-1].type != CELL_WATER&&w.map[i][j-1].type != CELL_ANT){
			printf("o %d %d W\n", i, j);
			fprintf(stderr,"order: o %d %d W\n",i, j);
			current->item->lastmove = W;
			w = update_ants(w,i,j,i,j-1, current);
			current->item->moving = 1;
			current->item->water = 0;
			return w;
		}
	}else{
		fprintf(stderr,"path blocked!\n");
		current->item->moving = 0;
		return w;
	}
	fprintf(stderr,"path blocked!\n");
	current->item->moving = 0;
	return w;	
}


worldmap send_orders(worldmap w){
			//fprintf(stderr, "checking row %d and col %d with type %d and owner %d\n", i,j,w.map[i][j].type,w.map[i][j].owner);
	Node *current = w.ants->head;	
	for(int z = 0; z < w.ants->counter; z++){
		fprintf(stderr, "Searching move for ant %d\n",current->item->id);	
		if(current->item->alive == 1){
			if(current->item->moving == 0){
				int i = current->item->y;
				int j = current->item->x;					
				int route_found = 0;
				current->item->route = search(w,i,j, current);
				current->item->routenr = 0;
				if(strcmp(current->item->route, "x") != 0){
					current->item->moving = 1;
					fprintf(stderr, "ant %d is now on his way!\n", current->item->id);
				}/*else{
					for(int a = 0; a <= 7; a++){
						for(int b = 0; b<=7; b++){
							if(w.map[mod(i+a,w.rows)][mod(j+b,w.cols)].type ==  CELL_FOOD || (w.map[mod(i+a,w.rows)][mod(j+b,w.cols)].type ==  CELL_HILL && w.map[mod(i+a,w.rows)][mod(j+b,w.cols)].owner !=  0)){
								w = find_route(w,i,j,mod(i+a,w.rows),mod(j+b,w.cols), current);
								if(current->item->moving == 1){break;}	
							}
						}
						if(current->item->moving == 1){break;}
					}	
				}*/
				/*if(w.ants[z].moving == 0){
					if(w.map[i][(j-1)%w.cols].type != CELL_WATER){ 
						printf("o %d %d W\n", i, j);
						fprintf(stderr,"order: o %d %d W\n",i, j);
						w = update_ants(w,i,j,i,j-1, z);
						w.ants[z].lastmove = W;
					
					}				
					else if(w.map[(i-1)%w.rows][j].type != CELL_WATER){ 
						printf("o %d %d N\n", i, j);
						fprintf(stderr,"order: o %d %d N\n", i, j);
						w = update_ants(w,i,j,i-1,j,z);
						w.ants[z].lastmove = N;
					}
					else if(w.map[i][(j+1)%w.cols].type != CELL_WATER){ 
						printf("o %d %d E\n", i, j);
						fprintf(stderr,"order: o %d %d E\n", i, j);
						w = update_ants(w,i,j,i,j+1,z);
						w.ants[z].lastmove = E;
					}
					else if(w.map[(i+1)%w.rows][j].type != CELL_WATER){ 
						printf("o %d %d S\n", i, j);
						fprintf(stderr,"order: o %d %d S\n", i, j);
						w = update_ants(w,i,j,i+1,j,z);
						w.ants[z].lastmove = S;
					
					}
				}*/
				if(current->item->moving == 0){
					if(w.map[i][mod((j-1),w.cols)].type == CELL_WATER && w.map[mod((i-1),w.rows)][j].type != CELL_ANT && w.map[mod((i-1),w.rows)][j].type != CELL_WATER){ 
						printf("o %d %d N\n", i, j);
						fprintf(stderr,"order: o %d %d N\n",i, j);
						current->item->lastmove = N;
						current->item->water = 1;
						w = update_ants(w,i,j,i-1,j, current);					
					}				
					else if(w.map[mod((i-1),w.rows)][j].type == CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT&&w.map[i][mod((j+1),w.cols)].type != CELL_WATER){ 
						printf("o %d %d E\n", i, j);
						fprintf(stderr,"order: o %d %d E\n", i, j);
						current->item->lastmove = E;
						current->item->water = 1;
						w = update_ants(w,i,j,i,j+1,current);
					}
					else if(w.map[i][mod((j+1),w.cols)].type == CELL_WATER&&w.map[mod((i+1),w.rows)][j].type != CELL_ANT&&w.map[mod((i+1),w.rows)][j].type != CELL_WATER){ 
						printf("o %d %d S\n", i, j);
						fprintf(stderr,"order: o %d %d S\n", i, j);
						current->item->lastmove = S;
						current->item->water = 1;
						w = update_ants(w,i,j,i+1,j,current);
					}
					else if(w.map[mod((i+1),w.rows)][j].type == CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT&&w.map[i][mod((j-1),w.cols)].type != CELL_WATER){ 
						printf("o %d %d W\n", i, j);
						fprintf(stderr,"order: o %d %d W\n", i, j);
						current->item->lastmove = W;
						current->item->water = 1;
						w = update_ants(w,i,j,i,j-1,current);
					
					}else if(current->item->water){
						current->item->water = 0;
						switch(current->item->lastmove){
							case N:
								if(w.map[i][mod((j-1),w.cols)].type == CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
									printf("o %d %d W\n", i, j);
									fprintf(stderr,"order: o %d %d W\n", i, j);
									current->item->lastmove = W;
									w = update_ants(w,i,j,i,j-1,current);	
								}
								else if(w.map[mod(i+1,w.rows)][j].type != CELL_WATER && w.map[mod(i+1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d S\n", i, j);
									fprintf(stderr,"order: o %d %d S\n", i, j);
									current->item->lastmove = S;
									w = update_ants(w,i,j,i+1,j,current);
								}			
								break;
							case E:
								if(w.map[mod(i-1,w.rows)][j].type != CELL_WATER&&w.map[mod(i-1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d N\n", i, j);
									fprintf(stderr,"order: o %d %d N\n",i, j);
									current->item->lastmove = N;
									w = update_ants(w,i,j,i-1,j, current);
								}else if(w.map[i][mod((j-1),w.cols)].type == CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
									printf("o %d %d W\n", i, j);
									fprintf(stderr,"order: o %d %d W\n", i, j);
									current->item->lastmove = W;
									w = update_ants(w,i,j,i,j-1,current);	
								}
								break;
							case S:
								if(w.map[i][mod(j+1,w.cols)].type != CELL_WATER&&w.map[i][mod(j+1,w.cols)].type != CELL_ANT){
									printf("o %d %d E\n", i, j);
									fprintf(stderr,"order: o %d %d E\n", i, j);
									current->item->lastmove = E;
									w = update_ants(w,i,j,i,j+1,current);
								}else if(w.map[mod(i-1,w.rows)][j].type != CELL_WATER&&w.map[mod(i-1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d N\n", i, j);
									fprintf(stderr,"order: o %d %d N\n",i, j);
									current->item->lastmove = N;
									w = update_ants(w,i,j,i-1,j, current);
								}
								break;
							case W:
								if(w.map[mod(i+1,w.rows)][j].type != CELL_WATER && w.map[mod(i+1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d S\n", i, j);
									fprintf(stderr,"order: o %d %d S\n", i, j);
									current->item->lastmove = S;
									w = update_ants(w,i,j,i+1,j,current);
								}else if(w.map[i][mod(j+1,w.cols)].type != CELL_WATER&&w.map[i][mod(j+1,w.cols)].type != CELL_ANT){
									printf("o %d %d E\n", i, j);
									fprintf(stderr,"order: o %d %d E\n", i, j);
									current->item->lastmove = E;
									w = update_ants(w,i,j,i,j+1,current);
								}
								break;
							default:
								if(w.map[mod(i+1,w.rows)][j].type != CELL_WATER && w.map[mod(i+1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d S\n", i, j);
									fprintf(stderr,"order: o %d %d S\n", i, j);
									current->item->lastmove = S;
									w = update_ants(w,i,j,i+1,j,current);
								}else if(w.map[mod(i-1,w.rows)][j].type != CELL_WATER&&w.map[mod(i-1,w.rows)][j].type != CELL_ANT){
									printf("o %d %d N\n", i, j);
									fprintf(stderr,"order: o %d %d N\n",i, j);
									current->item->lastmove = N;
									w = update_ants(w,i,j,i-1,j, current);
								}
								break;
						}
					}
					else{
						if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){ 
							printf("o %d %d W\n", i, j);
							fprintf(stderr,"order: o %d %d W\n",i, j);
							w = update_ants(w,i,j,i,j-1, current);
							current->item->lastmove = W;
					
						}				
						else if(w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){ 
							printf("o %d %d N\n", i, j);
							fprintf(stderr,"order: o %d %d N\n", i, j);
							w = update_ants(w,i,j,i-1,j,current);
							current->item->lastmove = N;
						}
						else if(w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){ 
							printf("o %d %d E\n", i, j);
							fprintf(stderr,"order: o %d %d E\n", i, j);
							w = update_ants(w,i,j,i,j+1,current);
							current->item->lastmove = E;
						}
						else if(w.map[mod((i+1),w.rows)][j].type != CELL_WATER&&w.map[mod((i+1),w.rows)][j].type != CELL_ANT){ 
							printf("o %d %d S\n", i, j);
							fprintf(stderr,"order: o %d %d S\n", i, j);
							w = update_ants(w,i,j,i+1,j,current);
							current->item->lastmove = S;
					
						}
					}
				}
			}
			else{
				//w = find_route(w,current->item->y,current->item->x,current->item->targety, current->item->targetx, current);
				fprintf(stderr, "Ant %d is still moving towards target!\n It will try to move in %c from (%d,%d)\n", current->item->id, current->item->route[current->item->routenr],current->item->x,current->item->y);
				int i = current->item->y;
				int j = current->item->x;
				if(i != current->item->targety || j != current->item->targetx){
					switch(current->item->route[current->item->routenr]){
						case 'N':
							if(w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){ 
								printf("o %d %d N\n", i, j);
								fprintf(stderr,"order: o %d %d N\n", i, j);
								w = update_ants(w,i,j,mod(i-1,w.rows),j,current);
								current->item->lastmove = N;
								current->item->routenr++;
							}
						break;
						case 'E':
							if(w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){ 
								printf("o %d %d E\n", i, j);
								fprintf(stderr,"order: o %d %d E\n", i, j);
								w = update_ants(w,i,j,i,mod(j+1,w.cols),current);
								current->item->lastmove = E;
								current->item->routenr++;
							}
						break;
						case 'S':
							if(w.map[mod((i+1),w.rows)][j].type != CELL_WATER&&w.map[mod((i+1),w.rows)][j].type != CELL_ANT){ 
								printf("o %d %d S\n", i, j);
								fprintf(stderr,"order: o %d %d S\n", i, j);
								w = update_ants(w,i,j,mod(i+1,w.rows),j,current);
								current->item->lastmove = S;
								current->item->routenr++;
					
							}
						break;
						case 'W':
							if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){ 
								printf("o %d %d W\n", i, j);
								fprintf(stderr,"order: o %d %d W\n",i, j);
								w = update_ants(w,i,j,i,mod(j-1,w.cols), current);
								current->item->lastmove = W;
								current->item->routenr++;
			
							}	
						break;
						default:
							fprintf(stderr,"invalid move\n");
						break;
					}
				}else{
					fprintf(stderr,"order finished!'(\n");
					current->item->moving = 0;
				}
			}
		current = current->next;
		fprintf(stderr, "the ant was alive and a move was found!\n the next ant to check will become: %d\n", current->item->id);
		}
		else{
			Node *next = current->next;
			fprintf(stderr, "The ant has been removed! the next ant to check will become: %d\n", current->item->id);
			world.map[current->item->y][current->item->x].type = CELL_DIRT;
			world.map[current->item->y][current->item->x].owner = -1;
			List_remove(world.ants, current);
			current = next;
		} //pls be the correct position <3
	}
	return w;	
}


