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

int Circuit::get_width(){
	return width;
}