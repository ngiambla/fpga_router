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

		void display_config() {
			int i;
			printf("** *** ***** [config] ***** *** **\n");
			printf("[size]    [%d]x[%d]\n", size, size);
			printf("[width]   [%d]\n",width);
			for(i=0; i<netlist.size(); ++ i){
				printf("[INFO] Net[%d] -- LBlck(%d,%d) Pin[%d] <-> LBlck(%d,%d) Pin[%d]\n", i, netlist[i][0], netlist[i][1], netlist[i][2], netlist[i][3], netlist[i][4], netlist[i][5]);
			}
			printf("-- End Of Config.\n");
		}
};


#endif