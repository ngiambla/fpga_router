#include "cirdefs.h"
#include "circuit.h"
#include "cconfig.h"
#include <fstream>
#include <stdio.h>

const char * usage="Usage ./cirutils -file filename -switch [f | w] -isParallel [T | F]\n";


CConfig init_util(char * filename) {
	CConfig config;
    FILE *fp;
    int read_int, decode_stage=0;
    char line[128];

	fp = fopen(filename, "r");
	if (fp) {
	    while (fgets (line , 128 , fp) != NULL) {
	    	vector<int> net;
	    	switch(decode_stage) {
	    		case 0:				//read in grid size
	    			config.set_grid_size(atoi(line));
	    			break;
	    		case 1:				//read in width
	    			config.set_track_width(atoi(line));
	    			break;
	    		default:				//read netlists;
	    			char * token = strtok(line, " ");
					while( token != NULL ) {
						if(atoi(token) == -1) {
							token=NULL;
							break;
						}
						net.push_back(atoi(token));
						if(net.size() == 6) {
							config.add_net(net);
							net.clear();
						}
						token = strtok(NULL, " ");
					}
	    			break;
    		}
    		++decode_stage;	    	
		}
		fclose(fp);
	} else {
		printf("[ERR] The specified file [%s] doesn't exist.\n\n", filename);
		exit(-1);
	}
    
    config.display_config();
	return config;
}

Circuit gen_circuit(CConfig config, char type) {
	Circuit circuit(config.get_track_width(), config.get_grid_size(), type);
	return circuit;
}


int main(int argc, char *argv[]) {
	CConfig config;

	char filename[20]="circuits/";

	if(argc < 7 || argc > 7) {
		printf("%s", usage);
		exit(-1);
	} else if(argv[4][0] != 'w' && argv[4][0] != 'f') {
		printf("%s", usage);
		exit(-2);
	} else if(argv[6][0] != 'T' && argv[6][0] != 'F') {
		printf("%s", usage);
		exit(-3);
	} else {
		strcat(filename, argv[2]);
		try { 
			config=init_util(filename);
			Circuit circuit=gen_circuit(config, argv[4][0]);		

		} catch (const char* msg) { 
			cerr << msg << endl;
		}
	}

	return 0;
}