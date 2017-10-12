#include "cirdefs.h"
#include "circuit.h"
#include "router.h"
#include "cconfig.h"
#include <fstream>
#include <cmath>
#include "graphics.h"

const char * usage="Usage ./cirutils -file filename -switch [f | w] -isParallel [T | F] -t [y | n]\n";

void drawscreen (void);
void layout_circuit(Circuit c);
void act_on_button_press (float x, float y);
void act_on_mouse_move (float x, float y);
void act_on_key_press (char c);
vector<int> generate_unique_colors(int how_many);

static Circuit circuit;
static CConfig config;
static Paths_t paths;

CConfig init_util(char * filename, int width_reduction) {
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
	    			config.set_track_width(atoi(line)-width_reduction);
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
    return config;
}

Circuit gen_circuit(CConfig config, char type) {
	Circuit circuit(config.get_track_width(), config.get_grid_size(), type);
	return circuit;
}


int main(int argc, char *argv[]) {
	int reroute_enabled=1;
	int reroute_tries=0;
	int init=0;
	char filename[20]="circuits/";
	char buf[120]="";

	if(argc < 9 || argc > 9) {
		printf("%s", usage);
		exit(-1);
	} else if(argv[4][0] != 'w' && argv[4][0] != 'f') {
		printf("%s", usage);
		exit(-2);
	} else if(argv[6][0] != 'T' && argv[6][0] != 'F') {
		printf("%s", usage);
		exit(-3);
	} else if(argv[8][0] != 'n' && argv[8][0] != 'y') {
		printf("%s", usage);
		exit(-4);
	} else {
		strcat(filename, argv[2]);
		try { 
			printf("\n\n############### [fpga_router] ###############\n\n");
			printf("-- init.\n\n");
			while(reroute_enabled==1){
				config=init_util(filename, reroute_tries);
				config.reorder_nets();
				if(init==0){
					config.display_config();
					init=1;
				}
				

				circuit=gen_circuit(config, argv[4][0]);
				Router router(config.get_netlist(), argv[6][0]);
				paths=router.begin_routing(circuit);
				cout << "[INFO] Track Width: "<< config.get_track_width() << "\n";

				if(argv[8][0]=='n'){
					reroute_enabled=0;
				} else {
					reroute_enabled=1;
					if(router.was_routable()==0) {
						++reroute_tries;
					} else {
						reroute_enabled=0;
					}
				}
			}
			if(argv[8][0]=='y') {
				config=init_util(filename, reroute_tries-1);
				config.display_config();

				circuit=gen_circuit(config, argv[4][0]);
				Router router(config.get_netlist(), argv[6][0]);
				paths=router.begin_routing(circuit);	
				cout << "[INFO] Track Width: "<< config.get_track_width() << "\n";
				argv[8][0]='n';			
			}

			if(argv[8][0]=='n') {
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
			}


		} catch (const char* msg) { 
			cerr << msg << endl;
		}
	}

	return 0;
}


