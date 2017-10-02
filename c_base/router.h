#ifndef ROUTER_H_   /* Include guard */
#define ROUTER_H_

#include "cdefs.h"
//#include <pthread.h>

// Define Path Codes.
#define EXIT_UNROUTABLE -1
#define EXIT_ERROR 2		
static int PATH_STATUS=0;

// Cost per new line.
#define INC_COST 1

// Define a FOUND signal.
#define FOUND 0
#define NOT_FOUND -1
static int PATH_FOUND=0;

//pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER; 


int route_path(struct chip *mchip_t, int src_x, int src_y, int src_pin, int targ_x, int targ_y, int targ_pin, char is_parallel);
void reset_router(struct chip *mchip_t);

#endif // ROUTER_H_