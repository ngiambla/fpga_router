#include "cirdefs.h"
#include "circuit.h"
#include "router.h"
#include "cconfig.h"
#include <fstream>
#include "graphics.h"

const char * usage="Usage ./cirutils -file filename -switch [f | w] -isParallel [T | F]\n";

void drawscreen (void);
void layout_circuit(Circuit c);
void act_on_button_press (float x, float y);
void act_on_mouse_move (float x, float y);
void act_on_key_press (char c);

static Circuit circuit;
Paths_t paths;

CConfig init_util(char * filename) {
	CConfig config;
    FILE *fp;
    int decode_stage=0;
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
	char buf[120]="";

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
			printf("\n\n############### [fpga_router] ###############\n\n");
			printf("-- init.\n\n");
			config=init_util(filename);
			circuit=gen_circuit(config, argv[4][0]);
			Router router(config.get_netlist(), argv[6][0]);
			paths=router.begin_routing(circuit);

			if(paths.size() == 0) {
				printf("[ERROR] Fatal Error. Netlist unroutable.\n");
				exit(-1);
			}

			printf("-- Going to Display.\n");
			init_graphics("#-##-###-#### [FPGA Router] ####-###-##-#", WHITE);

			sprintf(buf, "-- Circuit [%s] Routed.", argv[2]);

			update_message(buf);

   			init_world (0, 0, 1000, 1000);			
			clearscreen();


			set_keypress_input (false);
			set_mouse_move_input (false);

			drawscreen();
			event_loop(act_on_button_press, act_on_mouse_move, act_on_key_press, drawscreen);

			close_graphics ();
			printf ("[Router] Completed. Exited\n>\n>\n");


		} catch (const char* msg) { 
			cerr << msg << endl;
		}
	}

	return 0;
}


void drawscreen (void) {
	int i, j, k, g=0; 
	int blck_sz=100;
	int grid_size=circuit.get_size();
	int width_size = circuit.get_width();
	float inc_wire=blck_sz/(2*width_size);
	int offset=inc_wire;

	char buf[10]="";

	set_draw_mode (DRAW_NORMAL);
	clearscreen();
	setlinewidth (2);
	//Draw Switch Blocks
	for(i=0; i< grid_size*2+1; ++i) {
		for(j=0; j<grid_size*2+1; ++j) {
			if(i%2==0) {
				Sblck sblck=circuit.get_switch(i/2,j/2);
				if(j%2==0) {
					setcolor(DARKGREY);
					fillrect(10+j*blck_sz, 10+i*blck_sz, 110+j*blck_sz,110+i*blck_sz);
					setfontsize (8);
					sprintf(buf,"[%2d,%2d]", i/2,j/2);
					setcolor(BLACK);
					drawtext (10+j*blck_sz+blck_sz*0.5,10+i*blck_sz+blck_sz*0.5,buf,800);
				} 
				else {
					if(sblck.is_side_avail(EAST)==1){
						for(k=0;k<width_size*2; ++k) {
							if(k%2==0){

								if(sblck.get_pin(EAST, k/2)== UNAVAIL){
									setcolor(GREEN);
									//set_rgb_color(100,100,21);
								} else {
									setcolor(BLACK);
								}
								drawline (10+j*blck_sz,10+offset+i*blck_sz+k*inc_wire,110+j*blck_sz,10+offset+i*blck_sz+k*inc_wire);
							}
						}
					}
				}
			} else {
				if(j%2==0) {
					Sblck sblck=circuit.get_switch(i/2,j/2);
					if(sblck.is_side_avail(SOUTH)==1){
						for(k=0;k<width_size*2; ++k) {
							if(k%2==0){
								if(sblck.get_pin(SOUTH, k/2)==UNAVAIL){
									setcolor(GREEN);
								} else {
									setcolor(BLACK);
								}
								drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
							}
						}
					}
				}
			}
		}
	}

	//Draw Logic Blocks
	for(i=0; i<grid_size*2; ++i) {
		for(j=0; j<grid_size*2; ++j) {
			if(i%2==0) {
				if(j%2==0) {
					setcolor(LIGHTGREY);
					fillrect(110+j*blck_sz, 110+i*blck_sz, 210+j*blck_sz, 210+i*blck_sz);
					setfontsize (8);
					sprintf(buf,"(%2d,%2d)", i/2,j/2);
					setcolor(BLACK);
					drawtext (110+j*blck_sz+blck_sz*0.5,110+i*blck_sz+blck_sz*0.5,buf,800);
				}
			}
		}
	}

	for(Path_t path : paths) {
		set_rgb_color(200, g, 50);
		for(Path ele : path) {
			Sblck s=ele.get_sblck();
			//s.display_id();
			switch(ele.get_side()) {
				case NORTH:
					k=ele.get_pin()*2;
					j=s.get_x()*2;
					i=s.get_y()*2-1;
					drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
					break;
				case EAST:
					k=ele.get_pin()*2;
					j=s.get_x()*2;
					i=s.get_y()*2;
					drawline (10+j*blck_sz,10+offset+i*blck_sz+k*inc_wire,110+j*blck_sz,10+offset+i*blck_sz+k*inc_wire);
					break;
				case SOUTH:
					k=ele.get_pin()*2;
					j=s.get_x()*2;
					i=s.get_y()*2;
					drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
					break;
				case WEST:
					k=ele.get_pin()*2;
					j=s.get_x()*2-1;
					i=s.get_y()*2;
					drawline (10+j*blck_sz,10+offset+i*blck_sz+k*inc_wire,110+j*blck_sz,10+offset+i*blck_sz+k*inc_wire);
					break;
			}
		}
		g=g+10;
	}



}


void act_on_button_press (float x, float y) {
}

void act_on_mouse_move (float x, float y) {
}

void act_on_key_press (char c) {
}

