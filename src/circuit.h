#ifndef __CIRCUIT_H_INCLUDED__
#define __CIRCUIT_H_INCLUDED__

#include "cirdefs.h"
#include "lblck.h"
#include "sblck.h"

typedef vector<Lblck> Lrow;
typedef vector<Lrow> Lgrid;

typedef vector<Sblck> Srow;
typedef vector<Srow> Sgrid;

class Circuit {
	private:
		Lgrid lgrid;
		Sgrid sgrid;
		int width;
		char type;

	public:
		Circuit(int width, int size, char type){
			int i, j;

			this->width=width;
			this->type=type;

			for(i=0; i<size; ++i) {
				Lrow lrow;
				for(j=0; j<size; ++j) {
					lrow.push_back( Lblck(width) );
				}
				lgrid.push_back(lrow);
			}
			for(i=0; i<size+1; ++i) {
				Srow srow;
				for(j=0; j<size+1; ++j) {
					srow.push_back( Sblck(width, type));
				}
				sgrid.push_back(srow);
			}
		}



};

#endif