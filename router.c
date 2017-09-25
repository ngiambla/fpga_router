#include <stdio.h>
#include "router.h"


void reset_connections(struct chip *mchip_t) {
	int size, width;
	struct chip mchip;
	mchip=*mchip_t;
	width=mchip.width;
	size=mchip.grid_size;
	int i, j, k;
	for(i=0;i<size+1;++i) {
		for(j=0;j<size+1;++j) {
			for(k=0;k<width;++k) {
				if(mchip.switch_grid[i][j].e_pins[k]>=0) {
					mchip.switch_grid[i][j].e_pins[k]=INIT;
				}
				if(mchip.switch_grid[i][j].n_pins[k]>=0) {
					mchip.switch_grid[i][j].n_pins[k]=INIT;
				}
				if(mchip.switch_grid[i][j].s_pins[k]>=0) {
					mchip.switch_grid[i][j].s_pins[k]=INIT;
				}
				if(mchip.switch_grid[i][j].w_pins[k]>=0) {
					mchip.switch_grid[i][j].w_pins[k]=INIT;
				}
			}
		}
	}
	*mchip_t=mchip;
}

void reset_reqs(struct chip *mchip_t) {
	int i,j,k,size;
	struct chip mchip;
	mchip=*mchip_t;
	size=mchip.grid_size;
	for(i=0;i<size;++i) {
		for(j=0;j<size;++j) {
			for(k=0;k<4;++k) {
				mchip.logic_grid[i][j].pins[k]=0;
			}
		}
	}
	*mchip_t=mchip;
}

void trace_back(struct chip *mchip_t) {
	struct chip mchip;
	mchip=*mchip_t;
	PATH_FOUND=1;
	printf("-- Trace Back Instantiated.\n");
	*mchip_t=mchip;
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
				mchip.switch_grid[x][y].e_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
				*mchip_t=mchip;
				return FOUND;
			} 
			if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
				mchip.switch_grid[x][y].e_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
				*mchip_t=mchip;
				return FOUND;
			}
		}
		//check west
		if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && cameFrom != WEST) {
			if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
				mchip.switch_grid[x][y].w_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);

				*mchip_t=mchip;
				return FOUND;
			} 
			if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
				mchip.switch_grid[x][y].w_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);

				*mchip_t=mchip;
				return FOUND;
			}
		}
		//check south
		if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && cameFrom != SOUTH) {
			if(y>0){
				if(mchip.logic_grid[x][y-1].pins[EAST]==TARGET) {
				mchip.switch_grid[x][y].s_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);

					*mchip_t=mchip;
					return FOUND;							
				} 
			}
			if(y<size){
				if(mchip.logic_grid[x][y].pins[WEST]==TARGET) {
				mchip.switch_grid[x][y].w_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);

					*mchip_t=mchip;
					return FOUND;	
				}
			}
		}
		//check north
		if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && cameFrom != NORTH) {
			if(y>0){
				if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
				mchip.switch_grid[x][y].n_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y, mchip.switch_grid[x][y].n_pins[i]);

					*mchip_t=mchip;
					return FOUND;							
				} 
			}
			if(y<size){
				if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
				mchip.switch_grid[x][y].n_pins[i]=UNAVAIL;
				printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y, mchip.switch_grid[x][y].n_pins[i]);

					*mchip_t=mchip;
					return FOUND;	
				}
			}
		}

	}

	*mchip_t=mchip;
	return NOT_FOUND;
}

// int * hiltonSwitch() {
// 	int next[3];
// 	return next;
// }

int add_to_queue(struct chip *mchip_t, int x, int y, int src, int curr ) {

	struct chip mchip;
	mchip=*mchip_t;
	mchip.elist.sblocks[curr]=mchip.switch_grid[x][y];
	mchip.elist.x[curr]=x;
	mchip.elist.y[curr]=y;
	mchip.elist.entering_from[curr]=src;
	mchip.elist.used[curr]=0;
	*mchip_t=mchip;
	return ++curr;

}

