
#include "router.h"

void Router::begin_traceback(Circuit c, int x, int y, int came_from) {

}

int Router::check_for_target(Circuit c, int x, int y) {
	int i;
	Sblck p_trg=c.get_switch(x,y);
	for(i=0; i<c.get_width();++i) {
		if(p_trg.get_pin(NORTH, i) == TARGET) {
			target_hit=1;
			return 1;
		}
		if(p_trg.get_pin(EAST, i) == TARGET) {
			target_hit=1;
			return 1;
		}
		if(p_trg.get_pin(SOUTH, i) == TARGET) {
			target_hit=1;
			return 1;
		}
		if(p_trg.get_pin(WEST, i) == TARGET) {
			target_hit=1;
			return 1;
		}	
	}
	return 0;
}

void Router::search(Circuit c, int x1, int y1, int heading1, int x2, int y2, int heading2) {
	int HEAD=0, cur_x, cur_y, cur_heading;

	vector<int> sblcks_x;
	vector<int> sblcks_y;
	vector<int> going;

	sblcks_x.push_back(x1);
	sblcks_y.push_back(y1);
	going.push_back(heading1);

	sblcks_x.push_back(x2);
	sblcks_y.push_back(y2);
	going.push_back(heading2);


	for(int i : sblcks_x) {
		cur_x=sblcks_x[HEAD];
		cur_y=sblcks_y[HEAD];
		cur_heading=going[HEAD];
		if(check_for_target(c, cur_x, cur_y) == 1) {
			printf("[SEARCH] --Target Acquired.\n");
			begin_traceback(c, cur_x, cur_y, cur_heading);
			break;
		}
	}
}

void Router::begin_search(Circuit c, int x, int y, int init_dir) {
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

	}

}