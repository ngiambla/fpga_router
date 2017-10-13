
#include "cconfig.h"
#include <cmath>
#include <time.h>       /* time */

int get_squared_distance(int x1, int y1, int x2, int y2) {
	return pow((x2-x1), 2)+ pow((y2-y1), 2);
}

void CConfig::reorder_nets() {
	srand (time(NULL));
	int rand_idx, i;
	vector< vector<int> > new_net;
	vector<int> idx_swaps;
	printf("[cconfig] -- reordering nets.\n");
	while(idx_swaps.size() < netlist.size()) {
		GEN_RAND:
			rand_idx = rand() % netlist.size();
			for(i=0; i<idx_swaps.size(); ++i) {
				if(idx_swaps[i]==rand_idx) {
					goto GEN_RAND;
				}
			}
			idx_swaps.push_back(rand_idx);
	}
	for(i=0; i<idx_swaps.size(); ++i) {
		new_net.push_back(netlist[idx_swaps[i]]);
	}
	netlist=new_net;
}

void CConfig::display_config() {
	int i;
	printf("** *** ***** [config] ***** *** **\n");
	printf("[size]    [%d]x[%d]\n", size, size);
	printf("[width]   [%d]\n",width);
	for(i=0; i<netlist.size(); ++ i){
		printf("[INFO] Net[%d] -- LBlck(%d,%d) Pin[%d] <-> LBlck(%d,%d) Pin[%d]\n", i, netlist[i][0], netlist[i][1], netlist[i][2], netlist[i][3], netlist[i][4], netlist[i][5]);
	}
	printf("-- End Of Config.\n");
}