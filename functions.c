#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "0936196.h"

worldmap initialize_map(worldmap world){
	world.map = (cell **) malloc(world.rows * sizeof(cell*));
	for(int i = 0; i < world.rows; i++){
		world.map[i] = (cell *) malloc(world.cols * sizeof(cell));
	}
	if(world.map == NULL){
		printf("Error allocating memory");
	}
	return world;
}

worldmap cleanup_map(worldmap world){
	for(int i = 0; i < world.rows; i++){
		free(world.map[i]);
	}
	free(world.map);
	return world;
}


void print_map(worldmap w){
	char ant_letters[10] = {'a','b','c','d','e','f','g','h','i','j'};
	char ant_hill_letters[10] = {'A','B','C','D','E','F','G','H','I','J'};
	for(int i = 0; i < w.cols; i++){
	printf("m ");
		for(int j = 0; j < w.rows; j++){
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

worldmap reset_map(worldmap world){
	for(int i = 0; i < world.rows; i++){
		for(int j = 0; j < world.cols; j++){
			world.map[i][j].type = CELL_DIRT;
			world.map[i][j].owner = 0;
		}
	}
	return world;

}

void read_initialization(game_settings *s){
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
		//printf("end 2:%d\nend 1:%d\n", end2, end);
		key = (char *) malloc(sizeof(char)*end);
		for(int i = 0; i < end; i++){
			//printf("I can go through the first loop!");
			key[i] = line[i];
		}
		//printf("Linelenght: %zu, end: %d, end2: %d, string: %s, totalstring: %s.\n",linelength,end,end2,key,line);
		if(key != NULL){
			if(strcmp(key,"ready") == 0){
				free(key);
				break;	
			}
		}
		int multiplier = 1;
		value = 0;
		for(int i = end2 - 1; i > end ; i--){
			//printf("I can go through the second loop!");
			value += multiplier*(line[i]-'0'); //-'0'?
			multiplier *= 10;
			//printf("%d * %d = intermediate value: %d\n", line[i] - '0', multiplier, value);
		}
		if(strcmp(key, "turn") == 0){
			s->turn = value;
		}
		else if(strcmp(key, "loadtime") == 0){
			s->loadtime = value;
		}
		else if(strcmp(key, "turntime") == 0){
			s->turntime = value;
		}
		else if(strcmp(key, "rows") == 0){
			s->rows = value;
		}
		else if(strcmp(key, "cols") == 0){
			s->cols = value;
		}
		else if(strcmp(key, "turns") == 0){
			s->turns = value;
		}
		else if(strcmp(key, "viewradius2") == 0){
			s->viewradius2 = value;
		}
		else if(strcmp(key, "attackradius2") == 0){
			s->attackradius2 = value;
		}
		else if(strcmp(key, "spawnradius2") == 0){
			s->spawnradius2 = value;
		}
		else if(strcmp(key, "player_seed") == 0){
			s->player_seed = value;
		}
		//printf("key: %s\nvalue: %d\n", key, value);
		free(key);	
	}
	free(line);
}


worldmap read_turn(worldmap w){
	char *line = NULL;
	size_t linelength = 0;
	char *key = NULL;
	char *end = "end";
	char *delimiters = " \n";
	char *saveptr;
	w = reset_map(w);
	while(getline(&line, &linelength, stdin) != 0){	
		//printf("Linelenght: %zu, end: %d, end2: %d, string: %s, totalstring: %s.\n",linelength,end,end2,key,line);
		if(strcmp(line,"\n")!=0){
			long row = 0;
			long col = 0;
			long owner = 0;
			fprintf(stderr,"I HAVE BEEN HERE, text: ");
			key = (char *) malloc(sizeof(char)*linelength);
			key = strtok_r(line, delimiters, &saveptr);
			fprintf(stderr, "%s\n", key);
			fprintf(stderr, "compare: %d\n", strcmp(key,"turn"));
			if(key != NULL){
				if(strcmp(key,"end") == 0){
					w.end = 1;	
				}
		
				if(strcmp(key, "go") == 0){
					w.end = 0;
					break;
				}
				else if(strcmp(key, "turn") == 0){
					w.turn = strtol(strtok(line, delimiters), &key, 10);	
					fprintf(stderr, "saving a turn %d", w.turn);			
				}
				else if(strcmp(key, "w") == 0){
					row = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					col = strtol(saveptr,&key,10);
					fprintf(stderr, "saving a water on row %ld and col %ld\n", row,col);
					w.map[row][col].type = CELL_WATER;
				}
				else if(strcmp(key, "f") == 0){
					row = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					col = strtol(saveptr,&key,10);
					fprintf(stderr, "saving a food on row %ld and col %ld\n", row,col);
					w.map[row][col].type = CELL_FOOD;
				}
				else if(strcmp(key, "h") == 0){
					row = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					col = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					owner = strtol(saveptr,&key,10);
					fprintf(stderr, "saving a hill on row %ld and col %ld\n", row,col);
					w.map[row][col].type = CELL_HILL;
					w.map[row][col].owner = (int)owner;
				}
				else if(strcmp(key, "a") == 0){
					/*row = strtol(strtok(line, delimiters),&key, 10);
					col = strtol(strtok(line, delimiters),&key,10);
					owner = strtol(strtok(line, delimiters),&key,10);*/
					row = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					col = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					owner = strtol(saveptr,&key,10);
					fprintf(stderr, "saving an ant on row %ld and col %ld\n", row,col);
					if(w.map[row][col].type == CELL_HILL){
						w.map[row][col].type = CELL_ANT_ON_HILL;
					}else{
						w.map[row][col].type = CELL_ANT;
					}
					w.map[row][col].owner = (int)owner;
			
				}
			}
		}	
	}
	free(line);
	return w;
}
