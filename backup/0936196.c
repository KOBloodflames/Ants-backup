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
		world = assign_squads(world);
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
	int move = -1;
	Node *current = w.ants->head;	
	for(int z = 0; z < w.ants->counter; z++){
		fprintf(stderr, "Searching move for ant %d\n",current->item->id);	
		if(current->item->alive == 1){
			int i = current->item->y;
			int j = current->item->x;
			if(current->item->moving == 0){
				if(current->item->role == GATHERER){
					fprintf(stderr, "ant %d is a gatherer!\n", current->item->id);				
					int route_found = 0;
					current->item->route = search(w,i,j, current);
					current->item->routenr = 0;
					if(strcmp(current->item->route, "x") != 0){
						current->item->moving = 1;
						fprintf(stderr, "ant %d is now on his way!\n", current->item->id);
					}
					if(current->item->moving == 0){
						int friends = 0;
						int enemies = 0;
						for(int a = -5; a <=5 ; a++){
							for(int b = -5; b <= 5; b++){
								if(w.map[mod(i+a,w.rows)][mod(j+b,w.cols)].type == CELL_ANT){
									if(w.map[mod(i+a,w.rows)][mod(j+b,w.cols)].owner == 0){
										friends++;
									}else{
										enemies++;
									}
								}
							}
						}if(enemies > friends || friends < 10){
							if(move!= 0){
								move = 0;
								world = check_moves(world,0);
							}
						}else{
							if(move!= 4){
								move = 4;
								world = check_moves(world,4);
							}
						}
						if(w.map[mod(i+1,w.rows)][j].move > w.map[mod(i-1,w.rows)][j].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j+1,w.cols)].move && w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_WATER&&w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_ANT){
							printf("o %d %d S\n", i, j);
							fprintf(stderr,"order: o %d %d S\n", i, j);
							w = update_ants(w,i,j,mod(i+1,w.rows),j,current);
							current->item->lastmove = S;			
						}else if(w.map[i][mod(j+1,w.cols)].move > w.map[mod(i-1,w.rows)][j].move && w.map[i][mod(j+1,w.cols)].move > w.map[i][mod(j-1,w.cols)].move && w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){
							printf("o %d %d E\n", i, j);
							fprintf(stderr,"order: o %d %d E\n", i, j);
							w = update_ants(w,i,j,i,mod(j+1,w.cols),current);
							current->item->lastmove = E;
						}else if(w.map[mod(i-1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){
							printf("o %d %d N\n", i, j);
							fprintf(stderr,"order: o %d %d N\n", i, j);
							w = update_ants(w,i,j,mod(i-1,w.rows),j,current);
							current->item->lastmove = N;
						}else if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
							printf("o %d %d W\n", i, j);
							fprintf(stderr,"order: o %d %d W\n",i, j);
							w = update_ants(w,i,j,i,mod(j-1,w.cols), current);
							current->item->lastmove = W;
						}else{
							fprintf(stderr,"no good move\n");
						}
					}
					
				}else if(current->item->role == SOLDIER && (w.ants->squadmembers[0][0] == current->item->id)||(w.ants->squadmembers[1][0] == current->item->id)||(w.ants->squadmembers[2][0] == current->item->id)){
					fprintf(stderr, "ant %d is a soldier leader!\n", current->item->id);						
					int squad = 0;
					if(w.ants->squadmembers[0][0] == current->item->id){
						squad = 0;
					}else if(w.ants->squadmembers[1][0] == current->item->id){
						squad = 1;
					}else{
						squad = 2;
					}
					int x = current->item->x;
					int y = current->item->y;
					int squadready = 1;
					if(w.ants->squadsize[current->item->squad] > 4){
						int check = 6;
						for(int a = -1; a <= 1; a++){
							for(int b = -1; b <=1;b++){
								if(w.map[y+a][x+b].type != CELL_ANT){
								check--;
									if(check == 0){
										squadready = 0;
										break;
									}
								}
							}
						}
					}else{
						squadready = 0;
					}
					if(squadready){
						if(move!=1){
							move = 1;
							world = check_moves(world,1);
							for(int x = 0; x < w.ants->squadsize[squad]; x++){
								Node *current = List_find(w.ants,w.ants->squadmembers[squad][x]);
								fprintf(stderr, "ant %d is a soldier!\n", current->item->id);	
								if(w.map[mod(i+1,w.rows)][j].move > w.map[mod(i-1,w.rows)][j].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j+1,w.cols)].move && w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_WATER&&w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_ANT){
									printf("o %d %d S\n", i, j);
									fprintf(stderr,"order: o %d %d S\n", i, j);
									w = update_ants(w,i,j,mod(i+1,w.rows),j,current);
									current->item->lastmove = S;			
								}else if(w.map[i][mod(j+1,w.cols)].move > w.map[mod(i-1,w.rows)][j].move && w.map[i][mod(j+1,w.cols)].move > w.map[i][mod(j-1,w.cols)].move && w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){
									printf("o %d %d E\n", i, j);
									fprintf(stderr,"order: o %d %d E\n", i, j);
									w = update_ants(w,i,j,i,mod(j+1,w.cols),current);
									current->item->lastmove = E;
								}else if(w.map[mod(i-1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){
									printf("o %d %d N\n", i, j);
									fprintf(stderr,"order: o %d %d N\n", i, j);
									w = update_ants(w,i,j,mod(i-1,w.rows),j,current);
									current->item->lastmove = N;
								}else if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
									printf("o %d %d W\n", i, j);
									fprintf(stderr,"order: o %d %d W\n",i, j);
									w = update_ants(w,i,j,i,mod(j-1,w.cols), current);
									current->item->lastmove = W;
								}else{
									fprintf(stderr,"no good move\n");
								}
							}
						}
					}else{
						if(move != 2){
							move = 2;
							world = check_moves(world,2);
							for(int x = 0; x < w.ants->squadsize[squad]; x++){
								Node *current = List_find(w.ants,w.ants->squadmembers[squad][x]);
								fprintf(stderr, "ant %d is a soldier who wants to find his squad!\n", current->item->id);	
								if(w.map[mod(i+1,w.rows)][j].move > w.map[mod(i-1,w.rows)][j].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j+1,w.cols)].move && w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_WATER&&w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_ANT){
									printf("o %d %d S\n", i, j);
									fprintf(stderr,"order: o %d %d S\n", i, j);
									w = update_ants(w,i,j,mod(i+1,w.rows),j,current);
									current->item->lastmove = S;			
								}else if(w.map[i][mod(j+1,w.cols)].move > w.map[mod(i-1,w.rows)][j].move && w.map[i][mod(j+1,w.cols)].move > w.map[i][mod(j-1,w.cols)].move && w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){
									printf("o %d %d E\n", i, j);
									fprintf(stderr,"order: o %d %d E\n", i, j);
									w = update_ants(w,i,j,i,mod(j+1,w.cols),current);
									current->item->lastmove = E;
								}else if(w.map[mod(i-1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){
									printf("o %d %d N\n", i, j);
									fprintf(stderr,"order: o %d %d N\n", i, j);
									w = update_ants(w,i,j,mod(i-1,w.rows),j,current);
									current->item->lastmove = N;
								}else if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
									printf("o %d %d W\n", i, j);
									fprintf(stderr,"order: o %d %d W\n",i, j);
									w = update_ants(w,i,j,i,mod(j-1,w.cols), current);
									current->item->lastmove = W;
								}else{
									fprintf(stderr,"no good move\n");
								}
							}
						}
					}
					
				}else{
					fprintf(stderr, "ant %d is a defender!\n", current->item->id);	
					if(move!= 3){
						move = 3;
						world = check_moves(world,3);
					}
					if(w.map[mod(i+1,w.rows)][j].move > w.map[mod(i-1,w.rows)][j].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod(i+1,w.rows)][j].move > w.map[i][mod(j+1,w.cols)].move && w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_WATER&&w.map[mod((mod(i+1,w.rows)),w.rows)][j].type != CELL_ANT){
						printf("o %d %d S\n", i, j);
						fprintf(stderr,"order: o %d %d S\n", i, j);
						w = update_ants(w,i,j,mod(i+1,w.rows),j,current);
						current->item->lastmove = S;			
					}else if(w.map[i][mod(j+1,w.cols)].move > w.map[mod(i-1,w.rows)][j].move && w.map[i][mod(j+1,w.cols)].move > w.map[i][mod(j-1,w.cols)].move && w.map[i][mod((j+1),w.cols)].type != CELL_WATER&&w.map[i][mod((j+1),w.cols)].type != CELL_ANT){
						printf("o %d %d E\n", i, j);
						fprintf(stderr,"order: o %d %d E\n", i, j);
						w = update_ants(w,i,j,i,mod(j+1,w.cols),current);
						current->item->lastmove = E;
					}else if(w.map[mod(i-1,w.rows)][j].move > w.map[i][mod(j-1,w.cols)].move && w.map[mod((i-1),w.rows)][j].type != CELL_WATER&&w.map[mod((i-1),w.rows)][j].type != CELL_ANT){
						printf("o %d %d N\n", i, j);
						fprintf(stderr,"order: o %d %d N\n", i, j);
						w = update_ants(w,i,j,mod(i-1,w.rows),j,current);
						current->item->lastmove = N;
					}else if(w.map[i][mod((j-1),w.cols)].type != CELL_WATER&&w.map[i][mod((j-1),w.cols)].type != CELL_ANT){
						printf("o %d %d W\n", i, j);
						fprintf(stderr,"order: o %d %d W\n",i, j);
						w = update_ants(w,i,j,i,mod(j-1,w.cols), current);
						current->item->lastmove = W;
					}else{
						fprintf(stderr,"no good move\n");
					}			
				}
			}
			else{
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


