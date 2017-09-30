#include <stdio.h>
#include <stdlib.h>
//#include <signal.h>
#include "router.h"

/********************
GLOBAL VARS
********************/
int verbose; 			// flag for debugging

/* function reset_connections()
 * -- resets all unused connections
 */
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

int select_lowest_cost_idx(int * cost, int size) {
	int i=0;
	int min=cost[0];
	int idx=0;
	for(i=0; i<size; i++) {
		if(cost[i]<=min) {
			idx=i;
		}
	}
	return idx;
}

int get_backwards_path(int pin1, int pin2, int pin3, int dir1, int dir2, int dir3) {
	int dirs[3], cost[3], idx;
	dirs[0]=dir1;
	dirs[1]=dir2;
	dirs[2]=dir3;

	cost[0]=pin1;
	cost[1]=pin2;
	cost[2]=pin3;

	idx=select_lowest_cost_idx(cost,3);

	return dirs[idx];
}

void traceback(struct chip *mchip_t, int x, int y, int heading) {
	printf("--Heading [%d]\n",heading);
				// switch(nextBlck){
				// 	case NORTH:
				// 		//traceback(&mchip, x-1, y, nextBlck)
				// 		break;
				// 	case SOUTH:
				// 		//traceback(&mchip, x+1, y, nextBlck)
				// 		break;
				// 	case WEST:
				// 		//traceback(&mchip, x-1, y, nextBlck)
				// 		break;
				// }
}

void init_traceback(struct chip *mchip_t, int x, int y, int heading) {

	int i, j, k;
	int complete=0, cost=0, c1, c2, c3, nextBlck;
	char head_back=0;
	struct chip mchip;
	struct wilton_switch w_map;

	mchip=*mchip_t;
	w_map=mchip.switch_w;

	PATH_FOUND=1;

	printf("-- Trace Back Instantiated.\n");


	if(mchip.switch_type == 'f') {

	} else {
		for(i=0;i<mchip.width;++i) {
			if(mchip.switch_grid[x][y].e_pins[i]== USED){
				mchip.switch_grid[x][y].e_pins[i]=UNAVAIL;
				if(y+1 <= mchip.grid_size)
					mchip.switch_grid[x][y+1].w_pins[i]=UNAVAIL;
				c1=mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]];
				c2=mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]];
				c3=mchip.switch_grid[x][y].w_pins[i];
				nextBlck=get_backwards_path(c1,c2,c3,SOUTH, NORTH, WEST);
				traceback(&mchip, x, y, nextBlck);
				break;

			} else if(mchip.switch_grid[x][y].w_pins[i] == USED) {
				mchip.switch_grid[x][y].w_pins[i]=UNAVAIL;
				if(y-1 >= 0)
					mchip.switch_grid[x][y-1].e_pins[i]=UNAVAIL;
				c1=mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]];
				c2=mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]];
				c3=mchip.switch_grid[x][y].e_pins[i];
				nextBlck=get_backwards_path(c1,c2,c3,SOUTH, NORTH, EAST);
				traceback(&mchip, x, y, nextBlck);
				break;

			} else if(mchip.switch_grid[x][y].n_pins[i] == USED) {
				mchip.switch_grid[x][y].n_pins[i]=UNAVAIL;
				if(x-1 >= 0)
					mchip.switch_grid[x-1][y].s_pins[i]=UNAVAIL;
				c1=mchip.switch_grid[x][y].s_pins[i];
				c2=mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]];
				c3=mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]];
				nextBlck=get_backwards_path(c1,c2,c3,SOUTH, EAST, WEST);
				traceback(&mchip, x, y, nextBlck);
				break;

			} else if(mchip.switch_grid[x][y].s_pins[i] == USED) {
				mchip.switch_grid[x][y].s_pins[i]=UNAVAIL;
				if(x+1 <= mchip.grid_size)
					mchip.switch_grid[x+1][y].n_pins[i]=UNAVAIL;
				c1=mchip.switch_grid[x][y].n_pins[i];
				c2=mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]];
				c3=mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]];
				nextBlck=get_backwards_path(c1,c2,c3,NORTH, EAST, WEST);
				traceback(&mchip, x, y, nextBlck);

				break;
			}
		}
	}
	*mchip_t=mchip;
}

