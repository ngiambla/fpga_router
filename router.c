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
	printf("-- Trace Back Instantiated.\n");
}

int checkForCompletion(struct chip *mchip_t, int x, int y, int cameFrom) {
	int i, j, width, size;
	struct chip mchip;
	mchip=*mchip_t;
	width=mchip.width;
	size=mchip.grid_size;

	for(i=0;i<width;++i) {
		// check east
		if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL && cameFrom != EAST) {

			if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET){
				*mchip_t=mchip;
				return FOUND;
			} 
			if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
				*mchip_t=mchip;
				return FOUND;
			}
		}
		//check west
		if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && cameFrom != WEST) {
			if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
				*mchip_t=mchip;
				return FOUND;
			} 
			if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
				*mchip_t=mchip;
				return FOUND;
			}
		}
		//check south
		if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && cameFrom != SOUTH) {
			if(y>0){
				if(mchip.logic_grid[x][y-1].pins[EAST]==TARGET) {
					*mchip_t=mchip;
					return FOUND;							
				} 
			}
			if(y<size){
				if(mchip.logic_grid[x][y].pins[WEST]==TARGET) {
					*mchip_t=mchip;
					return FOUND;	
				}
			}
		}
		//check north
		if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && cameFrom != NORTH) {
			if(y>0){
				if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
					*mchip_t=mchip;
					return FOUND;							
				} 
			}
			if(y<size){
				if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
					*mchip_t=mchip;
					return FOUND;	
				}
			}
		}

	}

	*mchip_t=mchip;
	return NOT_FOUND;
}


void search(struct chip *mchip_t, int x, int y, int cameFrom) {
	int size, width, shouldTrace;
	struct chip mchip;
	mchip=*mchip_t;
	size=mchip.grid_size;
	width=mchip.width;
	
	shouldTrace=checkForCompletion(&mchip, x, y, cameFrom);
	if(shouldTrace==0) {
		trace_back();
	} else {
		for(i=0;i<width;++i) {
			// check east
			if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL && cameFrom != EAST) {

				if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET){
					*mchip_t=mchip;
					return FOUND;
				} 
				if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
					*mchip_t=mchip;
					return FOUND;
				}
			}
			//check west
			if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && cameFrom != WEST) {
				if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
					*mchip_t=mchip;
					return FOUND;
				} 
				if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
					*mchip_t=mchip;
					return FOUND;
				}
			}
			//check south
			if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && cameFrom != SOUTH) {
				if(y>0){
					if(mchip.logic_grid[x][y-1].pins[EAST]==TARGET) {
						*mchip_t=mchip;
						return FOUND;							
					} 
				}
				if(y<size){
					if(mchip.logic_grid[x][y].pins[WEST]==TARGET) {
						*mchip_t=mchip;
						return FOUND;	
					}
				}
			}
			//check north
			if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && cameFrom != NORTH) {
				if(y>0){
					if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
						*mchip_t=mchip;
						return FOUND;							
					} 
				}
				if(y<size){
					if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
						*mchip_t=mchip;
						return FOUND;	
					}
				}
			}
		}
	}
	*mchip_t=mchip;

}

void begin_search(struct chip *mchip_t, int src_x, int src_y, int src_pin) {
	int i=0;
	int noneAvail=0;
	struct chip mchip;
	mchip=*mchip_t;
	int width=mchip.width;
	printf("[INFO] Beginning Search @ L-Block[%d][%d]\n",src_x, src_y);
	switch(src_pin) {
		case NORTH:
				printf("Originating from North Direction.\n\n");
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y].w_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].e_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x][src_y].w_pins[i]=0;
						mchip.switch_grid[src_x][src_y+1].e_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x, src_y, WEST);
					search(&mchip, src_x,src_y+1, EAST);
				}
			break;
		case EAST:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x+1][src_y+1].s_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].n_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].s_pins[i]=0;
						mchip.switch_grid[src_x][src_y+1].n_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x+1, src_y+1, SOUTH);
					search(&mchip, src_x, src_y+1, NORTH);
				}
			break;
		case SOUTH:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x+1][src_y].w_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y+1].e_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].e_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y].w_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x+1, src_y, WEST);
					search(&mchip, src_x+1 ,src_y+1, EAST);
				}
			break;
		case WEST:
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y].s_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y].n_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x][src_y].s_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y].n_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					search(&mchip, src_x, src_y, SOUTH);
					search(&mchip, src_x+1,src_y, NORTH);
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