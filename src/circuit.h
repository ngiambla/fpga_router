#ifndef __CIRCUIT_H_INCLUDED__
#define __CIRCUIT_H_INCLUDED__

#include "cirdefs.h"
#include "lblck.h"
#include "sblck.h"
#include "track.h"

typedef vector<Lblck> Lrow;
typedef vector<Lrow> Lgrid;

typedef vector<Sblck> Srow;
typedef vector<Srow> Sgrid;

typedef vector<Track> Trow;
typedef vector<Trow> Tracks;

class Circuit {
	private:
		Lgrid lgrid;
		Sgrid sgrid;
		Tracks h_tracks;
		Tracks v_tracks;

		int width;
		int size;
		char type;

	public:
		Circuit(int width, int size, char type){
			int i, j, q;

			this->width=width;
			this->size=size;
			this->type=type;

			/* Create H_Tracks */
			for(i=0; i<size+1; ++i) {
				Trow trow;
				for(j=0; j<size; ++j) {
					trow.push_back(Track(width));
				}
				h_tracks.push_back(trow);
			}
			/* Create V_tracks */
			for(i=0; i<size; ++i) {
				Trow trow;
				for(j=0; j<size+1; ++j) {
					trow.push_back(Track(width));	
				}
				v_tracks.push_back(trow);
			}

			/* Create Logic Grid */
			for(i=0; i<size; ++i) {
				Lrow lrow;
				for(j=0; j<size; ++j) {
					lrow.push_back(Lblck(i,j,width));
				}
				lgrid.push_back(lrow);
			}
			/* Create Switch Grid */
			for(i=0; i<size+1; ++i) {
				Srow srow;
				for(j=0; j<size+1; ++j) {
					srow.push_back(Sblck(i,j,width, type));
				}
				sgrid.push_back(srow);
			}
			/* Connect Tracks to Switches */
			for(i=0; i<size+1; ++i) {
				for(j=0; j<size; ++j) {
					sgrid[i][j].connect_track(h_tracks[i][j], EAST);
					sgrid[i][j+1].connect_track(h_tracks[i][j], WEST);
				}
			}
			for(j=0;j<size+1;++j) {
				for(i=0;i<size;++i) {
					sgrid[i][j].connect_track(v_tracks[i][j], SOUTH);
					sgrid[i+1][j].connect_track(v_tracks[i][j], NORTH);
				}
			}

			/* Connect Track to Lblcks */
			for(i=0; i<size; ++i) {
				for(j=0; j<size ; ++j) {
					lgrid[i][j].connect_track(h_tracks[i][j], NORTH);
					lgrid[i][j].connect_track(h_tracks[i+1][j], SOUTH);	
					lgrid[i][j].connect_track(v_tracks[i][j], WEST);
					lgrid[i][j].connect_track(v_tracks[i][j+1], EAST);
				}
			}

		}

		Sblck& get_switch(int x, int y);
		Lblck& get_lblck(int x, int y);
		int get_width();
		int get_size();
		void reset();

};

#endif