int check_for_target(struct chip *mchip_t, int x, int y, int heading) {
	int i, j, width, size;
	struct chip mchip;
	struct wilton_switch w_map;

	mchip=*mchip_t;
	width=mchip.width;
	size=mchip.grid_size;
	w_map=mchip.switch_w;

	if(mchip.switch_type == 'f') {
		for(i=0;i<width;++i) {
			// check east
			if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL) {
				if(x-1>=0) {
					if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET){
						mchip.switch_grid[x][y].e_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;
					} 
				}

				if(x<size){
					if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
						mchip.switch_grid[x][y].e_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;
					}
				}

			}
			//check west
			if(mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL) {
				if(y-1 >=0 && x-1 >=0){
					if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
						mchip.switch_grid[x][y].w_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;
					} 
				}
				if(x<size){
					if(y-1>= 0 && mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
						mchip.switch_grid[x][y].w_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;
					}
				}
			}

			//check south
			if(mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL) {
				if(x<size && y-1>=0){
					if(mchip.logic_grid[x][y-1].pins[EAST]==TARGET) {
						mchip.switch_grid[x][y].s_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;							
					} 
				}
				if(x<size){
					if(y-1>= 0 && mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
						mchip.switch_grid[x][y].w_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;
					}
				}
		
				
			}

			//check north
			if(mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL) {
				if(x-1>=0 && y-1>=0){
					if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
						mchip.switch_grid[x][y].n_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;							
					} 
				}
				if(x-1>=0){
					if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
						mchip.switch_grid[x][y].n_pins[i]=USED;
						printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,i);
						*mchip_t=mchip;
						return FOUND;	
					}
				}
			}
		}
	} else {
		// for wilton
		for(i=0; i< width; ++i) {
			switch(heading) {
				case NORTH:

					// check south 
					if(mchip.switch_grid[x][y].s_pins[i] != UNAVAIL) {
						if(y-1 >= 0 && x < size) {
							if(mchip.logic_grid[x][y-1].pins[EAST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.w_to_s[i]);
								mchip.switch_grid[x][y].s_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x < size) {
							if(mchip.logic_grid[x][y].pins[WEST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.w_to_s[i]);
								mchip.switch_grid[x][y].s_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					// check east
					if(mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]] != UNAVAIL) {
						if(x-1 >= 0) {
							if(mchip.logic_grid[x-1][y].pins[SOUTH] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.s_to_e[i]);
								mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x < size) {
							if(mchip.logic_grid[x][y].pins[NORTH] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.s_to_e[i]);
								mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]]=USED;
								*mchip_t=mchip;
								return FOUND;								
							}
						}
					}
					// check north
					if(mchip.switch_grid[x][y].n_pins[i] != UNAVAIL) {
						if(y-1 >= 0 && x-1>=0) {
							if(mchip.logic_grid[x-1][y-1].pins[EAST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].n_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x-1>=0) {
							if(mchip.logic_grid[x-1][y].pins[WEST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].n_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}

					// check west
					if(mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]] != UNAVAIL) {
						if(y-1 >=0 && x-1 >=0){
							if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.s_to_w[i]);
								mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}
						if(y-1>=0 && x<size){
							if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.s_to_w[i]);
								mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}

					break;
				case EAST:

					// check west
					if(mchip.switch_grid[x][y].w_pins[i] != UNAVAIL) {
						if(y-1 >=0 && x-1 >=0){
							if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.s_to_w[i]);
								mchip.switch_grid[x][y].w_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}
						if(y-1>=0 && x<size){
							if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.s_to_w[i]);
								mchip.switch_grid[x][y].w_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}						
					}

					// check south
					if(mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]] != UNAVAIL) {
						if(y-1 >= 0 && x < size) {
							if(mchip.logic_grid[x][y-1].pins[EAST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.w_to_s[i]);
								mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x < size) {
							if(mchip.logic_grid[x][y].pins[WEST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.w_to_s[i]);
								mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					// check east
					if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL) {
						if(y-1 >=0 && x-1 >=0){
							if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].e_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}
						if(y-1>= 0 && x<size){
							if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].e_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					// check north
					if(mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]] != UNAVAIL) {
						if(x-1>=0 && y-1>=0){
							if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,w_map.w_to_n[i]);
								mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]]=USED;								
								*mchip_t=mchip;
								return FOUND;							
							} 
						}
						if(x-1>=0){
							if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,w_map.w_to_n[i]);
								mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]]=USED;
								*mchip_t=mchip;
								return FOUND;	
							}
						}
					}
					break;
				case SOUTH:

					//check north
					if(mchip.switch_grid[x][y].n_pins[i] != UNAVAIL) {
						if(y-1 >= 0 && x-1>=0) {
							if(mchip.logic_grid[x-1][y-1].pins[EAST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].n_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x-1>=0) {
							if(mchip.logic_grid[x-1][y].pins[WEST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].n_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}

					// check west
					if(mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]] != UNAVAIL) {
						if(y-1 >=0 && x-1 >=0){
							if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.n_to_w[i]);
								mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}
						if(y-1>= 0 && x<size){
							if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,w_map.n_to_w[i]);
								mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					// check south
					if(mchip.switch_grid[x][y].s_pins[i] != UNAVAIL) {
						if(x-1>=0 && y-1>=0){
							if(mchip.logic_grid[x][y-1].pins[EAST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].s_pins[i]=USED;								
								*mchip_t=mchip;
								return FOUND;							
							} 
						}
						if(x-1>=0){
							if(mchip.logic_grid[x][y].pins[WEST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].s_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;	
							}
						}
					}
					// check east
					if(mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]] != UNAVAIL) {
						if(x-1>=0) {
							if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET){
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.n_to_e[i]);
								mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}

						if(x<size){
							if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.n_to_e[i]);
								mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					break;
				case WEST:
									//DO NOT FOrGET

					// check east 
					if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL) {
						if(x-1>=0) {
							if(mchip.logic_grid[x-1][y].pins[SOUTH]==TARGET){
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.n_to_e[i]);
								mchip.switch_grid[x][y].e_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}

						if(x<size){
							if(mchip.logic_grid[x][y].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], e_pin[%d]\n",x,y,w_map.n_to_e[i]);
								mchip.switch_grid[x][y].e_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}						
					}

					// check north
					if(mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]] != UNAVAIL) {
						if(x-1>=0 && y-1>=0){
							if(mchip.logic_grid[x-1][y-1].pins[EAST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,w_map.e_to_n[i]);
								mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]]=USED;								
								*mchip_t=mchip;
								return FOUND;							
							} 
						}
						if(x-1>=0){
							if(mchip.logic_grid[x-1][y].pins[WEST]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], n_pin[%d]\n",x,y,w_map.e_to_n[i]);
								mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]]=USED;
								*mchip_t=mchip;
								return FOUND;	
							}
						}
					}
					// check west
					if(mchip.switch_grid[x][y].w_pins[i] != UNAVAIL) {
						if(x-1>=0 && y-1>=0) {
							if(mchip.logic_grid[x-1][y-1].pins[SOUTH]==TARGET){
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].w_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							} 
						}

						if(y-1>=0 && x<size){
							if(mchip.logic_grid[x][y-1].pins[NORTH]==TARGET) {
								printf("--FOUND @ sblock[%d][%d], w_pin[%d]\n",x,y,i);
								mchip.switch_grid[x][y].w_pins[i]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					// check south
					if(mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]] != UNAVAIL) {
						if(y-1 >= 0 && x < size) {
							if(mchip.logic_grid[x][y-1].pins[EAST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.e_to_s[i]);
								mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
						if(x < size) {
							if(mchip.logic_grid[x][y].pins[WEST] == TARGET) {
								printf("--FOUND @ sblock[%d][%d], s_pin[%d]\n",x,y,w_map.e_to_s[i]);
								mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]]=USED;
								*mchip_t=mchip;
								return FOUND;
							}
						}
					}
					break;
				default:
					printf("[ERR] Fatal Error Occured.\n");
					exit(-1);
			}
		}
	}
	*mchip_t=mchip;
	return NOT_FOUND;
}

