typedef enum {CELL_DIRT , CELL_WATER, CELL_ANT , CELL_FOOD , CELL_HILL , CELL_ANT_ON_HILL }  cell_type;
typedef enum {N, E, S, W} direction;
typedef enum {GATHERER , EXPLORER , DEFENDER} ant_role;
typedef struct {cell_type type; int owner;} cell;
//typedef struct {int x; int y; ant_role role; direction lastmove; int water; int alive; int moving; int targetx; int targety;} ant;
typedef struct {int turn; int loadtime; int turntime; int rows; int cols; int turns; int viewradius2; int attackradius2; int spawnradius2; int player_seed;} game_settings; 

/*linked list*/
struct Node;
typedef struct Item{int id; int x; int y; ant_role role; direction lastmove; int water; int alive; int moving; int targetx; int targety;} Item;
typedef struct Node{struct Node* next; struct Node* prev; struct Item* item;} Node;
typedef struct ant {int idcounter; int counter; Node* head;} ant;

typedef struct {cell **map; int rows; int cols; unsigned int turn; int end; ant *ants; int antnr;} worldmap;

int mod(int a, int b);

ant *List_create();
void List_destroy(ant *list);
void List_clear(ant *list);
void List_print(ant *list);

void List_append(ant *list);
void List_insert(ant *list);

void List_remove(ant* list, Node* node);
Node* List_find(ant *list,int id);
void List_putfirst(ant *list,Node* node);
void List_sort(ant* list);


worldmap read_turn(worldmap w);
worldmap initialize_map(worldmap w);
worldmap cleanup_map(worldmap w);
worldmap reset_map(worldmap world);
worldmap update_ants(worldmap w, int i, int j, int in, int jn, Node *ant);
void print_map(worldmap w);
void read_initialization(game_settings *s);
worldmap send_orders(worldmap w);
worldmap find_route(worldmap w, int Y, int X, int Yn, int Xn, Node *ant);
