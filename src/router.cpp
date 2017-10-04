
#include "router.h"

int add_block_to_path(Spath &cur_path, Sblck sblck) {
	for(Sblck s : cur_path) {
		if(sblck.get_x() == s.get_x() && sblck.get_y() == s.get_y()) {
			return 0;
		}
	}
	cur_path.push_back(sblck);
	return 1;
}

void Router::traceback(Circuit &c, int x, int y, int pin, int side) {
	int i, dir;
	int min_pin_weight, cur_pin, next_side;
	int cur_x=x, cur_y=y, cur_side=side;
	int found_src=0;
	vector<Sblck> cur_path;

	c.get_switch(3,2).display_block();

	while(found_src==0) {

		min_pin_weight=MAX_WEIGHT;
		Sblck blck=c.get_switch(cur_x,cur_y);

		if(add_block_to_path(cur_path, blck)==0) {
			printf("[ERR] -- encountered this block already.\n");
			break;
		}

		printf("[INFO] @Sblock[%d][%d] **\n    Heading Back <<<<-- from [%d]\n", cur_x, cur_y, cur_side);

		for(dir=0; dir<4; ++dir) {
			for(i=0; i<c.get_width(); ++i) {
				if(blck.is_side_avail(dir)==1) {
					if(blck.get_pin(dir, cur_side, i) == 0) {
						printf("FOUND_SRC\n");
						found_src=1;
						goto FOUND_SRC;
					} else if(blck.get_pin(dir, cur_side, i) > 0) {
						if(blck.get_pin(dir, cur_side, i) < min_pin_weight) {
							min_pin_weight=blck.get_pin(dir, cur_side, i);
							cur_pin=i;
							next_side=dir;
						}
					}
				}
			}
		}

	
		switch(next_side) {
			case NORTH:
				cur_x=cur_x+1;
				cur_y=cur_y;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_side=SOUTH;
				break;
			case EAST:
				cur_x=cur_x;
				cur_y=cur_y-1; 
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_side=WEST;
				break;
			case SOUTH:
				cur_x=cur_x-1;
				cur_y=cur_y;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_side=NORTH;
				break;
			case WEST:
				cur_x=cur_x;
				cur_y=cur_y+1;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_side=EAST;
				break;
		}
	}

	if(cur_path.size() > 0) {
		complete_paths.push_back(cur_path);
	} else {
		printf("--Found nothing????\n");
	}
	FOUND_SRC:
		printf("here...");
}

void Router::begin_traceback(Circuit &c, int x, int y, int came_from) {
	int i, j;
	Sblck t_src=c.get_switch(x,y);
	for(i=0;i<c.get_size();++i){
		for(j=0;j<4;++j) {
			if(t_src.is_side_avail(j)==1) {
				if(t_src.get_pin(j, came_from, i) == USED) {
					t_src.set_pin(j, came_from, i, UNAVAIL);					
					traceback(c, x, y, i, j);
				}
			}
		}	
	}
}

int Router::check_for_target(Circuit &c, int x, int y, int came_from) {
	int i, j;
	Sblck p_trg=c.get_switch(x,y);
	for(i=0; i<c.get_width();++i) {
		for(j=0; j<4; ++j){
			if(p_trg.is_side_avail(j)==1) {
				if(p_trg.get_pin(j, came_from, i) == TARGET) {
					p_trg.set_pin(j, came_from, i, USED);
					target_hit=1;
					return 1;
				}
			}
		}	
	}
	return 0;
}

void Router::add_to_queue(vector<int> &_x, vector<int> &_y, vector<int> &_g, int x1, int y1, int dir) {
	int i=0, append=0;
	for(i=0; i<_x.size();++i) {
		if(_x[i]== x1 && _y[i]==y1) {
			append=1;
			break;
		}
	}
	if(append==0) {
		_x.push_back(x1);
		_y.push_back(y1);
		_g.push_back(dir);
	}
}