void search_s(struct chip *mchip_t, int x1, int y1, int src1, int x2, int y2, int src2) {
	int i, tail=0, curr=0, size, width, *costVec, x, y, cameFrom, goN=0, goE=0, goW=0, goS=0;
	struct chip mchip;
	mchip=*mchip_t;
	size=mchip.grid_size;
	width=mchip.width;

	tail=add_to_queue(&mchip,x1,y1,src1,tail);
	tail=add_to_queue(&mchip,x2,y2,src2,tail);

	printf("[[.-* .-* .-* .-* .-* .-*]]\n");

	while(curr < (size+1)*(size+1) && curr < tail) {
		goN=0;
		goE=0; 
		goW=0;
		goS=0;
		x=mchip.elist.x[curr];
		y=mchip.elist.y[curr];
		cameFrom=mchip.elist.entering_from[curr];
		if(0==checkForCompletion(&mchip, x, y, cameFrom)){
			break;
		}
		switch(cameFrom){
			case NORTH:
				printf("Entering From: [NORTH]\n");
				costVec=mchip.switch_grid[x][y].n_pins;
				break;
			case SOUTH:
				printf("Entering From: [SOUTH]\n");
				costVec=mchip.switch_grid[x][y].s_pins;
				break;
			case EAST:
				printf("Entering From: [EAST]\n");
				costVec=mchip.switch_grid[x][y].e_pins;
				break;
			case WEST:
				printf("Entering From: [WEST]\n");
				costVec=mchip.switch_grid[x][y].w_pins;
				break;
		}
		for(i=0;i<width;++i){
			printf("[%d]",costVec[i]);
		}
		printf("\n");

		for(i=0;i<width;++i) {
			// go west
			if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL && cameFrom != EAST) {
				++goW;
				if(mchip.switch_grid[x][y].e_pins[i]==INIT || mchip.switch_grid[x][y].e_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].e_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x][y+1].w_pins[i]==INIT || mchip.switch_grid[x][y+1].w_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x][y+1].w_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go east
			if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && cameFrom != WEST) {
				++goE;
				if(mchip.switch_grid[x][y].w_pins[i]==INIT || mchip.switch_grid[x][y].w_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].w_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x][y-1].e_pins[i]==INIT || mchip.switch_grid[x][y-1].e_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x][y-1].e_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go north
			if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && cameFrom != SOUTH) {
				++goN;
				if(mchip.switch_grid[x][y].s_pins[i]==INIT || mchip.switch_grid[x][y].s_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].s_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x+1][y].n_pins[i]==INIT || mchip.switch_grid[x+1][y].n_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x+1][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go south
			if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && cameFrom != NORTH) {
				++goS;
				if(mchip.switch_grid[x][y].n_pins[i]==INIT || mchip.switch_grid[x][y].n_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x-1][y].s_pins[i]==INIT || mchip.switch_grid[x-1][y].s_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x-1][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
			}
		}
		//Continue Search...
		if(goS > 0) {
			tail=add_to_queue(&mchip,x-1,y,NORTH,tail);
			//search(&mchip, x-1, y, NORTH);
		} 
		if(goN > 0) {
			tail=add_to_queue(&mchip,x+1,y,SOUTH,tail);
			//search(&mchip, x+1, y, SOUTH);
		}
		if(goE > 0) {
			tail=add_to_queue(&mchip,x,y-1,WEST,tail);
			//search(&mchip, x, y-1, WEST);
		}
		if(goW > 0) {
			tail=add_to_queue(&mchip,x,y+1,EAST,tail);
			//search(&mchip, x, y+1, EAST);
		}
		++curr;
	}
	*mchip_t=mchip;
}

void search(struct chip *mchip_t, int x, int y, int cameFrom) {
	int i, *costVec, size, width, shouldTrace, goN=0, goE=0, goW=0, goS=0;
	struct chip mchip;
	mchip=*mchip_t;
	size=mchip.grid_size;
	width=mchip.width;

	shouldTrace=checkForCompletion(&mchip, x, y, cameFrom);
	if(shouldTrace==0) {
		PATH_FOUND=1;
		//trace_back(&mchip);
	} else if(PATH_FOUND == 1) {
		printf("-- skipping iter.\n");
	} else {
		switch(cameFrom){
			case NORTH:
				printf("Entering From: [NORTH]\n");
				costVec=mchip.switch_grid[x][y].n_pins;
				break;
			case SOUTH:
				printf("Entering From: [SOUTH]\n");
				costVec=mchip.switch_grid[x][y].s_pins;
				break;
			case EAST:
				printf("Entering From: [EAST]\n");
				costVec=mchip.switch_grid[x][y].e_pins;
				break;
			case WEST:
				printf("Entering From: [WEST]\n");
				costVec=mchip.switch_grid[x][y].w_pins;
				break;
		}
		for(i=0;i<width;++i){
			printf("[%d]",costVec[i]);
		}
		printf("\nsblock[%d][%d]\n", x,y);
		read(0,NULL,1);

		for(i=0;i<width;++i) {
			// go west
			if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL && cameFrom != EAST) {
				++goW;
				if(mchip.switch_grid[x][y].e_pins[i]==INIT || mchip.switch_grid[x][y].e_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].e_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x][y+1].w_pins[i]==INIT || mchip.switch_grid[x][y+1].w_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x][y+1].w_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go east
			if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && cameFrom != WEST) {
				++goE;
				if(mchip.switch_grid[x][y].w_pins[i]==INIT || mchip.switch_grid[x][y].w_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].w_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x][y-1].e_pins[i]==INIT || mchip.switch_grid[x][y-1].e_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x][y-1].e_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go north
			if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && cameFrom != SOUTH) {
				++goN;
				if(mchip.switch_grid[x][y].s_pins[i]==INIT || mchip.switch_grid[x][y].s_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].s_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x+1][y].n_pins[i]==INIT || mchip.switch_grid[x+1][y].n_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x+1][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
			}
			//go south
			if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && cameFrom != NORTH) {
				++goS;
				if(mchip.switch_grid[x][y].n_pins[i]==INIT || mchip.switch_grid[x][y].n_pins[i] < *(costVec+i)+INC_COST){
					mchip.switch_grid[x][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
				if(mchip.switch_grid[x-1][y].s_pins[i]==INIT || mchip.switch_grid[x-1][y].s_pins[i] < *(costVec+i)+INC_COST) {
					mchip.switch_grid[x-1][y].n_pins[i]=*(costVec+i)+INC_COST;
				}
			}
		}
		//Continue Search...
		if(goS > 0) {
			//printf("[%d][%d]\n",x-1,y);
			search(&mchip, x-1, y, NORTH);
		} 
		if(goN > 0) {
			//printf("[%d][%d]\n",x+1,y);
			search(&mchip, x+1, y, SOUTH);
		}
		if(goE > 0) {
			//printf("[%d][%d]\n",x,y-1);
			search(&mchip, x, y-1, WEST);
		}
		if(goW > 0) {
			//printf("[%d][%d]\n",x,y+1);
			search(&mchip, x, y+1, EAST);
		}
	}
	*mchip_t=mchip;

}