struct elist init_elst(int size) {
	int size_sq=size*size;
	struct elist elst;
	elst.x=malloc(size_sq*sizeof(int));
	elst.y=malloc(size_sq*sizeof(int));
	elst.entered=malloc(size_sq*sizeof(int));
	return elst;
}

int add_to_queue(struct elist *elst_t, int x, int y, int src, int tail ) {
	struct elist elst;
	int i;
	elst=*elst_t;
	for(i=0 ; i<tail ; ++i) {
		if(x == elst.x[i] && y==elst.y[i]) {
			return tail;
		}
	}
	elst.x[tail]=x;
	elst.y[tail]=y;
	elst.entered[tail]=src;
	*elst_t=elst;
	tail++;
	return tail;
}

void search_s(struct chip *mchip_t, int x1, int y1, int src1, int x2, int y2, int src2) {

	int i, tail=0, curr=0, size, width, *costVec, x, y, heading, whereTo;
	int goN=0, goE=0, goW=0, goS=0;			//for fully-connected switches.
	int goN2=0, goE2=0, goW2=0, goS2=0;

	int flagN=0, flagS=0, flagE=0, flagW=0;	//for wilton switches.

	struct chip mchip;
	struct elist elst; // to hold the relevant searches in expansion list.
	struct wilton_switch w_map;

	mchip=*mchip_t;
	size=mchip.grid_size;
	width=mchip.width;
	w_map=mchip.switch_w;	// for wilton mapping; fully connected allows any pin to go to any pin (as long as it's avail);

	//init the elist;
	elst=init_elst(size);
	for(i=0; i<size*size;++i) {
		elst.x[i]=-1;
		elst.y[i]=-1;
		elst.entered[i]=-1;
	}
	printf("\n-- Init\n");

	tail=add_to_queue(&elst,x1,y1,src1,tail);
	tail=add_to_queue(&elst,x2,y2,src2,tail);

	printf("[[.-* .-* .-* .-* .-* .-* .-* .-*]]\n");

	while(curr < (size+1)*(size+1) && curr < tail) {
		goN=0;
		goE=0; 
		goW=0;
		goS=0;

		goN2=0;
		goE2=0;
		goW2=0;
		goS2=0;

		x=elst.x[curr];
		y=elst.y[curr];
		heading=elst.entered[curr];

		if(0==check_for_target(&mchip, x, y, heading)){
			printf("-- Found Path: Press Any Key to Traceback.\n\n");
			init_traceback(&mchip,x,y, heading);
			break;
		}
		switch(heading){

			case NORTH:
				printf("Entering From: [NORTH] @ sblock[%d][%d]\n",x,y);
				costVec=mchip.switch_grid[x][y].s_pins;
				break;
			case SOUTH:
				printf("Entering From: [SOUTH] @ sblock[%d][%d]\n",x,y);
				costVec=mchip.switch_grid[x][y].n_pins;
				break;
			case EAST:
				printf("Entering From: [EAST] @ sblock[%d][%d]\n",x,y);
				costVec=mchip.switch_grid[x][y].w_pins;
				break;
			case WEST:
				printf("Entering From: [WEST] @ sblock[%d][%d]\n",x,y);
				costVec=mchip.switch_grid[x][y].e_pins;
				break;
			default:
				printf("[ERR] Exiting. Came from unknown direction[%d]\n",heading);
				exit(-1);
				break;
		}
		for(i=0;i<width;++i){
			printf("[%d]",costVec[i]);
		}
		printf("\n");

		if(mchip.switch_type == 'f'){
			for(i=0;i<width;++i) {
				// go east
				if(y+1 <= size){
					if(mchip.switch_grid[x][y+1].w_pins[i] != UNAVAIL && mchip.switch_grid[x][y].e_pins[i] != UNAVAIL && heading != WEST) {
						if(mchip.switch_grid[x][y].e_pins[i]==INIT) {
							goW=2;
						}
						if(mchip.switch_grid[x][y+1].w_pins[i]==INIT) {
							goW2=1;
						}
					}
				}

				//go west
				if(y-1>=0) {
					if(mchip.switch_grid[x][y-1].e_pins[i] != UNAVAIL && mchip.switch_grid[x][y].w_pins[i]!= UNAVAIL && heading != EAST) {
						if(mchip.switch_grid[x][y].w_pins[i]==INIT) {
							goE=2;
						}
						if(mchip.switch_grid[x][y-1].e_pins[i]==INIT) {
							goE2=1;
						}
					}
				}

				//go south
				if(x+1 <=size) {
					if(mchip.switch_grid[x+1][y].n_pins[i]!= UNAVAIL && mchip.switch_grid[x][y].s_pins[i]!= UNAVAIL && heading != SOUTH) {
						if(mchip.switch_grid[x][y].s_pins[i]==INIT) {
							goS=2;
						}
						if(mchip.switch_grid[x+1][y].n_pins[i]==INIT) {
							goS2=1;
						}
					}
				}

				//go north
				if(x-1>=0) {
					if(mchip.switch_grid[x-1][y].s_pins[i] != UNAVAIL && mchip.switch_grid[x][y].n_pins[i]!= UNAVAIL && heading != NORTH) {
						if(mchip.switch_grid[x][y].n_pins[i]==INIT) {
							goN=2;
						}
						if(mchip.switch_grid[x-1][y].s_pins[i]==INIT) {
							goN2=1;
						}
					}
				}
			}
			//Continue Search...
			if(goN-1 == goN2) {
				tail=add_to_queue(&elst,x-1,y,NORTH,tail);
				for(i=0;i<width;++i) {
					mchip.switch_grid[x][y].n_pins[i]=costVec[i]+INC_COST;
					mchip.switch_grid[x-1][y].s_pins[i]=costVec[i]+INC_COST;
				}
			} 
			if(goS-1 == goS2) {
				tail=add_to_queue(&elst,x+1,y,SOUTH,tail);
				if(mchip.switch_type == 'f') {
					for(i=0;i<width;++i) {
						mchip.switch_grid[x][y].s_pins[i]=costVec[i]+INC_COST;
						mchip.switch_grid[x+1][y].n_pins[i]=costVec[i]+INC_COST;
					}
				} else {
					printf("..-*\n");

				}
			}
			if(goE-1 == goE2) {
				tail=add_to_queue(&elst,x,y-1,WEST,tail);
				if(mchip.switch_type == 'f') {
					for(i=0;i<width;++i) {
						mchip.switch_grid[x][y-1].e_pins[i]=costVec[i]+INC_COST;
						mchip.switch_grid[x][y].w_pins[i]=costVec[i]+INC_COST;
					}
				} else {
					printf("..-*\n");
				}
			}
			if(goW-1 == goW2) {
				tail=add_to_queue(&elst,x,y+1,EAST,tail);
				if(mchip.switch_type == 'f') {
					for(i=0;i<width;++i) {
						mchip.switch_grid[x][y+1].w_pins[i]=costVec[i]+INC_COST;
						mchip.switch_grid[x][y].e_pins[i]=costVec[i]+INC_COST;
					}
				} else {
					printf("..-*\n");
				}
			}
		} else {
			for(i=0; i<width ; ++i) {
				switch(heading){
					case SOUTH:
						// head east 
						if(mchip.switch_grid[x][y].n_pins[i] != UNAVAIL) {
							if(y+1 <= size){
								if((mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]] == INIT ) && mchip.switch_grid[x][y+1].w_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].e_pins[w_map.n_to_e[i]]=costVec[i]+INC_COST;
									mchip.switch_grid[x][y+1].w_pins[i]=costVec[i]+INC_COST;
									flagE=1;
								}
							}
							// head west
							if(y-1>=0) {
								if((mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]] == INIT ) && mchip.switch_grid[x][y-1].e_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].w_pins[w_map.n_to_w[i]]=costVec[i]+INC_COST;
									mchip.switch_grid[x][y-1].e_pins[i]=costVec[i]+INC_COST;
									flagW=1;
								}
							}
							// head south
							if(x-1>=0) {
								if((mchip.switch_grid[x][y].s_pins[i] == INIT) && mchip.switch_grid[x+1][y].s_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].s_pins[i]=costVec[i]+INC_COST;
									mchip.switch_grid[x+1][y].n_pins[i]=costVec[i]+INC_COST;
									flagN=1;
								}
							}
						}
						break;

					case NORTH:
						if(mchip.switch_grid[x][y].s_pins[i] != UNAVAIL) {
							// head east 
							if(y+1 <= size){
								if((mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]] == INIT) && mchip.switch_grid[x][y+1].w_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].e_pins[w_map.s_to_e[i]]=costVec[i]+INC_COST;
									mchip.switch_grid[x][y+1].w_pins[i]=costVec[i]+INC_COST;
									flagE=1;
								}
							}
							// head west
							if(y-1>=0) {
								if((mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]] == INIT) && mchip.switch_grid[x][y-1].e_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].w_pins[w_map.s_to_w[i]]=costVec[i] +INC_COST;
									mchip.switch_grid[x][y-1].e_pins[i]=costVec[i]+INC_COST;
									flagW=1;
								}
							}
							// head north
							if(x-1>=0) {
								if((mchip.switch_grid[x][y].n_pins[i] == INIT) && mchip.switch_grid[x-1][y].s_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].n_pins[i]=costVec[i]+INC_COST;
									mchip.switch_grid[x-1][y].s_pins[i]=costVec[i]+INC_COST;
									flagS=1;
								} 
							}
						}
						break;
					case EAST:
						if(mchip.switch_grid[x][y].w_pins[i] != UNAVAIL) {
							// head north
							if(x-1>=0) {
								if((mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]] == INIT) && mchip.switch_grid[x-1][y].s_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].n_pins[w_map.w_to_n[i]]=costVec[i] + INC_COST;
									mchip.switch_grid[x-1][y].s_pins[i]=costVec[i] +INC_COST;
									flagN=1;
								}
							}
							// head south
							if(x+1 <=size) {
								if((mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]] == INIT) && mchip.switch_grid[x+1][y].n_pins[i] !=UNAVAIL) {
									mchip.switch_grid[x][y].s_pins[w_map.w_to_s[i]]=costVec[i] + INC_COST;
									mchip.switch_grid[x+1][y].n_pins[i]=costVec[i] + INC_COST;
									flagS=1;
								}
							}
							// head east
							if(y+1 <=size) {
								if((mchip.switch_grid[x][y].e_pins[i] == INIT || mchip.switch_grid[x][y].e_pins[i] > costVec[i] +INC_COST) && mchip.switch_grid[x][y+1].w_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].e_pins[i]=costVec[i] + INC_COST;
									mchip.switch_grid[x][y+1].w_pins[i] = costVec[i] + INC_COST;
									flagW=1;
								}
							}
						}
						break;
					case WEST:
						if(mchip.switch_grid[x][y].e_pins[i] != UNAVAIL) {
							// head north
							if(x-1>=0) {
								if((mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]] == INIT) && mchip.switch_grid[x-1][y].s_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].n_pins[w_map.e_to_n[i]]=costVec[i]+INC_COST;
									mchip.switch_grid[x-1][y].s_pins[i]=costVec[i]+INC_COST;
									flagN=1;
								}
							}

							// head south
							if(x+1 <=size) {
								if((mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]] == INIT) && mchip.switch_grid[x+1][y].n_pins[i] != UNAVAIL ) {
									mchip.switch_grid[x][y].s_pins[w_map.e_to_s[i]]=costVec[i]+INC_COST;
									mchip.switch_grid[x+1][y].n_pins[i]=costVec[i]+INC_COST;
									flagS=1;
								}
							}
							// head west 
							if(y+1 <= size){
								if((mchip.switch_grid[x][y].w_pins[i] == INIT) && mchip.switch_grid[x][y-1].e_pins[i] != UNAVAIL) {
									mchip.switch_grid[x][y].w_pins[i]=costVec[i] + INC_COST;
									mchip.switch_grid[x][y-1].e_pins[i]=costVec[i] + INC_COST;
									flagE=1;
								}
							}
						}
						break;
				}
			}
			if(flagN == 1) {
				tail=add_to_queue(&elst,x-1,y,NORTH,tail);
				flagN=0;
			}
			if(flagS == 1) {
				tail=add_to_queue(&elst,x+1,y,SOUTH,tail);
				flagS=0;
			}
			if(flagE == 1) {
				tail=add_to_queue(&elst,x,y+1,EAST,tail);
				flagE=0;
			}
			if(flagW == 1) {
				tail=add_to_queue(&elst,x,y-1,WEST,tail);
				flagW=0;
			}
		}
		++curr;
	}
	free(elst.x);
	free(elst.y);
	free(elst.entered);

	*mchip_t=mchip;
}

