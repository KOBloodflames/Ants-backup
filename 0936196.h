
typedef enum {CELL_DIRT , CELL_WATER, CELL_ANT , CELL_FOOD , CELL_HILL , CELL_ANT_ON_HILL }  cell_type;
typedef struct {cell_type type; int owner;} cell;
typedef struct {cell **map; unsigned int rows; unsigned int cols;} worldmap;
typedef struct {int turn; int loadtime; int turntime; int rows; int cols; int turns; int viewradius2; int attackradius2; int spawnradius2; int player_seed;} game_settings;


void initialize_map();
void cleanup_map();
void print_map(worldmap w);
void read_initialization(game_settings *s);

