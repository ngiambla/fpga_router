#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cirutils.h"



struct chip init_circuit(char * filename) {
	char line[128], check[128]; //*split;
	int i, j, k, num_of_lblocks, width, lbx, lby, stage=0, setupStage=0;
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
				printf("[INFO] Channel Width [%d]\n",width);
				sblck.e_pins=malloc(width*sizeof(int));
				sblck.n_pins=malloc(width*sizeof(int));
				sblck.w_pins=malloc(width*sizeof(int));
				sblck.s_pins=malloc(width*sizeof(int));

				mchip.switch_grid =  malloc ((num_of_lblocks+1)*sizeof(sblck));
				for(i=0;i<num_of_lblocks+1;++i) {
					mchip.switch_grid[i]=malloc ((num_of_lblocks+1)*sizeof(sblck));
				}
				printf("~");
				for(i=0;i<num_of_lblocks+1;++i) {
					for(j=0;j<num_of_lblocks+1;++j) {
						mchip.switch_grid[i][j].e_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].n_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].w_pins= malloc(width * sizeof(int));
						mchip.switch_grid[i][j].s_pins= malloc(width * sizeof(int));
						for(k=0;k<width;++k) {
							mchip.switch_grid[i][j].e_pins[k]=0;
							mchip.switch_grid[i][j].n_pins[k]=0;
							mchip.switch_grid[i][j].w_pins[k]=0;
							mchip.switch_grid[i][j].s_pins[k]=0;
						}

					}
				}
			}
			else if(stage > 1) {
					if(atoi(line) != -1){
						switch(setupStage) {
							case 0: 
								lbx=atoi(line);
								break;
							case 1:
								lby=atoi(line);
								break;
							case 2:
								printf("Setting lblock[%d][%d] as source from pin[%d]\n",lbx,lby,atoi(line));
								mchip.logic_grid[lbx][lby].pins[atoi(line)]=SOURCE;
								break;
							case 3:
								lbx=atoi(line);
								break;
							case 4:
								lby=atoi(line);
								break;
							case 5:
								mchip.logic_grid[lbx][lby].pins[atoi(line)]=TARGET;
								setupStage=0;
								break;
						}
						setupStage++;
					}
			}
			stage++;
	    }
		for(i=0;i<num_of_lblocks;++i) {
			for(j=0;j<num_of_lblocks;++j) {
				for(k=0;k<4;++k) {
					printf("[%2d]",mchip.logic_grid[i][j].pins[k]);
				}
				printf("   ");
			}
			printf("\n");
		}
	    printf("[INFO] Chip Ready for Analysis.\n");
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
	mchip=init_circuit(filename);

	free(mchip.logic_grid);
	return 0;
}