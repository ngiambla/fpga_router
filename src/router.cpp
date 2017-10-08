
#include "router.h"
#include <thread>
#include <mutex>

mutex search_mut;

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
	int min_pin_weight, cur_pin=pin, next_side;
	int cur_x=x, cur_y=y, cur_side=side;
	int found_src=0;
	vector<Sblck> cur_path;
	vector<Path> cur_path_p;
	
	min_pin_weight=MAX_WEIGHT;

	while(found_src==0) {
		Sblck blck=c.get_switch(cur_x,cur_y);
		if(add_block_to_path(cur_path, blck)==0) {
			printf("[ERR] -- encountered this block already.\n");
			exit(-1);
			break;
		} else {
			Path p(cur_pin, cur_side, blck);
			cur_path_p.push_back(p);
			blck.display_id();
			blck.display_block();
		}

		printf("[INFO] @Sblock[%d][%d] **\n    Heading Back <<<<-- from [%d]\n", cur_x, cur_y, cur_side);

		for(dir=0; dir<4; ++dir) {
			for(i=0; i<c.get_width(); ++i) {
				if(blck.is_side_avail(dir)==1) {
					if(blck.get_pin(dir, cur_side, i) == 0) {
						
						printf("[INFO] Source discovered. Saving Path.\n");
						blck.set_pin(dir, cur_side, i, UNAVAIL);
						found_src=1;
						goto FOUND_SRC;

					} else if(blck.get_pin(dir, cur_side, i) > 0 && blck.is_side_avail(dir) == 1) {
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
				cout << "[NORTH]\n";
				cur_x=cur_x-1;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_pin=blck.get_pin_pos(next_side, cur_side, cur_pin);
				cur_side=SOUTH;
				break;
			case EAST:
				cout << "[EAST]\n";
				cur_y=cur_y+1; 
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_pin=blck.get_pin_pos(next_side, cur_side, cur_pin);
				cur_side=WEST;
				break;
			case SOUTH:
				cout << "[SOUTH]\n";
				cur_x=cur_x+1;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_pin=blck.get_pin_pos(next_side, cur_side, cur_pin);
				cur_side=NORTH;
				break;
			case WEST:
				cout << "[WEST]\n";
				cur_y=cur_y-1;
				blck.set_pin(next_side, cur_side, cur_pin, UNAVAIL);
				cur_pin=blck.get_pin_pos(next_side, cur_side, cur_pin);
				cur_side=EAST;
				break;
		}
	}

	FOUND_SRC:
		all_paths.push_back(cur_path_p);
}

void Router::begin_traceback(Circuit &c, int x, int y, int came_from) {
	int i, j, side;
	Sblck t_src=c.get_switch(x,y);
	switch(came_from) {
		case NORTH:
			side=SOUTH;
			break;
		case EAST:
			side=WEST;
			break;
		case SOUTH:
			side=NORTH;
			break;
		case WEST:
			side=EAST;
			break;
	}

	for(i=0;i<c.get_size();++i){
		for(j=0;j<4;++j) {
			if(t_src.is_side_avail(j)==1) {
				if(t_src.get_pin(j, side, i) == USED) {
					t_src.set_pin(j, side, i, UNAVAIL);
					traceback(c, x, y, i, j);
					i=MAX_WEIGHT;
					j=MAX_WEIGHT;
					break;
				}
			}
		}	
	}
}

int Router::check_for_target(Circuit &c, int x, int y, int came_from, int HEAD) {
	int i, j, side;
	vector<Path> cur_path;

	Sblck p_trg=c.get_switch(x,y);
	p_trg.display_block();

	switch(came_from) {
		case NORTH:
			side=SOUTH;
			break;
		case EAST:
			side=WEST;
			break;
		case SOUTH:
			side=NORTH;
			break;
		case WEST:
			side=EAST;
			break;
	}

	for(i=0; i<c.get_width();++i) {
		for(j=0; j<4; ++j){
			if(p_trg.is_side_avail(j)==1) {
				if(p_trg.get_pin(j, side, i) == TARGET && (p_trg.get_pin(side, i)>=0 || p_trg.get_pin(side, i)== TARGET)) {
					if(HEAD==0){
						p_trg.set_pin(j, side, i, UNAVAIL);
						target_hit=1;
						Path p(i, j, p_trg);
						cur_path.push_back(p);
						all_paths.push_back(cur_path);
						return 1;

					} else {
						p_trg.set_pin(j, side, i, USED);
						target_hit=1;
						return 1;
					}
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

		if(check_for_target(c, cur_x, cur_y, cur_heading, HEAD) == 1) {
			printf("[SEARCH] --Target Acquired.\n");
			if(HEAD != 0) {
				begin_traceback(c, cur_x, cur_y, cur_heading);
			}
			break;
		}

		Sblck s_t = c.get_switch(cur_x, cur_y);

		for(i=0; i< c.get_width(); ++i) {
			switch(cur_heading){
				case NORTH:
					if(s_t.get_pin(SOUTH, NORTH, i) != UNAVAIL) {
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
					if(s_t.get_pin(WEST, EAST, i) != UNAVAIL) {
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
					if(s_t.get_pin(NORTH, SOUTH, i) != UNAVAIL) {
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
					if(s_t.get_pin(EAST, WEST, i) != UNAVAIL) {
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

void Router::search_p(Circuit &c, int x1, int y1, int heading1) {
	// fill in today.
}

void Router::begin_search(Circuit &c, int x, int y, int init_dir) {
	printf("[INFO] -- Beginning Search.\n");

	switch(init_dir) {
		case NORTH:
			if(is_parallel == 'F'){
				search(c, x, y, WEST, x, y+1, EAST);
			} else {
				thread st1= thread(&Router::search_p, this, ref(c), x, y, WEST);
				thread st2= thread(&Router::search_p, this, ref(c), x, y+1, EAST);

				st1.join();
				st2.join();
			}
			break;
		case EAST:
			if(is_parallel == 'F') {
				search(c, x, y+1, NORTH, x+1, y+1, SOUTH);
			} else {
				thread st1= thread(&Router::search_p, this, ref(c), x, y+1, NORTH);
				thread st2= thread(&Router::search_p, this, ref(c), x+1, y+1, SOUTH);

				st1.join();
				st2.join();
			}
			break;
		case SOUTH:
			if(is_parallel == 'F') {
				search(c, x+1, y, WEST, x+1, y+1, EAST);
			} else {
				thread st1= thread(&Router::search_p, this, ref(c), x+1, y, WEST);
				thread st2= thread(&Router::search_p, this, ref(c), x+1, y+1, EAST);

				st1.join();
				st2.join();
			}
			break;
		case WEST:
			if(is_parallel == 'F') {
				search(c, x, y, NORTH, x+1, y, SOUTH);
			} else {
				thread st1= thread(&Router::search_p, this, ref(c), x, y, NORTH);
				thread st2= thread(&Router::search_p, this, ref(c), x+1, y, SOUTH);

				st1.join();
				st2.join();
			}
			break;
		default:
			throw "Incorrect Direction";
	}

}

Paths_t Router::begin_routing(Circuit &c) {

	int cur_net=0;
	printf("[INFO] -- Routing has begun ..-*\n\n");
	printf("[INFO] Routing [%d] nets.\n", netlist.size());
	for(vector<int> net : netlist) {

		Lblck lsrc = c.get_lblck(net[0], net[1]);
		lsrc.set_as_source(net[2]-1);

		Lblck ltrg = c.get_lblck(net[3], net[4]);
		ltrg.set_as_target(net[5]-1);
		printf("[router] Routing lblck[%d][%d]@[%d] --> lblck[%d][%d]@[%d]\n", net[0],net[1],net[2]-1,net[3],net[4],net[5]-1);

		begin_search(c, net[0], net[1], net[2]-1);

		if(all_paths.size() == cur_net && all_paths.size() > 0) {
			// printf("###### Path [%d] ######\n", cur_net);
			// for(Sblck ss : complete_paths[cur_net]) {
			// 	ss.display_id();
			// 	ss.display_block();
			// }
			// printf("### End of Path ###\n");
		}

		if(target_hit!=1) {
			printf("No Target Found.\n");
			if(is_parallel != 'T')
				exit(-1);
		} else {
			target_hit=0;
		}
		c.reset();
		++cur_net;
	}
	c.compute_stats();
	return all_paths;
}