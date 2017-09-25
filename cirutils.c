#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "router.h"

char * usage="Usage ./cirutils -file filename -switch [f | w] -isParallel [T | F]\n";


void display_switch_config(struct chip mchip) {
	int i,j,k,num_of_lblocks=mchip.grid_size, width=mchip.width;

	for(i=0;i<num_of_lblocks+1;++i) {
		for(j=0;j<num_of_lblocks+1;++j) {
			printf("sblock[%d][%d]\n",i,j);
			printf("<N>");
			for(k=0;k<width;++k) {
				printf("[%2d]",mchip.switch_grid[i][j].n_pins[k]);
			}
			printf(" <E>");
			for(k=0;k<width; ++k) {
				printf("[%2d]",mchip.switch_grid[i][j].e_pins[k]);
			}
			printf(" <S>");
			for(k=0;k<width; ++k) {
				printf("[%2d]",mchip.switch_grid[i][j].s_pins[k]);
			}
			printf(" <W>");											
			for(k=0;k<width; ++k) {
				printf("[%2d]",mchip.switch_grid[i][j].w_pins[k]);
			}
			printf("\n\n");
		}
		printf("\n");
	}
}

void free_memory(struct chip mchip) {
	int i,j,k,num_of_lblocks=mchip.grid_size, width=mchip.width;

	for(i=0;i<num_of_lblocks+1;++i) {
		for(j=0;j<num_of_lblocks+1;++j) {
			free(mchip.logic_grid[i][j].pins);
			free(mchip.switch_grid[i][j].n_pins);
			free(mchip.switch_grid[i][j].e_pins);
			free(mchip.switch_grid[i][j].s_pins);
			free(mchip.switch_grid[i][j].w_pins);
		}
		free(mchip.logic_grid[i]);
		free(mchip.switch_grid[i]);
	}
	free(mchip.logic_grid);
	free(mchip.switch_grid);
}


struct chip route_circuit(char * filename, char switch_type, char parallel) {
	char line[128], check[128];
	int this;
	int i, j, k, num_of_lblocks, width, sbx, sby, spin, tbx, tby, tpin, stage=0, setupStage=0;
	struct chip mchip;
	struct sblock sblck;
	FILE *fp;


	fp = fopen(filename, "r");
	if (fp) {
	    while (fgets (line , 128 , fp) != NULL) {
	    	if(stage == 0) {
	    		num_of_lblocks=atoi(line);
	    		mchip.grid_size=num_of_lblocks;
				printf("[INFO] Logic Array [%d] x [%d]\n", num_of_lblocks,num_of_lblocks);
				mchip.logic_grid =  malloc (num_of_lblocks*sizeof(struct lblock));
				for(i=0;i<num_of_lblocks;++i) {
					mchip.logic_grid[i]=malloc(num_of_lblocks*sizeof(struct lblock));
				}
				for(i=0;i<num_of_lblocks;++i) {
					for(j=0;j<num_of_lblocks;++j) {
						for(k=0;k<4;++k) {
							mchip.logic_grid[i][j].pins[k]=0;
						}
					}
				}
			}
			else if(stage == 1) {
				width=atoi(line);
				mchip.width=width;
				printf("[INFO] Channel Width [%d]\n",width);
				sblck.e_pins=malloc(width*sizeof(int));
				sblck.n_pins=malloc(width*sizeof(int));
				sblck.w_pins=malloc(width*sizeof(int));
				sblck.s_pins=malloc(width*sizeof(int));

				// Allocate Space for Switches
				mchip.switch_grid =  malloc ((num_of_lblocks+1)*sizeof(sblck));
				for(i=0;i<num_of_lblocks+1;++i) {
					mchip.switch_grid[i]=malloc ((num_of_lblocks+1)*sizeof(sblck));
				}
				for(i=0;i<num_of_lblocks+1;++i) {
					for(j=0;j<num_of_lblocks+1;++j) {
						mchip.switch_grid[i][j].e_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].n_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].w_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].s_pins= malloc(width * sizeof(int));
						for(k=0;k<width;++k) {
							mchip.switch_grid[i][j].e_pins[k]=INIT;
							mchip.switch_grid[i][j].n_pins[k]=INIT;
							mchip.switch_grid[i][j].w_pins[k]=INIT;
							mchip.switch_grid[i][j].s_pins[k]=INIT;
							if(i==0) {
								mchip.switch_grid[i][j].n_pins[k]=UNAVAIL;
							}
							if(i==num_of_lblocks) {
								mchip.switch_grid[i][j].s_pins[k]=UNAVAIL;
							}
							if(j== 0) {
								mchip.switch_grid[i][j].w_pins[k]=UNAVAIL;
							}
							if(j == num_of_lblocks) {
								mchip.switch_grid[i][j].e_pins[k]=UNAVAIL;
							}
						}

					}
				}

				printf("################################################\n");
				printf("Process Parallel? [%c]\n",parallel);
				if(switch_type=='w') {
					printf("-- Using Wilton-Style Switch\n");
				} else {
					printf("-- Using Fully-Connected Switch\n");		
				}
				printf("################################################\n");
	    		printf("[INFO] Chip Ready for Analysis.\n");
			}
			else if(stage > 1) {
					char * token = strtok(line, " ");
					while( token != NULL ) {
						switch(setupStage) {
							case 0: 
								sbx=atoi(token);
								break;
							case 1:
								sby=atoi(token);
								break;
							case 2:
								spin=atoi(token)-1;
								printf("[SRC] L-Block[%2d][%2d] @ Pin[%2d]\n",sbx,sby,spin+1);
								mchip.logic_grid[sbx][sby].pins[spin]=SOURCE;
								break;
							case 3:
								tbx=atoi(token);
								break;
							case 4:
								tby=atoi(token);
								break;
							case 5:
								tpin=atoi(token)-1;
								printf("[TRG] L-Block[%2d][%2d] @ Pin[%2d]\n+--------------------------------------+\n",tbx,tby,tpin+1);
								mchip.logic_grid[tbx][tby].pins[tpin]=TARGET;
								route_path(&mchip, sbx, sby, spin, tbx, tby, tpin, switch_type, parallel);
								display_switch_config(mchip);
								reset_router(&mchip);
								setupStage=-1;
								break;
						}
						if(sbx!=-1) {
							token = strtok(NULL, " ");
						} else {
							token=NULL;
						}
						++setupStage;
					}
			}
			stage++;
	    }
	    printf("[INFO] Analysis Complete.\n");
	    fclose(fp);
	    return mchip;
	} else {
		printf("[ERR] Could not find file.\n");
		return mchip;
	}
}


int main(int argc, char * argv[]) {

	char filename[20]="circuits/";
	struct chip mchip;

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
		mchip=route_circuit(filename, argv[4][0], argv[6][0]);
		free_memory(mchip);
	}
	return 0;
}

