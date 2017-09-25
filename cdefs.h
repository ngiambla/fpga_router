
#define UNAVAIL -3		// For sblock
#define INIT -1
#define USED -44

#define TARGET -4		// For lblock
#define SOURCE -5		// For lblock

#define SOUTH 1 		// for block pins
#define EAST 2 
#define NORTH 3 	
#define WEST 0 		

#define WILTON 'W'		// For Wilton Switch
#define FULL 'F'		// For Fully-Connected Switch

/* lblock: Logic Block repsentation */
// [W,S,E,N]
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

/* To Store Routes once complete. */
struct route {
	struct sblock *switch_path;
};

/* Expansion List for Routing */
struct elist {
	int * x;
	int * y;
	int * entered;
};

/* Entire Chip */
struct chip {
	int grid_size;
	int width;
	struct lblock **logic_grid;
	struct sblock **switch_grid;
};