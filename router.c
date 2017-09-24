#include <stdio.h>
#include "router.h"


void reset_connections(struct chip *mchip, int size, int width) {
	int i, j, k;
	for(i=0;i<size+1;++i) {
		for(j=0;j<size+1;++j) {
			for(k=0;k<width;++k) {

			}
		}
	}
}

void trace_back() {

}


void search(struct chip *mchip_t, int x, int y, int cameFrom) {
	struct chip mchip;
	mchip=*mchip_t;
	switch(cameFrom) {
		case NORTH:

			break;
		case SOUTH:
			break;

		case EAST:
			break;

		case WEST:
			break;
	}
	*mchip_t=mchip;

}

void begin_search(struct chip *mchip_t, int src_x, int src_y, int src_pin) {
	int i=0;
	int noneAvail=0;
	struct chip mchip;
	mchip=*mchip_t;
	int width=mchip.width;

	switch(src_pin) {
		case NORTH:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y+1].w_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y+1].e_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x][src_y+1].w_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y+1].e_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x, src_y+1, WEST);
					search(&mchip, src_x+1,src_y+1, EAST);
				}
			break;
		case EAST:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x+1][src_y+1].s_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y].n_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].s_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y].n_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x+1, src_y+1, SOUTH);
					search(&mchip, src_x+1,src_y, NORTH);
				}
			break;
		case SOUTH:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y].w_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y].e_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y].e_pins[i]=0;
						mchip.switch_grid[src_x][src_y].e_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x, src_y, WEST);
					search(&mchip, src_x+1,src_y, EAST);
				}
			break;
		case WEST:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y+1].s_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y].n_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x][src_y+1].s_pins[i]=0;
						mchip.switch_grid[src_x][src_y].n_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x, src_y+1, WEST);
					search(&mchip, src_x+1,src_y+1, EAST);
				}
			break;
	}
	*mchip_t=mchip;
}


int route_path(struct chip *mchip_t, int src_x, int src_y, int src_pin, int targ_x, int targ_y, int targ_pin, char switch_type ) {

	printf("[INFO] --Beginning Path Search\n");
	begin_search(mchip_t, src_x,src_y,src_pin);
	
	return PATH_STATUS;
}