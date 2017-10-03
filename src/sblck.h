#ifndef __SBLCK_H_INCLUDED__ 
#define __SBLCK_H_INCLUDED__ 

#include "cirdefs.h"
#include "track.h"

class Sblck {
	private:
		vector<int> * n_pins;		//define pins for logic block
		vector<int> * e_pins;
		vector<int> * s_pins;
		vector<int> * w_pins;

		int n_conn;
		int e_conn;
		int s_conn;
		int w_conn;

		int w_per_pin;
		char sw_type;

		vector<int>* get_side(int dir);

	public:
		Sblck(int width, char type) {
			int i=0;
			w_per_pin=width;
			n_pins = new vector<int>();
			e_pins = new vector<int>();
			s_pins = new vector<int>();
			w_pins = new vector<int>();

			n_conn=0;
			e_conn=0;
			s_conn=0;
			w_conn=0;

			sw_type=type;
		}

		void display_block();
		void connect_track(Track &trck, int side);
		void set_pin(int side, int pin, int weight);
		int is_side_avail(int side);
		int get_pin(int side, int pin);
		int set_switch(int dest, int src, int pin);
		void reset();

};

#endif