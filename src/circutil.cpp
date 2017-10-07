#include "cirdefs.h"
#include "circuit.h"
#include "router.h"
#include "cconfig.h"
#include <fstream>
#include "graphics.h"

const char * usage="Usage ./cirutils -file filename -switch [f | w] -isParallel [T | F]\n";

void delay (void);
void drawscreen (void);
void layout_circuit(Circuit c);
void act_on_button_press (float x, float y);
void act_on_mouse_move (float x, float y);
void act_on_key_press (char c);

// For example showing entering lines and rubber banding
static bool rubber_band_on = false;
static bool have_entered_line, have_rubber_line;
static bool line_entering_demo = false;
static float x1, y1, x2, y2;  
static int num_new_button_clicks = 0;

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
	int i;
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
	int i, j, k; 
	int blck_sz=50;
	int grid_size=circuit.get_size();
	int width_size = circuit.get_width();
	float inc_wire=blck_sz/(2*width_size);

	set_draw_mode (DRAW_NORMAL);
	clearscreen();

	//Draw Switch Blocks
	for(i=0; i< grid_size*2+1; ++i) {
		for(j=0; j<grid_size*2+1; ++j) {
			if(i%2==0) {
				if(j%2==0) {
					setcolor(DARKGREY);
					fillrect(10+j*blck_sz, 10+i*blck_sz, 60+j*blck_sz, 60+i*blck_sz);
				} 
				else {
					for(k=0;k<width_size*2; ++k) {
						if(k%2==0){
							setcolor(BLACK);
							drawline (10+j*blck_sz,10+i*blck_sz+k*inc_wire,60+j*blck_sz,10+i*blck_sz+k*inc_wire);
						}
					}
				}
			} else {
				if(j%2==0) {

				}
				// complete switch block logic.
			}
		}
	}

	//Draw Logic Blocks
	for(i=0; i<grid_size*2; ++i) {
		for(j=0; j<grid_size*2; ++j) {
			if(i%2==0) {
				if(j%2==0) {
					setcolor(LIGHTGREY);
					fillrect(60+j*blck_sz, 60+i*blck_sz, 110+j*blck_sz, 110+i*blck_sz);
				} else {
					setcolor(WHITE);
					fillrect(60+j*blck_sz, 60+i*blck_sz, 110+j*blck_sz, 110+i*blck_sz);
				}
			}
		}
	}

	// setfontsize (10);
	// setlinestyle (SOLID);
	// setlinewidth (1);
	// setcolor (BLACK);

	// drawtext (110.,55.,"colors",150.);
	// setcolor (LIGHTGREY);
	// fillrect (150.,30.,200.,80.);
	// setcolor (DARKGREY);
	// fillrect (200.,30.,250.,80.);
	// setcolor (WHITE);
	// fillrect (250.,30.,300.,80.);
	// setcolor (BLACK);
	// fillrect (300.,30.,350.,80.);
	// setcolor (BLUE);
	// fillrect (350.,30.,400.,80.);
	// setcolor (GREEN);
	// fillrect (400.,30.,450.,80.);
	// setcolor (YELLOW);
	// fillrect (450.,30.,500.,80.);
	// setcolor (CYAN);
	// fillrect (500.,30.,550.,80.);
	// setcolor (RED);
	// fillrect (550.,30.,600.,80.);
	// setcolor (DARKGREEN);
	// fillrect (600.,30.,650.,80.);
	// setcolor (MAGENTA);
	// fillrect (650.,30.,700.,80.);
	// setcolor (WHITE);
	// drawtext (400.,55.,"fillrect",150.);

	// setcolor (BLACK);
	// drawtext (250.,150.,"drawline",150.);
	// setlinestyle (SOLID);
	// drawline (200.,120.,200.,200.);
	// setlinestyle (DASHED);
	// drawline (300.,120.,300.,200.);

	// setcolor (MAGENTA);
	// drawtext (450, 160, "drawellipticarc", 150);
	// drawellipticarc (550, 160, 30, 60, 90, 270);
	// drawtext (720, 160, "fillellipticarc", 150);
	// fillellipticarc (800, 160, 30, 60, 90, 270);

	// setcolor (BLUE);
	// drawtext (190.,300.,"drawarc",150.);
	// drawarc (190.,300.,50.,0.,270.);
	// drawarc (300.,300.,50.,0.,-180.);
	// fillarc (410.,300.,50.,90., -90.);
	// fillarc (520.,300.,50.,0.,360.);
	// setcolor (BLACK);
	// drawtext (520.,300.,"fillarc",150.);
	// setcolor (BLUE);
	// fillarc (630.,300.,50.,90.,180.);
	// fillarc (740.,300.,50.,90.,270.);
	// fillarc (850.,300.,50.,90.,30.);
	// setcolor (RED);
	// fillpoly(polypts,3);
	// fillpoly(polypts2,4);
	// setcolor (BLACK);
	// drawtext (500.,450.,"fillpoly",150.);
	// setcolor (DARKGREEN);
	// drawtext (500.,610.,"drawrect",150.);
	// drawrect (350.,550.,650.,670.); 


	// setcolor (BLACK);
	// setfontsize (8);
	// drawtext (100.,770.,"8 Point Text",800.);
	// setfontsize (12);
	// drawtext (400.,770.,"12 Point Text",800.);
	// setfontsize (15);
	// drawtext (700.,770.,"18 Point Text",800.);
	// setfontsize (24);
	// drawtext (300.,830.,"24 Point Text",800.);
	// setfontsize (32);
	// drawtext (700.,830.,"32 Point Text",800.);
	// setfontsize (10);

	// setlinestyle (SOLID);
	// drawtext (200.,900.,"Thin line (width 1)",800.);
	// setlinewidth (1);
	// drawline (100.,920.,300.,920.);
	// drawtext (500.,900.,"Width 3 Line",800.);
	// setlinewidth (3);
	// drawline (400.,920.,600.,920.);
	// drawtext (800.,900.,"Width 6 Line",800.);
	// setlinewidth (6);
	// drawline (700.,920.,900.,920.);

	// setlinewidth (1);
	// setcolor (GREEN);


	// if (have_entered_line) {
	// 	drawline (x1, y1, x2, y2);
	// }
 //   // Screen redraw will get rid of a rubber line.  
 //   have_rubber_line = false;
}


void delay (void) {

/* A simple delay routine for animation. */

   int i, j, k, sum;

   sum = 0;
   for (i=0;i<100;i++) 
      for (j=0;j<i;j++)
         for (k=0;k<1000;k++) 
            sum = sum + i+j-k; 
}


void act_on_button_press (float x, float y) {


}



void act_on_mouse_move (float x, float y) {

}


void act_on_key_press (char c) {
	// function to handle keyboard press event, the ASCII character is returned
   printf ("Key press: %c\n", c);
}