void Router::search(Circuit &c, int x1, int y1, int heading1, int x2, int y2, int heading2) {

	int HEAD=0, i=0, cur_x, cur_y, cur_heading;
	char placeholder;
	int eflag=0, nflag=0, sflag=0, wflag=0;

	vector<int> sblcks_x;
	vector<int> sblcks_y;
	vector<int> going;

	sblcks_x.push_back(x1);
	sblcks_y.push_back(y1);
	going.push_back(heading1);

	sblcks_x.push_back(x2);
	sblcks_y.push_back(y2);
	going.push_back(heading2);


	while(HEAD < sblcks_x.size()) {

		cur_x=sblcks_x[HEAD];
		cur_y=sblcks_y[HEAD];
		cur_heading=going[HEAD];
		printf("[router] -- inspecting [%d][%d], heading [%d]\n", cur_x, cur_y, cur_heading);

		if(check_for_target(c, cur_x, cur_y, cur_heading) == 1) {
			printf("[SEARCH] --Target Acquired.\n");
			if(HEAD ==0 ) {
				printf("-- on the same line\n");
			} else {
				
			}
			//begin_traceback(c, cur_x, cur_y, cur_heading);

			cin.ignore();
			break;
		}

		Sblck s_t = c.get_switch(cur_x, cur_y);

		for(i=0; i< c.get_width(); ++i) {
			switch(cur_heading){
				case NORTH:
					if(s_t.get_pin(SOUTH, i) != UNAVAIL) {
						if(s_t.is_side_avail(NORTH)==1) {
							s_t.set_switch(NORTH, SOUTH, i);
							nflag=1;
						}
						if(s_t.is_side_avail(EAST)==1) {
							s_t.set_switch(EAST, SOUTH, i);
							eflag=1;
						}
						if(s_t.is_side_avail(WEST)==1) {
							s_t.set_switch(WEST, SOUTH, i);
							wflag=1;
						}
					}
					break;
				case EAST:
					if(s_t.get_pin(WEST, i) != UNAVAIL) {
						if(s_t.is_side_avail(NORTH)==1) {
							s_t.set_switch(NORTH, WEST, i);
							nflag=1;
						}
						if(s_t.is_side_avail(EAST)==1) {
							s_t.set_switch(EAST, WEST, i);
							eflag=1;
						}
						if(s_t.is_side_avail(SOUTH)==1) {
							s_t.set_switch(SOUTH, WEST, i);
							sflag=1;						
						}
					}
					break;
				case SOUTH:
					if(s_t.get_pin(NORTH, i) != UNAVAIL) {
						if(s_t.is_side_avail(SOUTH)==1) {
							s_t.set_switch(SOUTH, NORTH, i);
							sflag=1;
						}
						if(s_t.is_side_avail(EAST)==1) {
							s_t.set_switch(EAST, NORTH, i);
							eflag=1;						
						}
						if(s_t.is_side_avail(WEST)==1) {
							s_t.set_switch(WEST, NORTH, i);
							wflag=1;						
						}
					}
					break;
				case WEST:
					if(s_t.get_pin(EAST, i) != UNAVAIL) {
						if(s_t.is_side_avail(NORTH)==1) {
							s_t.set_switch(NORTH, EAST, i);
							nflag=1;						
						}
						if(s_t.is_side_avail(SOUTH)==1) {
							s_t.set_switch(SOUTH, EAST, i);
							sflag=1;												
						}
						if(s_t.is_side_avail(WEST)==1) {
							s_t.set_switch(WEST, EAST, i);
							wflag=1;						
						}
					}
					break;
			}
		}
		if(nflag==1) {
			add_to_queue(sblcks_x, sblcks_y, going, cur_x-1, cur_y, NORTH);
			nflag=0;
		} 
		if(sflag==1) {
			add_to_queue(sblcks_x, sblcks_y, going, cur_x+1, cur_y, SOUTH);
			sflag=0;
		}
		if(eflag==1) {
			add_to_queue(sblcks_x, sblcks_y, going, cur_x, cur_y+1, EAST);
			eflag=0;	
		}
		if(wflag==1) {
			add_to_queue(sblcks_x, sblcks_y, going, cur_x, cur_y-1, WEST);
			wflag=0;			
		}
		++HEAD;
	}
}

void Router::begin_search(Circuit &c, int x, int y, int init_dir) {
	printf("[INFO] -- Beginning Search.\n");
	switch(init_dir) {
		case NORTH:
			search(c, x, y, WEST, x, y+1, EAST);
			break;
		case EAST:
			search(c, x, y+1, NORTH, x+1, y+1, SOUTH);
			break;
		case SOUTH:
			search(c, x+1, y, WEST, x+1, y+1, EAST);
			break;
		case WEST:
			search(c, x, y, NORTH, x+1, y, SOUTH);
			break;
		default:
			throw "Incorrect Direction";
	}

}

int Router::begin_routing(Circuit &c) {

	printf("[INFO] -- Routing has begun ..-*\n\n");
	printf("[INFO] Routing [%d] nets.\n", netlist.size());
	for(vector<int> net : netlist) {

		Lblck lsrc = c.get_lblck(net[0], net[1]);
		lsrc.set_as_source(net[2]-1);

		Lblck ltrg = c.get_lblck(net[3], net[4]);
		ltrg.set_as_target(net[5]-1);

		printf("[router] Routing lblck[%d][%d]@[%d] --> lblck[%d][%d]@[%d]\n", net[0],net[1],net[2],net[3],net[4],net[5]);

		begin_search(c, net[0], net[1], net[2]-1);
		if(target_hit!=1) {
			printf("No Target Found.\n");
			exit(-1);
		} else {
			target_hit=0;
		}
		c.reset();
	}
	c.compute_stats();

}