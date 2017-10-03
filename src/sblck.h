#ifndef __SBLCK_H_INCLUDED__ 
#define __SBLCK_H_INCLUDED__ 

#include "cirdefs.h"

class Sblck {
	private:
		vector<int> n_pins;		//define pins for logic block
		vector<int> e_pins;
		vector<int> s_pins;
		vector<int> w_pins;
		int w_per_pin;
		char sw_type;

	public:
		Sblck(int width, char type) {
			int i=0;
			w_per_pin=width;
			sw_type=type;
	 		for(i=0; i<width; ++i) {
	 			n_pins.push_back(AVAIL);
	 			e_pins.push_back(AVAIL);
	 			s_pins.push_back(AVAIL);
	 			w_pins.push_back(AVAIL);
	 		}
		}
	vector<int> get_side(int side) {
		switch(side) {
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

	void set_side(int side, vector<int> vec) {
		switch(side) {
			case NORTH:
				n_pins = vec;
				break;
			case EAST:
				e_pins = vec;
				break;
			case SOUTH:
				s_pins = vec;
				break;
			case WEST:
				w_pins = vec;
				break;
			default:
				throw "Invalid Side";
		}
	}

	void display_block();
	void disconnect_pin(int side, int pin);
	void connect_pin(int side, int pin);
	void get_next();
	void reset();
};

#endif