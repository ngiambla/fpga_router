#ifndef __CCONFIG_H_INCLUDED__
#define __CCONFIG_H_INCLUDED__

#include "cirdefs.h"

class CConfig {
	private:
		int width;
		int size;
		vector< vector<int> > netlist; 

	public:

		void set_track_width(int width) {
			this->width=width;
		}

		int get_track_width() {
			return width;
		}

		void set_grid_size(int size) {
			this->size =size;
		}

		int get_grid_size() {
			return size;
		}

		void add_net(vector<int> net) {
			netlist.push_back(net);
		} 

		vector< vector<int> > get_netlist() {
			return netlist;
		}

		void reorder_nets();

		void display_config();
};


#endif