#include <stdio.h>

#define UNAVAIL -1		// For sblock
#define TARGET -2		// For lblock
#define SOURCE -3		// For lblock


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
	int * sources;
	int * targets;
};

