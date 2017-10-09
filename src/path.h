#ifndef __PATH_H_INCLUDED__
#define __PATH_H_INCLUDED__

#include "path.h"
#include "sblck.h"


class Path {
	private:
		int pin;
		int side;
		vector<Sblck> blck;

	public:
		Path(int pin, int side, Sblck sblck) {
			this->pin=pin;
			this->side=side;
			blck.push_back(sblck);
		}

		Sblck& get_sblck() {
			return blck[0];
		}

		int get_pin() {
			return pin;
		}

		int get_side() {
			return side;
		}

		void display_path() {
			printf("SBLCK[%d,%d] SIDE[%d] PIN[%d]\n", blck[0].get_x(), blck[0].get_y(), side, pin);
		}

};

#endif
