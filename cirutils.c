#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "router.h"



struct chip init_circuit(char * filename, char switch_type) {
	char line[128], check[128];
	int i, j, k, num_of_lblocks, width, sbx, sby, spin, tbx, tby, tpin, stage=0, setupStage=0;
	struct chip mchip;
	struct sblock sblck;
	FILE *fp;


	fp = fopen(filename, "r");
	if (fp) {
	    while (fscanf(fp, "%s", line)!=EOF) {
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
	    		printf("[INFO] Chip Ready for Analysis.\n");
			}
			else if(stage > 1) {
					if(atoi(line) != -1){
						switch(setupStage) {
							case 0: 
								sbx=atoi(line);
								break;
							case 1:
								sby=atoi(line);
								break;
							case 2:
								spin=atoi(line)-1;
								printf("{SRC} L-Block[%2d][%2d] @ Pin[%2d]\n",sbx,sby,spin+1);
								mchip.logic_grid[sbx][sby].pins[spin]=SOURCE;
								break;
							case 3:
								tbx=atoi(line);
								break;
							case 4:
								tby=atoi(line);
								break;
							case 5:
								tpin=atoi(line)-1;
								printf("{TRG} L-Block[%2d][%2d] @ Pin[%2d]\n",tbx,tby,tpin+1);
								mchip.logic_grid[tbx][tby].pins[tpin]=TARGET;
								route_path(&mchip, sbx, sby, spin, tbx, tby, tpin, switch_type);
		for(i=0;i<num_of_lblocks+1;++i) {
			for(j=0;j<num_of_lblocks+1;++j) {
				printf("sblock[%d][%d]\n",i,j);
				for(k=0;k<width;++k) {
					printf("[%2d]",mchip.switch_grid[i][j].n_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].e_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].s_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].w_pins[k]);
				}
				printf("\n\n");
			}
			printf("\n");
		}
								read(0, NULL, 10);

								setupStage=0;
								break;
						}
						setupStage++;
					}
			}
			stage++;
	    }
	    printf("[INFO] Analysis Complete.\n");
		for(i=0;i<num_of_lblocks;++i) {
			for(j=0;j<num_of_lblocks;++j) {
				for(k=0;k<4;++k) {
					printf("[%2d]",mchip.logic_grid[i][j].pins[k]);
				}
				printf("   ");
			}
			printf("\n\n\n");
		}
		for(i=0;i<num_of_lblocks+1;++i) {
			for(j=0;j<num_of_lblocks+1;++j) {
				printf("sblock[%d][%d]\n",i,j);
				for(k=0;k<width;++k) {
					printf("[%2d]",mchip.switch_grid[i][j].n_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].e_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].s_pins[k]);
				}
				for(k=0;k<width; ++k) {
					printf("[%2d]",mchip.switch_grid[i][j].w_pins[k]);
				}
				printf("\n\n");
			}
			printf("\n");
		}
	    fclose(fp);
	    return mchip;
	} else {
		printf("[ERR] Could not find file.\n");
		return mchip;
	}
}


int main(int argc, char * argv[]) {
	int i,j;
	char filename[20]="circuits/";
	struct chip mchip;
	strcat(filename, argv[1]);
	mchip=init_circuit(filename, argv[2][0]);

	free(mchip.logic_grid);
	free(mchip.switch_grid);
	return 0;
}

