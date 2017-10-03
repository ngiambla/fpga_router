#ifndef __LBLCK_H_INCLUDED__ 
#define __LBLCK_H_INCLUDED__ 

#include "cirdefs.h"
#include "track.h"

class Lblck {
	private:
		vector<int> * n_pins;		//define pins for logic block
		vector<int> * e_pins;
		vector<int> * s_pins;
		vector<int> * w_pins;
		int w_per_pin;

	public:
		Lblck(int width) {
	 		int i=0;
	 		w_per_pin=width;
			n_pins = new vector<int>();
			e_pins = new vector<int>();
			s_pins = new vector<int>();
			w_pins = new vector<int>();
		}

	void display_block();
	void set_as_target(int side);
	void set_as_source(int side);
	void connect_track(Track &trck, int side);
	void reset();

};

#endif 
