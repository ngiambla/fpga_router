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