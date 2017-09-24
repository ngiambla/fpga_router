
#define UNAVAIL -1		// For sblock
#define INIT -2

#define TARGET -4		// For lblock
#define SOURCE -5		// For lblock

#define SOUTH 0 		// for block pins
#define EAST 1 
#define NORTH 2 	
#define WEST 3 		

#define WILTON 'W'		// For Wilton Switch
#define FULL 'F'		// For Fully-Connected Switch

/* lblock: Logic Block repsentation */
// [S,E,N,W]
struct lblock {
	int pins[4];
};

/* sblock: Switch Block representation */
struct sblock {
	int * e_pins;
	int * n_pins;
	int * w_pins;
	int * s_pins;
};

/* Entire Chip */
struct chip {
	int grid_size;
	int width;
	struct lblock **logic_grid;
	struct sblock **switch_grid;
};

struct path {

	struct sblock *switch_path;
};

