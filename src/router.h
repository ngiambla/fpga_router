#ifndef __ROUTER_H_INCLUDED__
#define __ROUTER_H_INCLUDED__

#include "cirdefs.h"
#include "circuit.h"
#include "sblck.h"

typedef vector<Sblck> Spath;
typedef vector<Spath> Spaths;

class Router {
	private:

		vector< vector<int> > netlist;
		Spaths complete_paths;
		int target_hit;
		void add_to_queue(vector<int> &_x, vector<int> &_y, vector<int> &_g, int x1, int y1, int dir);
		void begin_search(Circuit &c, int x, int y, int init_dir);
		void search(Circuit &c, int x1, int y1, int heading1, int x2, int y2, int heading2);
		void begin_traceback(Circuit &c, int x, int y, int came_from);
		void traceback(Circuit &c, int x, int y, int pin, int side);
		int check_for_target(Circuit &c, int x, int y, int came_from);
	public:
		Router(vector< vector<int> > netlist){
			this->netlist=netlist;
			target_hit=0;
		}
		int begin_routing(Circuit &c);
};


#endif
