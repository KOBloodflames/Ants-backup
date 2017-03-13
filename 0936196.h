
typedef enum {CELL_DIRT , CELL_WATER, CELL_ANT , CELL_FOOD , CELL_HILL , CELL_ANT_ON_HILL }  cell_type;
typedef enum {GATHERER , EXPLORER , DEFENDER} ant_role;
typedef struct {cell_type type; int owner;} cell;
typedef struct {int x; int y; ant_role role; int alive; int moving; int targetx; int targety;} ant;
typedef struct {cell **map; unsigned int rows; unsigned int cols; unsigned int turn; int end; ant ants[1000]; int antnr;} worldmap;
typedef struct {int turn; int loadtime; int turntime; int rows; int cols; int turns; int viewradius2; int attackradius2; int spawnradius2; int player_seed;} game_settings; 

worldmap read_turn(worldmap w);
worldmap initialize_map(worldmap w);
worldmap cleanup_map(worldmap w);
worldmap reset_map(worldmap world);
worldmap update_ants(worldmap w, int i, int j, int in, int jn, int ant);
void print_map(worldmap w);
void read_initialization(game_settings *s);
worldmap send_orders(worldmap w);
worldmap find_route(worldmap w, int Y, int X, int Yn, int Xn, int ant);
