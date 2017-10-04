#include "circuit.h"

Sblck& Circuit::get_switch(int x, int y) {
	if(x >=0 && y<=size){
		return sgrid[x][y];
	} else {
		throw "Out of Bounds on Switch Grid";
	}
}

Lblck& Circuit::get_lblck(int x, int y) {
	if(x >=0 && y<size){
		return lgrid[x][y];
	} else {
		throw "Out of Bounds on Switch Grid";
	}
}

int Circuit::get_width() {
	return width;
}

int Circuit::get_size() {
	return size;
}

void Circuit::reset() {
	int i, j, k, side;
	for(i=0 ;i<size+1; ++i) {
		for(j=0; j<size+1; ++j) {
			for(k=0; k<width; ++k) {
				for(side=0; side<4; ++side){
					if(sgrid[i][j].is_side_avail(side)==1) {
						if(sgrid[i][j].get_pin(side,k) >= 0 || sgrid[i][j].get_pin(side,k) == TARGET || sgrid[i][j].get_pin(side,k) == SOURCE) {
							sgrid[i][j].set_pin(side,k, AVAIL);
						}
					}
				}
			}
		}
	}
}

void Circuit::compute_stats() {
	int i, j, k, side;
	int num_unused_wires;
	int avg_capacity_per_sblock=0;
	int total_capacity=0;

	printf("..-*[STATS]*-..\n");

	/* Inspect H_Tracks */
	for(i=0; i<size+1; ++i) {
		for(j=0; j<size; ++j) {
			for(int i : h_tracks[i][j].get_track()) {
				if(i!=UNAVAIL){
					num_unused_wires++;
				}
			}
		}
	}
	/* Inspect V_tracks */
	for(i=0; i<size; ++i) {
		for(j=0; j<size+1; ++j) {
			for(int i : v_tracks[i][j].get_track()) {
				if(i!=UNAVAIL){
					num_unused_wires++;
				}
			}
		}
	}
	num_unused_wires=num_unused_wires/(2*(size)*(size-1));

	for(i=0 ;i<size+1; ++i) {
		for(j=0; j<size+1; ++j) {
			for(k=0; k<width; ++k) {
				for(side=0; side<4; ++side){
					if(sgrid[i][j].is_side_avail(side)==1) {
						if(sgrid[i][j].get_pin(side,k) != UNAVAIL) {
							avg_capacity_per_sblock++;
						}
					}
				}
			}
		}
	}
	avg_capacity_per_sblock=avg_capacity_per_sblock/((size+1)*(size+1));
	total_capacity=4*width;

	printf(">> Average Number of Unused Segments/Track ==> [%4d]\n", num_unused_wires);
	printf(">> Total Capacity Per Block                ==> [%4d]\n", total_capacity);
	printf(">> Average Capacity per Block              ==> [%4d]\n", avg_capacity_per_sblock);
	printf("###### END ######\n");
}



