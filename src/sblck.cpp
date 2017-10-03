#include "cirdefs.h"
#include "track.h"
#include "sblck.h"


void Sblck::connect_track(Track &trck, int side) {
	switch(side) {
		case NORTH:
			n_pins=&trck.get_track();
			n_conn=1;
			break;
		case EAST:
			e_pins=&trck.get_track();
			e_conn=1;
			break;
		case WEST:
			w_pins=&trck.get_track();
			w_conn=1;
			break;
		case SOUTH:
			s_pins=&trck.get_track();
			s_conn=1;
			break;
		default:
			throw "Invalid Side.";
	}
}

int Sblck::is_side_avail(int side) {
	switch(side) {
		case NORTH:
			return n_conn;
		case EAST:
			return e_conn;
		case SOUTH:
			return s_conn;
		case WEST:
			return w_conn;
	}
}

void Sblck::set_pin(int side, int pin, int weight) {
	switch(side) {
		case NORTH:
			(*n_pins)[pin]=weight;
			break;
		case EAST:
			(*e_pins)[pin]=weight;
			break;
		case SOUTH:
			(*s_pins)[pin]=weight;
			break;
		case WEST:
			(*w_pins)[pin]=weight;
			break;
	}
}

int Sblck::get_pin(int side, int pin) {
	switch(side) {
		case NORTH:
			return (*n_pins)[pin];
		case EAST:
			return (*e_pins)[pin];
		case SOUTH:
			return (*s_pins)[pin];
		case WEST:
			return (*w_pins)[pin];
		default:
			throw "Invalid Side";
	}
}

void Sblck::display_block() {
	int i, j;
	printf("[ N ]  [ E ]  [ S ]  [ W ]\n");
	for(i=0; i< w_per_pin; ++i) {
		if(n_conn==1) {
			printf("[%2d ]  ", (*n_pins)[i]);
		}else {
			printf("[%2d ]  ", UNAVAIL);
		}
		if(e_conn==1) {
			printf("[%2d ]  ", (*e_pins)[i]);			
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		if(s_conn==1) {
			printf("[%2d ]  ", (*s_pins)[i]);						
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		if(w_conn==1) {
			printf("[%2d ]  ", (*w_pins)[i]);						
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		printf("\n");
	}
}

vector<int>* Sblck::get_side(int dir){
	switch(dir) {
		case NORTH:
			return n_pins;
		case EAST:
			return e_pins;
		case SOUTH:
			return s_pins;
		case WEST:
			return w_pins;
		default:
			throw "Invalid Side";
	}
}


int Sblck::set_switch(int dest, int src, int pin) {
	vector<int> * src_v, * dest_v;
	if(sw_type == 'f') {
		if(is_side_avail(dest) == 1) {
			src_v=get_side(src);
			dest_v=get_side(dest);
			if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
					(*dest_v)[pin]=(*src_v)[pin]+1;
			}
		}
	} else {
		
	}
	
}