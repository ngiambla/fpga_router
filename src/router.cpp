
#include "router.h"

void Router::begin_traceback(Circuit &c, int x, int y, int came_from) {

}

int Router::check_for_target(Circuit &c, int x, int y) {
	int i;
	Sblck p_trg=c.get_switch(x,y);
	for(i=0; i<c.get_width();++i) {
		if(p_trg.is_side_avail(NORTH)==1)
			if(p_trg.get_pin(NORTH, i) == TARGET) {
				target_hit=1;
				return 1;
			}
		if(p_trg.is_side_avail(EAST)==1)
			if(p_trg.get_pin(EAST, i) == TARGET) {
				target_hit=1;
				return 1;
			}
		if(p_trg.is_side_avail(SOUTH)==1)
			if(p_trg.get_pin(SOUTH, i) == TARGET) {
				target_hit=1;
				return 1;
			}
		if(p_trg.is_side_avail(WEST)==1)
			if(p_trg.get_pin(WEST, i) == TARGET) {
				target_hit=1;
				return 1;
			}	
	}
	return 0;
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
		if(check_for_target(c, cur_x, cur_y) == 1) {
			printf("[SEARCH] --Target Acquired.\n");
			begin_traceback(c, cur_x, cur_y, cur_heading);
			break;
		}
		Sblck s_t = c.get_switch(cur_x, cur_y);

		for(i=0; i< c.get_width(); ++i){
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
			sblcks_x.push_back(cur_x-1);
			sblcks_y.push_back(cur_y);
			going.push_back(NORTH);
			nflag=0;
		} 
		if(sflag==1) {
			sblcks_x.push_back(cur_x+1);
			sblcks_y.push_back(cur_y);
			going.push_back(SOUTH);
			sflag=0;
		}
		if(eflag==1) {
			sblcks_x.push_back(cur_x);
			sblcks_y.push_back(cur_y+1);
			going.push_back(EAST);	
			eflag=0;	
		}
		if(wflag==1) {
			sblcks_x.push_back(cur_x);
			sblcks_y.push_back(cur_y-1);
			going.push_back(WEST);
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

	for(vector<int> net : netlist) {

		Lblck lsrc = c.get_lblck(net[0], net[1]);
		lsrc.set_as_source(net[2]-1);

		Lblck ltrg = c.get_lblck(net[3], net[4]);
		ltrg.set_as_target(net[5]-1);

		begin_search(c, net[0], net[1], net[2]-1);

		c.reset();

	}

}