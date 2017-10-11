#ifndef __SBLCK_H_INCLUDED__ 
#define __SBLCK_H_INCLUDED__ 

#include "cirdefs.h"
#include "track.h"
#include <thread>

class Sblck {
	private:
		vector<int> * n_pins;		//define pins for logic block
		vector<int> * e_pins;
		vector<int> * s_pins;
		vector<int> * w_pins;

		vector<int> w_to_n;
		vector<int> n_to_w;

		vector<int> n_to_e;
		vector<int> e_to_n;

		vector<int> e_to_s;
		vector<int> s_to_e;

		vector<int> s_to_w;
		vector<int> w_to_s;

		int n_conn;
		int e_conn;
		int s_conn;
		int w_conn;

		int was_seen;

		int w_per_pin;
		char sw_type;

		int x;	// to store coords for plot.
		int y;	//

		vector<int>*& get_side(int dir);
		int wilton(int dest, int src, int pin);

	public:
		Sblck(int x, int y, int width, char type) {

			int i;

			this->x=x;
			this->y=y;

			was_seen=0;

			w_per_pin=width;
			
			n_pins = new vector<int>();
			e_pins = new vector<int>();
			s_pins = new vector<int>();
			w_pins = new vector<int>();

			w_to_n.resize(width);
			n_to_w.resize(width);

			n_to_e.resize(width);
			e_to_n.resize(width);

			e_to_s.resize(width);
			s_to_e.resize(width);

			s_to_w.resize(width);
			w_to_s.resize(width);


			n_conn=0;
			e_conn=0;
			s_conn=0;
			w_conn=0;

			sw_type=type;
			for(i=0; i< width; ++i) {

				w_to_n[i]=(i+1)%width;
				n_to_w[w_to_n[i]]=i;

				//n-to-e
				n_to_e[i]=(width-i)%width;
				e_to_n[n_to_e[i]]=i;

				//e-to-s
				e_to_s[i]=(i+1)%width;
				s_to_e[e_to_s[i]]=i;

				//s-to-w
				s_to_w[i]=(2*width-2-i)%width;
				w_to_s[s_to_w[i]]=i;

			}
		}
		
		void display_block();
		void display_id();
		void connect_track(Track &trck, int side);
		void set_pin(int side, int pin, int weight);
		void set_pin(int dest, int src, int pin, int weight);
		int is_side_avail(int side);
		
		int get_pin(int side, int pin);
		int get_pin(int dest, int src, int pin);

		int get_pin_pos(int dest, int src, int pin);

		int set_switch(int dest, int src, int pin);
		int get_x();
		int get_y();
		void reset();
		void was_used();
		int check_used();

};

#endif