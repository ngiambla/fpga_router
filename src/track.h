#ifndef __TRACK_H_INCLUDED__
#define __TRACK_H_INCLUDED__

#include "cirdefs.h"

class Track {
	private:
		vector<int> lines;

	public:
		Track(int width) {
			int i=0;
			for(i=0; i<width; ++i) {
				lines.push_back(AVAIL);
			}
		}

		vector<int>& get_track() {
			return lines;
		}


};


#endif