void begin_search(struct chip *mchip_t, int src_x, int src_y, int src_pin, char is_parallel) {
	int i=0;
	int noneAvail=0;
	struct chip mchip;
	mchip=*mchip_t;
	int width=mchip.width;

	// Set up threads
	// pthread_t threads[2];
	// void *status;

	printf("[INFO] Beginning Search @ L-Block[%d][%d]\n",src_x, src_y);
	switch(src_pin) {
		case NORTH:
				printf("[INFO] Start @ NORTH\n");
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x][src_y].e_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].w_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x][src_y].e_pins[i]=0;
						mchip.switch_grid[src_x][src_y+1].w_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {

					if(is_parallel=='T') {
						search(&mchip, src_x, src_y, EAST);
						search(&mchip, src_x,src_y+1, WEST);
						trace_back(&mchip);
					} else {
						search_s(&mchip, src_x, src_y, EAST, src_x, src_y+1, WEST);
						trace_back(&mchip);
					}
				}
			break;
		case EAST:
				printf("[INFO] Start @ EAST\n");
				for(int i=0; i<width; ++i){
					if(mchip.switch_grid[src_x+1][src_y+1].n_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].s_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].n_pins[i]=0;
						mchip.switch_grid[src_x][src_y+1].s_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==3) {
					printf("[ERR] No free path from source...?");
				} else {
					if(is_parallel=='T'){
						search(&mchip, src_x+1, src_y+1, NORTH);
						search(&mchip, src_x, src_y+1, SOUTH);
						trace_back(&mchip);
					} else {
						search_s(&mchip, src_x+1, src_y+1, NORTH, src_x, src_y+1, SOUTH);
						trace_back(&mchip);
					}
				}
			break;
		case SOUTH:
				printf("[INFO] Start @ SOUTH\n");
				for(int i=0; i<width; ++i) {
					if(mchip.switch_grid[src_x+1][src_y].e_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y+1].w_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].w_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y].e_pins[i]=0;
					} else {
						noneAvail++;
					}

				}
				if(noneAvail==width) {
					printf("[ERR] No free path from source...?");
				} else {
					if(is_parallel=='T') {
						search(&mchip, src_x+1, src_y, EAST);
						search(&mchip, src_x+1 ,src_y+1, WEST);
						trace_back(&mchip);
					} else {
						search_s(&mchip, src_x+1, src_y, EAST, src_x+1, src_y+1, WEST);
						trace_back(&mchip);
					}
				}
			break;
		case WEST:
				printf("[INFO] Start @ WEST\n");
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
					if(is_parallel=='T') {
						search(&mchip, src_x, src_y, SOUTH);
						search(&mchip, src_x+1,src_y, NORTH);
						trace_back(&mchip);
					} else {
						search_s(&mchip, src_x, src_y, SOUTH, src_x+1, src_y, NORTH);
						trace_back(&mchip);
					}
				}
			break;
	}
	*mchip_t=mchip;
}

int reset_router(struct chip *mchip_t) {
	printf("-- Resetting Router for next Route.\n");
	PATH_FOUND=0;
	reset_connections(mchip_t);
	reset_reqs(mchip_t);
	return 0; // change to status;
}

int route_path(struct chip *mchip_t, int src_x, int src_y, int src_pin, int targ_x, int targ_y, int targ_pin, char switch_type, char is_parallel) {

	printf("[INFO] --Beginning Path Search\n");
	begin_search(mchip_t, src_x,src_y,src_pin, is_parallel);
	
	return PATH_STATUS;
}