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
	}else{
		world.ants = List_create();
		world.ants->counter = 0;
		world.ants->idcounter = 0;
		world = reset_map(world);
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

char *search(worldmap w, int Y, int X, int Yn, int Xn, Node *current){

	Xn = mod(Xn,w.cols);
	Yn = mod(Yn,w.rows);
	current->item->targetx = Xn;
	current->item->targety = Yn;
	fprintf(stderr,"searching for a way to (%d,%d) using BFS\n",Xn, Yn);
	ant *queue = List_create();
	List_insert(queue);
	List_print(queue);
	queue->head->bfs->n = 0;
	queue->head->bfs->x = X;
	queue->head->bfs->y = Y;
	w.map[Y][X].bfs = 0;
	int finished = 0;
	int noroute = 0;
	int longest = 999;
	//FOR MAP, VISITED = 0
	int maxdepth = 7;
	while(!finished){
		if(queue->counter == 0){
			finished = 1;
			noroute = 1;
		}
		else if(queue->head->bfs->x == Xn && queue->head->bfs->y == Yn){
			longest = queue->head->bfs->n;
			finished = 1;
		}else{
			if(queue->head->bfs->n+1 > maxdepth){
				finished = 1;
				noroute = 1;
			}
			int x = queue->head->bfs->x;
			int y = queue->head->bfs->y;
			w.map[y][x].bfsvisited = 1;
			Node *delete = queue->head;
			if(w.map[y+1][x].type != CELL_WATER && !w.map[y+1][x].bfsvisited){
				List_append(queue);
				queue->head->prev->bfs->n = delete->bfs->n + 1;
				w.map[y+1][x].bfs = queue->head->bfs->n;
				queue->head->prev->bfs->x = x;
				queue->head->prev->bfs->y = y+1;
			}
			if(w.map[y-1][x].type != CELL_WATER && !w.map[y-1][x].bfsvisited){
				List_append(queue);
				queue->head->prev->bfs->n = delete->bfs->n + 1;
				w.map[y-1][x].bfs = queue->head->bfs->n;
				queue->head->prev->bfs->x = x;
				queue->head->prev->bfs->y = y-1;
			}
			if(w.map[y][x+1].type != CELL_WATER && !w.map[y][x+1].bfsvisited){
				List_append(queue);
				queue->head->bfs->n = delete->bfs->n + 1;
				w.map[y][x+1].bfs = queue->head->bfs->n;
				queue->head->prev->bfs->x = x+1;
				queue->head->prev->bfs->y = y;
			}
			if(w.map[y][x-1].type != CELL_WATER && !w.map[y][x-1].bfsvisited){
				List_append(queue);
				queue->head->prev->bfs->n = delete->bfs->n + 1;
				w.map[y][x-1].bfs = queue->head->bfs->n;
				queue->head->prev->bfs->x = x-1;
				queue->head->prev->bfs->y = y;
			}
			List_remove(queue,delete);
		}
	}
	if(!noroute){
		char *route = (char *)malloc(sizeof(char)*longest);
		int Xl = Xn;
		int Yl = Yn;
		int num = longest;
		for(int i = longest-1; i >= 0; i--){
			if(w.map[Yl+1][Xl].bfs == num-1){
				route[i] = 'N';
				num--;
				Yl = Yl+1;
			}else if(w.map[Yl-1][Xl].bfs == num-1){
				route[i] = 'S';
				num--;
				Yl = Yl-1;
			}else if(w.map[Yl][Xl+1].bfs == num-1){
				route[i] = 'W';
				num--;
				Xl = Xl+1;
			}else if(w.map[Yl][Xl-1].bfs == num-1){
				route[i] = 'E';
				num--;
				Xl = Xl-1;
			}else{
				fprintf(stderr, "Something went wrong in the DFS!\n");
			}
		}
		return route;
	}
	else{
		char *wrong = "x";
		return wrong;
	}

	
}


void print_map(worldmap w){
	char ant_letters[10] = {'a','b','c','d','e','f','g','h','i','j'};
	char ant_hill_letters[10] = {'A','B','C','D','E','F','G','H','I','J'};
	for(int i = 0; i < w.rows; i++){
	fprintf(stderr,"m ");
		for(int j = 0; j < w.cols; j++){
			switch(w.map[i][j].type){
			case CELL_DIRT:
				fprintf(stderr,".");
				break;
			case CELL_WATER:
				fprintf(stderr,"%%");
				break;
			case CELL_ANT:
				fprintf(stderr,"%c",ant_letters[w.map[i][j].owner]);
				break;
			case CELL_FOOD:
				fprintf(stderr,"*");
				break;
			case CELL_HILL:
				fprintf(stderr,"%d",w.map[i][j].owner);
				break;
			case CELL_ANT_ON_HILL:
				fprintf(stderr,"%c",ant_hill_letters[w.map[i][j].owner]);
				break;
																					
			}
		}
	fprintf(stderr,"\n");
	}
}

worldmap reset_map(worldmap world){
	for(int i = 0; i < world.rows; i++){
		for(int j = 0; j < world.cols; j++){
			world.map[i][j].type = CELL_DIRT;
			world.map[i][j].owner = -1;
		}
	}
	return world;

}

worldmap soft_reset_map(worldmap world){
	for(int i = 0; i < world.rows; i++){
		for(int j = 0; j < world.cols; j++){
			if(world.map[i][j].type != CELL_WATER && world.map[i][j].type != CELL_HILL && (world.map[i][j].type != CELL_ANT || world.map[i][j].owner != 0)){ 
				world.map[i][j].type = CELL_DIRT;
				world.map[i][j].owner = -1;
			}
		}
	}
	Node *current = world.ants->head;
	for(int i = 0; i < world.ants->counter; i++){	
		current->item->alive =0;
		if(current->item->x == current->item->targetx && current->item->y == current->item->targety){
			current->item->moving = 0;
		}
		current = current->next;
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

worldmap update_ants(worldmap w, int i, int j, int in, int jn, Node *current){
	if(w.map[in][jn].type != CELL_WATER){
		if(jn < 0 || jn > w.cols){
			fprintf(stderr, "amount of columns: %d, previous number j: %d\n",w.cols,jn);
			jn = mod(jn,w.cols);
		}	
		if(in < 0 || in > w.rows){
			fprintf(stderr, "amount of rows: %d, previous number i: %d\n",w.rows, in);
			in = mod(in,w.rows);
		}
		fprintf(stderr, "Moving ant %d, from (%d,%d) to (%d,%d)\n",current->item->id, j, i, jn, in);	
		if(w.map[i][j].type == CELL_ANT_ON_HILL){
			w.map[i][j].type = CELL_HILL;
		}else{
			w.map[i][j].type = CELL_DIRT;
			w.map[i][j].owner = -1;
		}
		w.map[in][jn].owner = 0;
		if(w.map[in][jn].type == CELL_HILL){
			w.map[in][jn].type = CELL_ANT_ON_HILL;
		}else{
			w.map[in][jn].type = CELL_ANT;
		}	
		current->item->y = in;
		current->item->x = jn;
	}else{
		fprintf(stderr, "Could not move!\n");
	}
	return w;
}


worldmap read_turn(worldmap w){
	char *line = NULL;
	size_t linelength = 0;
	char *key = NULL;
	char *end = "end";
	char *delimiters = " \n";
	char *saveptr;
	w = soft_reset_map(w);
	while(getline(&line, &linelength, stdin) != 0){	
		//printf("Linelenght: %zu, end: %d, end2: %d, string: %s, totalstring: %s.\n",linelength,end,end2,key,line);
		if(strcmp(line,"\n")!=0){
			long row = 0;
			long col = 0;
			long owner = 0;
			//fprintf(stderr,"I HAVE BEEN HERE, text: ");
			key = (char *) malloc(sizeof(char)*linelength);
			key = strtok_r(line, delimiters, &saveptr);
			//fprintf(stderr, "%s\n", key);
			if(key != NULL){
				if(strcmp(key,"end") == 0){
					w.end = 1;	
				}
		
				if(strcmp(key, "go") == 0){
					w.end = 0;
					break;
				}
				else if(strcmp(key, "turn") == 0){
					w.turn = strtol(strtok(saveptr, delimiters), &key, 10);	
					fprintf(stderr, "saving a turn %d\n", w.turn);			
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
					fprintf(stderr, "saving a hill of %ld on row %ld and col %ld\n", owner, row,col);
					w.map[row][col].type = CELL_HILL;
					w.map[row][col].owner = (int)owner;
				}
				else if(strcmp(key, "a") == 0){
					row = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					col = strtol(strtok_r(saveptr, delimiters, &saveptr), &key, 10);
					owner = strtol(saveptr,&key,10);
					fprintf(stderr, "saving an ant of %ld on row %ld and col %ld\n", owner, row,col);
					w.map[row][col].owner = (int)owner;
					if(w.map[row][col].type == CELL_HILL){
						w.map[row][col].type = CELL_ANT_ON_HILL;
					}else{
						w.map[row][col].type = CELL_ANT;
					}
					int antfound = 0;
					Node *current = w.ants->head;
					for(int i = 0 ; i < w.ants->counter; i++){
						if(row == current->item->y && col == current->item->x){
							fprintf(stderr, "Ant %d is alive!\n",current->item->id);
							current->item->alive = 1;
							antfound = 1;
							break;
							
						}
						current = current->next;
					}
					if(!antfound && owner == 0){ //delete last when bug is found!
						List_insert(w.ants);
						w.ants->head->item->y = row;
						w.ants->head->item->x = col;
						w.ants->head->item->alive = 1;
						w.ants->head->item->moving = 0;
						fprintf(stderr, "Ant %d has spawned!\n",w.ants->head->item->id);
						w.antnr++;
					}
				}
			}
		}	
	}
	free(line);
	return w;
}














/*modulo*/

int mod(int a, int b){
	if(b < 0){ //you can check for b == 0 separately and do what you want
		return mod(a, -b);   
	}
	int mod = a % b;
	if(mod < 0){
		mod+=b;
	}
	return mod;
}















/*linked list commands*/

void List_print(ant *list){
	Node *start = list->head;
	for(int i = 0; i < list->counter; i++){
		fprintf(stderr,"-> id: %i\n",start->item->id);
		start = start->next;
	}
}

void List_append(ant *list){
	if(list->counter!=0){	
		Node *previous = list->head->prev;
		/*for(int i = 1; i < list->counter; i++){
			previous = previous->next;	
		}*/
		Node *new;
		Item *newitem; 
		BFS *newbfs;
		newitem = (Item *)malloc(sizeof(Item));
		newbfs = (BFS *)malloc(sizeof(BFS));
		new = (Node *)malloc(sizeof(Node));
		new->bfs = newbfs;
		newitem->id = list->idcounter;
		new->item = newitem;
		Node *next = previous->next;
		previous->next = new;
		new->prev = previous;
		new->next = next;
		next->prev = new;
		list->counter++;
		list->idcounter++;
	}else{
		Node *start;
		Item *item;
		BFS *bfs;
		item = (Item *)malloc(sizeof(Item));
		start = (Node *)malloc(sizeof(Node));
		bfs = (BFS *)malloc(sizeof(BFS));
		item->id = 0;
		start->item = item;
		start->bfs = bfs;
		start->next = start;
		start->prev = start;
		list->head = start;
		list->counter++;
	}
}

void List_putfirst(ant *list, Node *node){
	Node *previous = list->head;
	int found = 0;
	for(int i = 0; i < list->counter; i++){
		if(previous == node){
			found = 1;
			break;
		}else{	
			previous = previous->next;
		}	
	}
	if(found){
		Node *before = previous->prev;
		Node *after = previous->next;
		Node *last = list->head->prev;
		before->next = after;
		after->prev = before;		
		previous->next = list->head;
		previous->prev = last;
		list->head->prev = previous;
		last->next = previous;
		list->head = previous;	
	}else{
		printf("Error, node not found!\n");
	}

}

void List_sort(ant* list){
	int *id;
	Node *previous = list->head;
	id = (int *)malloc(sizeof(int)*list->counter);
	for(int i = 0; i < list->counter; i++){
		id[i] = previous->item->id;		
		previous = previous->next;
	}
	//bubble sort
	int finished = 0;
	while(!finished){
		finished = 1;
		for(int i = 0; i < (list->counter-1);i++){
			if(id[i+1] > id[i]){
				finished = 0;
				int temp = id[i];
				id[i] = id[i+1];
				id[i+1] = temp;
			}
		}
	}
	for(int i = 0; i < list->counter; i++){
	    Node* tomove = List_find(list,id[i]);
            List_putfirst(list,tomove);
	}

}

void List_remove(ant *list, Node *node){
	Node *previous = list->head;
	int found = 0;
	for(int i = 0; i < list->counter; i++){
		if(previous == node){
			found = 1;
			break;
		}else{	
			previous = previous->next;
		}	
	}
	if(found){
		fprintf(stderr, "List before deleting ant %d\n",node->item->id);
		List_print(list);
		fprintf(stderr, "Deleted ant %d! New list will be printed:\n", node->item->id);
		if(previous == list->head){
			list->head = previous->next;
		}
		Node *before = previous->prev;
		Node *after = previous->next;
		before->next = after;
		after->prev = before;
		free(previous->item);
		free(previous);
		list->counter--;
		List_print(list);
	}else{
		fprintf(stderr,"Error, node not found!\n");
	}
}

void List_destroy(ant* list){
	Node *previous = list->head;
	for(int i = 0; i < list->counter; i++){
		Node *next = previous->next;
		free(previous->item);
		free(previous);
		previous = next;
	}
	list->counter = 0;
}

void List_insert(ant *list){
	if(list->counter != 0){
		Node *next = list->head;
		Node *last = list->head->prev;
		Node *new;
		Item *newitem; 
		BFS *newbfs;
		newitem = (Item *)malloc(sizeof(Item));
		new = (Node *)malloc(sizeof(Node));
		newbfs = (BFS *)malloc(sizeof(BFS));
		newitem->id = list->idcounter;
		new->item = newitem;
		new->bfs = newbfs;
		next->prev = new;
		new->next = next;
		new->prev = last;
		last->next = new;
		list->head = new;
		list->counter++;
		list->idcounter++;
	}
	else{
		Node *start;
		Item *item;
		BFS *bfs;
		bfs = (BFS *)malloc(sizeof(BFS));
		item = (Item *)malloc(sizeof(Item));
		start = (Node *)malloc(sizeof(Node));
		item->id = 0;
		start->item = item;
		start->bfs = bfs;
		start->next = start;
		start->prev = start;
		list->head = start;
		list->counter++;
		list->idcounter++;
	}
}


ant *List_create(){
	ant *list;
	list = (ant *)malloc(sizeof(ant));
}



Node *List_find(ant *list, int id){
	Node *previous = list->head;
	for(int i = 0; i < list->counter; i++){
		if(previous->item->id == id){
			return previous;
		}else{
			previous = previous->next;	
		}
	}
	return 0;
}