void drawscreen (void) {
	int i, j, k, g=0;
	int cur_path=0; 
	int blck_sz=100;
	int grid_size=circuit.get_size();
	int width_size = circuit.get_width();
	float inc_wire=(float)blck_sz/(2*width_size);
	float offset=(float)inc_wire;
	int lblck_w_offset=25;

	int start_path=0;

	char buf[10]="";

	vector<int> colors;

	set_draw_mode (DRAW_NORMAL);
	
	clearscreen();
	setlinewidth (2);
	//Draw Logic Blocks
	for(i=0; i<grid_size*2; ++i) {
		for(j=0; j<grid_size*2; ++j) {
			if(i%2==0) {
				if(j%2==0) {
					setcolor(LIGHTGREY);
					fillrect(110+j*blck_sz, 110+i*blck_sz, 210+j*blck_sz, 210+i*blck_sz);

					setcolor(BLACK);
					//north
					drawline(110+j*blck_sz+lblck_w_offset, 110+(i-1)*(blck_sz)+offset, 110+j*blck_sz+lblck_w_offset, 110+i*(blck_sz));
	
					//east
					drawline(110+(j+1)*blck_sz, 110+i*(blck_sz)+lblck_w_offset, 110+(j+2)*blck_sz-offset, 110+i*(blck_sz)+lblck_w_offset);

					//south
					drawline(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz), 110+(j+1)*blck_sz-lblck_w_offset, 110+(i+2)*(blck_sz)-offset);

					//west
					drawline(110+(j-1)*blck_sz+offset, 110+(i+1)*(blck_sz)-lblck_w_offset, 110+(j)*blck_sz, 110+(i+1)*(blck_sz)-lblck_w_offset);

					setfontsize (8);
					sprintf(buf,"(%2d,%2d)", i/2,j/2);
					setcolor(BLACK);
					drawtext (110+j*blck_sz+blck_sz*0.5,110+i*blck_sz+blck_sz*0.5,buf,800);
				}
			}
		}
	}

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
								setcolor(BLACK);
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
								setcolor(BLACK);
								drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
							}
						}
					}
				}
			}
		}
	}

	colors=generate_unique_colors(paths.size());
	for(Path_t path : paths) {
		set_color(colors[cur_path]);
		start_path=0;
		for(Path ele : path) {

			Sblck s=ele.get_sblck();
			switch(ele.get_side()) {
				case NORTH:
					k=ele.get_pin()*2;
					i=s.get_x()*2-1;
					j=s.get_y()*2;
					drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
					if(start_path==0) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][3]*2;
						j=nets[cur_path][4]*2;
						k=nets[cur_path][5]-1;
						if(k == EAST) {	
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-10, 110+i*(blck_sz)+lblck_w_offset,buf,800);
							drawline(110+(j+1)*blck_sz, 110+i*(blck_sz)+lblck_w_offset, 110+(j+1)*blck_sz+offset+offset*ele.get_pin()*2, 110+i*(blck_sz)+lblck_w_offset);
						} else {
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j)*blck_sz+10, 110+(i+1)*(blck_sz)-lblck_w_offset,buf,800);
							drawline(110+(j-1)*blck_sz+offset+offset*ele.get_pin()*2, 110+(i+1)*(blck_sz)-lblck_w_offset, 110+(j)*blck_sz, 110+(i+1)*(blck_sz)-lblck_w_offset);
						}
					}
					if(start_path==path.size()-1) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][0]*2;
						j=nets[cur_path][1]*2;
						k=nets[cur_path][2]-1;
						if(k == EAST) {	
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-10, 110+i*(blck_sz)+lblck_w_offset,buf,800);
							drawline(110+(j+1)*blck_sz, 110+i*(blck_sz)+lblck_w_offset, 110+(j+1)*blck_sz+offset+offset*ele.get_pin()*2, 110+i*(blck_sz)+lblck_w_offset);
						} else {
							sprintf(buf,"S[%d]", cur_path);
							drawtext(110+(j)*blck_sz+10, 110+(i+1)*(blck_sz)-lblck_w_offset,buf,800);
							drawline(110+(j-1)*blck_sz+offset+offset*ele.get_pin()*2, 110+(i+1)*(blck_sz)-lblck_w_offset, 110+(j)*blck_sz, 110+(i+1)*(blck_sz)-lblck_w_offset);
						}						
					}
					break;
				case EAST:
					k=ele.get_pin()*2;
					i=s.get_x()*2;
					j=s.get_y()*2+1;
					drawline (10+j*blck_sz,10+offset+i*blck_sz+k*inc_wire,110+j*blck_sz,10+offset+i*blck_sz+k*inc_wire);
					if(start_path==0) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][3]*2;
						j=nets[cur_path][4]*2;
						k=nets[cur_path][5]-1;
						if(k==NORTH) {
							sprintf(buf, "T[%d]", cur_path);
							drawtext(110+j*blck_sz+lblck_w_offset, 110+(i)*(blck_sz)+10, buf, 800);
							drawline(110+j*blck_sz+lblck_w_offset, 110+(i-1)*(blck_sz)+offset+offset*ele.get_pin()*2, 110+j*blck_sz+lblck_w_offset, 110+i*(blck_sz));
						} else {
							sprintf(buf, "T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)-10, buf, 800);
							drawline(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz), 110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)+offset+offset*ele.get_pin()*2);
						}
					}
					if(start_path==path.size()-1) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][0]*2;
						j=nets[cur_path][1]*2;
						k=nets[cur_path][2]-1;
						if(k==NORTH) {
							sprintf(buf, "S[%d]", cur_path);
							drawtext(110+j*blck_sz+lblck_w_offset, 110+(i)*(blck_sz)+10, buf, 800);
							drawline(110+j*blck_sz+lblck_w_offset, 110+(i-1)*(blck_sz)+offset+offset*ele.get_pin()*2, 110+j*blck_sz+lblck_w_offset, 110+i*(blck_sz));
						} else {
							sprintf(buf, "S[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)-10, buf, 800);
							drawline(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz), 110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)+offset+offset*ele.get_pin()*2);
						}
					}
					break;

				case SOUTH:
					k=ele.get_pin()*2;
					i=s.get_x()*2+1;
					j=s.get_y()*2;
					drawline (10+offset+j*blck_sz+k*inc_wire,10+i*blck_sz,10+offset+j*blck_sz+k*inc_wire,110+i*blck_sz);
					if(start_path==0) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][3]*2;
						j=nets[cur_path][4]*2;
						k=nets[cur_path][5]-1;
						if(k == EAST) {	
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-10, 110+i*(blck_sz)+lblck_w_offset,buf,800);
							drawline(110+(j+1)*blck_sz, 110+i*(blck_sz)+lblck_w_offset, 110+(j+1)*blck_sz+offset+offset*ele.get_pin()*2, 110+i*(blck_sz)+lblck_w_offset);
						} else {
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j)*blck_sz+10, 110+(i+1)*(blck_sz)-lblck_w_offset,buf,800);
							drawline(110+(j-1)*blck_sz+offset+offset*ele.get_pin()*2, 110+(i+1)*(blck_sz)-lblck_w_offset, 110+(j)*blck_sz, 110+(i+1)*(blck_sz)-lblck_w_offset);
						}
					}
					if(start_path==path.size()-1) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][0]*2;
						j=nets[cur_path][1]*2;
						k=nets[cur_path][2]-1;
						if(k == EAST) {	
							sprintf(buf,"T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-10, 110+i*(blck_sz)+lblck_w_offset,buf,800);
							drawline(110+(j+1)*blck_sz, 110+i*(blck_sz)+lblck_w_offset, 110+(j+1)*blck_sz+offset+offset*ele.get_pin()*2, 110+i*(blck_sz)+lblck_w_offset);
						} else {
							sprintf(buf,"S[%d]", cur_path);
							drawtext(110+(j)*blck_sz+10, 110+(i+1)*(blck_sz)-lblck_w_offset,buf,800);
							drawline(110+(j-1)*blck_sz+offset+offset*ele.get_pin()*2, 110+(i+1)*(blck_sz)-lblck_w_offset, 110+(j)*blck_sz, 110+(i+1)*(blck_sz)-lblck_w_offset);
						}						
					}
					break;
				case WEST:
					k=ele.get_pin()*2;
					i=s.get_x()*2;
					j=s.get_y()*2-1;
					drawline (10+j*blck_sz,10+offset+i*blck_sz+k*inc_wire,110+j*blck_sz,10+offset+i*blck_sz+k*inc_wire);
					if(start_path==0) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][3]*2;
						j=nets[cur_path][4]*2;
						k=nets[cur_path][5]-1;
						if(k==NORTH) {
							sprintf(buf, "T[%d]", cur_path);
							drawtext(110+j*blck_sz+lblck_w_offset, 110+(i)*(blck_sz)+10, buf, 800);
							drawline(110+j*blck_sz+lblck_w_offset, 110+(i-1)*(blck_sz)+offset+offset*ele.get_pin()*2, 110+j*blck_sz+lblck_w_offset, 110+i*(blck_sz));
						} else {
							sprintf(buf, "T[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)-10, buf, 800);
							drawline(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz), 110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)+offset+offset*ele.get_pin()*2);
						}
					}
					if(start_path==path.size()-1) {
						vector< vector<int> > nets=config.get_netlist();
						i=nets[cur_path][0]*2;
						j=nets[cur_path][1]*2;
						k=nets[cur_path][2]-1;
						if(k==NORTH) {
							sprintf(buf, "S[%d]", cur_path);
							drawtext(110+j*blck_sz+lblck_w_offset, 110+(i)*(blck_sz)+10, buf, 800);
							drawline(110+j*blck_sz+lblck_w_offset, 110+(i-1)*(blck_sz)+offset+offset*ele.get_pin()*2, 110+j*blck_sz+lblck_w_offset, 110+i*(blck_sz));
						} else {
							sprintf(buf, "S[%d]", cur_path);
							drawtext(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)-10, buf, 800);
							drawline(110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz), 110+(j+1)*blck_sz-lblck_w_offset, 110+(i+1)*(blck_sz)+offset+offset*ele.get_pin()*2);
						}
					}
					break;
			}
			++start_path;
		}
		cur_path++;
	}



}


vector<int> generate_unique_colors(int how_many) {
	int i=0;
	float frequency=0.25;
	unsigned char red, green, blue;
	vector<int> colors;
	for (i = 0; i < how_many; ++i) {
		red   = (unsigned char) (sin(frequency*i + 0) * 127 + 128);
		green = (unsigned char) (sin(frequency*i + 1.5) * 127 + 128);
		blue  = (unsigned char) (sin(frequency*i + 3) * 127 + 128);
		colors.push_back(get_color(red, green, blue));
	}
	return colors;
}

void act_on_button_press (float x, float y) {
}

void act_on_mouse_move (float x, float y) {
}

void act_on_key_press (char c) {
}

