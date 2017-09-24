#ifndef ROUTER_H_   /* Include guard */
#define ROUTER_H_

#include "cdefs.h"

#define EXIT_UNROUTABLE -1		// 
#define EXIT_ERROR 2			//
static int PATH_STATUS=0;

#define INC_COST 1

int route_path(struct chip *mchip_t, int src_x, int src_y, int src_pin, int targ_x, int targ_y, int targ_pin, char switch_type);

#endif // FOO_H_