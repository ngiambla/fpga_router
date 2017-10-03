#ifndef __TRACK_H_INCLUDED__
#define __TRACK_H_INCLUDED__

#include "cirdefs.h"
#include "sblck.h"
#include "lblck.h"

class Track {
	private:
		vector<sblck> sblcks;
		vector<lblck> lblcks;
		vector<int> lines;
	public:
		Track(int width) {
			int i=0;
			for(i=0; i<width; ++i) {
				lines.push_back(AVAIL);
			}
		}

		void connect_switches(Sblck ls, Sblck rs, int dir);
		void connect_lblck(Lblck lblck, int dir);

};


#endif