#include "cirdefs.h"
#include "lblck.h"


void Lblck::connect_track(Track &trck, int side) {
	switch(side) {
		case NORTH:
			n_pins=&trck.get_track();
			break;
		case EAST:
			e_pins=&trck.get_track();
			break;
		case WEST:
			w_pins=&trck.get_track();
			break;
		case SOUTH:
			s_pins=&trck.get_track();
			break;
		default:
			throw "Invalid Side.";
	}

}

void Lblck::set_as_source(int side){
	int i;
	for(i=0; i<w_per_pin; ++i) {
		switch(side) {
			case NORTH:
				if((*n_pins)[i] != UNAVAIL) {
					(*n_pins)[i]=SOURCE;
				}
				break;
			case EAST:
				if((*e_pins)[i] != UNAVAIL) {
					(*e_pins)[i]=SOURCE;
				}
				break;
			case SOUTH:
				if((*s_pins)[i] != UNAVAIL) {
					(*s_pins)[i]=SOURCE;
				}
				break;
			case WEST:
				if((*w_pins)[i] != UNAVAIL) {
					(*w_pins)[i]=SOURCE;
				}
				break;
			default:
				throw "Invalid Side.";
		}
	}
}

void Lblck::set_as_target(int side){
	int i;
	for(i=0; i<w_per_pin; ++i) {
		switch(side) {
			case NORTH:
				if((*n_pins)[i] != UNAVAIL) {
					(*n_pins)[i]=TARGET;
				}
				break;
			case EAST:
				if((*e_pins)[i] != UNAVAIL) {
					(*e_pins)[i]=TARGET;
				}
				break;
			case SOUTH:
				if((*s_pins)[i] != UNAVAIL) {
					(*s_pins)[i]=TARGET;
				}
				break;
			case WEST:
				if((*w_pins)[i] != UNAVAIL) {
					(*w_pins)[i]=TARGET;
				}
				break;
			default:
				throw "Invalid Side.";
		}
	}
}

void Lblck::display_block() {
	int i;
	for(i=0;i<w_per_pin;++i) {
		printf("[%2d][%2d][%2d][%2d]\n", (*n_pins)[i], (*e_pins)[i], (*s_pins)[i], (*w_pins)[i]);
	}
}