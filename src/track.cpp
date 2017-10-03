#include "cirdefs.h"
#include "track.h"

void Track::connect_switches(Sblck &ls, Sblck &rs, int dir) {
	if(sblcks.size() > 2) {
		throw "Too Many Switches Connected.";
	}
	sblcks.push_back(ls);
	sblcks.push_back(rs);
	
}

void Track::connect_lblck(Lblck &lblck, int dir) {
	if(lblcks.size() > 2) {
		throw "Too Many L-Blocks Connected";
	}
	lblck.push_back(lblck);
}