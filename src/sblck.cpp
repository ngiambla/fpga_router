#include "cirdefs.h"
#include "track.h"
#include "sblck.h"


void Sblck::connect_track(Track &trck, int side) {
	switch(side) {
		case NORTH:
			n_pins=&trck.get_track();
			n_conn=1;
			break;
		case EAST:
			e_pins=&trck.get_track();
			e_conn=1;
			break;
		case WEST:
			w_pins=&trck.get_track();
			w_conn=1;
			break;
		case SOUTH:
			s_pins=&trck.get_track();
			s_conn=1;
			break;
		default:
			throw "Invalid Side.";
	}
}

int Sblck::is_side_avail(int side) {
	switch(side) {
		case NORTH:
			return n_conn;
		case EAST:
			return e_conn;
		case SOUTH:
			return s_conn;
		case WEST:
			return w_conn;
	}
}

void Sblck::set_pin(int side, int pin, int weight) {
	switch(side) {
		case NORTH:
			(*n_pins)[pin]=weight;
			break;
		case EAST:
			(*e_pins)[pin]=weight;
			break;
		case SOUTH:
			(*s_pins)[pin]=weight;
			break;
		case WEST:
			(*w_pins)[pin]=weight;
			break;
	}
}

int Sblck::wilton(int dest, int src, int i) {
	if(dest == src){
		return i;
	}
	switch(src) {
		case NORTH:
			switch(dest) {
				case EAST:
					return n_to_e[i];
				case SOUTH:
					return i;
				case WEST:
					return n_to_w[i];
			}
		case EAST:
			switch(dest) {
				case NORTH:
					return e_to_n[i];
				case SOUTH:
					return e_to_s[i];
				case WEST:
					return i;
			}
		case SOUTH:
			switch(dest) {
				case NORTH:
					return i;
				case EAST:
					return s_to_e[i];
				case WEST:
					return s_to_w[i];
			}
		case WEST:
			switch(dest) {
				case NORTH:
					return w_to_n[i];
				case EAST:
					return i;
				case SOUTH:
					return w_to_s[i];
			}
	}
}

void Sblck::set_pin(int dest, int src, int pin, int weight) {
	mutex blck_access;
	int i;
	blck_access.lock();
	if(sw_type=='f' || src == dest) {
		set_pin(dest, pin, weight);
	} else {
		i=wilton(dest, src, pin);
		set_pin(dest, i, weight);
	}
	blck_access.unlock();

}

int Sblck::get_pin(int side, int pin) {
	switch(side) {
		case NORTH:
			return (*n_pins)[pin];
		case EAST:
			return (*e_pins)[pin];
		case SOUTH:
			return (*s_pins)[pin];
		case WEST:
			return (*w_pins)[pin];
		default:
			throw "Invalid Side";
	}
}

int Sblck::get_pin(int dest, int src, int pin) {
	int i;
	if(sw_type=='f') {
		return get_pin(dest, pin);
	} else {
		i=wilton(dest, src, pin);
		return get_pin(dest, i);
	}
}

int Sblck::get_pin_pos(int dest, int src, int pin) {
	if(sw_type=='f') {
		return pin;
	} else {
		return wilton(dest, src, pin);
	}
}

void Sblck::display_block() {
	int i;
	printf("[ N ]  [ E ]  [ S ]  [ W ]\n");
	for(i=0; i< w_per_pin; ++i) {
		if(n_conn==1) {
			printf("[%2d ]  ", (*n_pins)[i]);
		}else {
			printf("[%2d ]  ", UNAVAIL);
		}
		if(e_conn==1) {
			printf("[%2d ]  ", (*e_pins)[i]);			
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		if(s_conn==1) {
			printf("[%2d ]  ", (*s_pins)[i]);						
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		if(w_conn==1) {
			printf("[%2d ]  ", (*w_pins)[i]);						
		} else {
			printf("[%2d ]  ", UNAVAIL);			
		}
		printf("\n");
	}
}

vector<int>*& Sblck::get_side(int dir){
	switch(dir) {
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

int Sblck::get_x() {
	return x;
}

int Sblck::get_y() {
	return y;
}


void Sblck::display_id() {
	printf("Sblck[%d][%d] \n", x,y);
}

int Sblck::set_switch(int dest, int src, int pin) {
	int i;
	vector<int> * src_v, * dest_v;
	mutex blck_access;
	blck_access.lock();
	if(was_seen==0) {
		if(sw_type == 'f') {
			if(is_side_avail(dest) == 1) {
				src_v=get_side(src);
				dest_v=get_side(dest);
				if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
						(*dest_v)[pin]=(*src_v)[pin]+1;
				}

			}
		} else {
			if(is_side_avail(dest) == 1) {
				src_v=get_side(src);
				dest_v=get_side(dest);
				i=wilton(dest, src, pin);
				if((*src_v)[pin] >= 0 && (*dest_v)[i] == AVAIL) {
						(*dest_v)[i]=(*src_v)[pin]+1;
				}
			}
		}
	}
	blck_access.unlock();

}

void Sblck::was_used() {
	mutex blck_access;
	blck_access.lock();
	was_seen=1;
	blck_access.unlock();
}