void search(struct chip *mchip_t, int x, int y, int heading) {

}

void begin_search(struct chip *mchip_t, int src_x, int src_y, int src_pin, char is_parallel) {
	int i=0;
	int noneAvail=0;
	struct chip mchip;
	mchip=*mchip_t;
	int width=mchip.width;
	int size=mchip.grid_size;
	// Set up threads
	// pthread_t threads[2];
	// void *status;

	printf("[INFO] Beginning Search @ L-Block[%d][%d]\n",src_x, src_y);
	switch(src_pin) {
		case NORTH:
				printf("[INFO] Start @ NORTH\n");
				for(int i=0; i<width; ++i){
					if(src_y+1 <=size && mchip.switch_grid[src_x][src_y].e_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].w_pins[i] !=UNAVAIL) {
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
					} else {
						search_s(&mchip, src_x, src_y, WEST, src_x, src_y+1, EAST);
					}
				}
			break;
		case EAST:
				printf("[INFO] Start @ EAST\n");
				for(int i=0; i<width; ++i){
					if(src_x+1 <= size && src_y+1 <=size && mchip.switch_grid[src_x+1][src_y+1].n_pins[i] != UNAVAIL && mchip.switch_grid[src_x][src_y+1].s_pins[i] !=UNAVAIL) {
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
					} else {
						search_s(&mchip, src_x+1, src_y+1, SOUTH, src_x, src_y+1, NORTH);
					}
				}
			break;
		case SOUTH:
				printf("[INFO] Start @ SOUTH\n");
				for(int i=0; i<width; ++i) {
					if(src_x+1 <= size && src_y+1 <=size && mchip.switch_grid[src_x+1][src_y].e_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y+1].w_pins[i] !=UNAVAIL) {
						mchip.switch_grid[src_x+1][src_y+1].e_pins[i]=0;
						mchip.switch_grid[src_x+1][src_y].w_pins[i]=0;
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
					} else {
						search_s(&mchip, src_x+1, src_y, EAST, src_x+1, src_y+1, WEST);
					}
				}
			break;
		case WEST:
				printf("[INFO] Start @ WEST\n");
				for(int i=0; i<width; ++i){
					if(src_x+1 <= size && mchip.switch_grid[src_x][src_y].s_pins[i] != UNAVAIL && mchip.switch_grid[src_x+1][src_y].n_pins[i] !=UNAVAIL) {
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
					} else {
						search_s(&mchip, src_x, src_y, NORTH, src_x+1, src_y, SOUTH);
					}
				}
			break;
	}
	*mchip_t=mchip;
}

void reset_router(struct chip *mchip_t) {
	printf("-- Resetting Router for next Route.\n");
	PATH_FOUND=0;
	reset_connections(mchip_t);
	reset_reqs(mchip_t);
}

int route_path(struct chip *mchip_t, int src_x, int src_y, int src_pin, int targ_x, int targ_y, int targ_pin, char is_parallel) {

	printf("[INFO] --Beginning Path Search\n");
	begin_search(mchip_t, src_x,src_y,src_pin, is_parallel);
	
	return PATH_FOUND;
}