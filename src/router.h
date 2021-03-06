#ifndef __ROUTER_H_INCLUDED__
#define __ROUTER_H_INCLUDED__

#include "cirdefs.h"
#include "circuit.h"
#include "sblck.h"
#include "path.h"

typedef vector<Sblck> Spath;

typedef vector<Path> Path_t;
typedef vector<Path_t> Paths_t;

class Router {
	private:

		vector< vector<int> > netlist;	// holds current netlist.
		Paths_t all_paths;				// holds paths.

		vector<int> sblcks_x_p;	
		vector<int> sblcks_y_p;
		vector<int> going_p;
		int src_hit;
		int target_hit;
		int traceback_started;
		
		char is_parallel;

		int north_search_count;
		int east_search_count;
		int south_search_count;
		int west_search_count;

		
		void add_to_queue(vector<int> &_x, vector<int> &_y, vector<int> &_g, int x1, int y1, int dir);
		void begin_search(Circuit &c, int x, int y, int init_dir);
		void search(Circuit &c, int x1, int y1, int heading1, int x2, int y2, int heading2);
		void thr_search(Circuit &c, int x1, int y1, int heading1, int id);		
		void traceback(Circuit &c, int x, int y, int pin, int side, int weight);
		int check_for_target(Circuit &c, int x, int y, int came_from, int HEAD);

	public:

		Router(vector< vector<int> > netlist, char is_parallel){
			this->netlist=netlist;
			this->is_parallel = is_parallel;
			this->target_hit=0;
			this->src_hit=0;
			this->traceback_started=0;

			north_search_count=0;
			east_search_count=0;
			south_search_count=0;
			west_search_count=0;

		}
		
		Paths_t begin_routing(Circuit &c);
		int should_search_side(int side);			// proposed improvement
		void update_side_likelihood(int nflag, int eflag, int sflag, int wflag);		//
		int was_routable();
};


#endif
