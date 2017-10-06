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

void Sblck::set_pin(int dest, int src, int pin, int weight) {
	int width = w_per_pin;
	int i = pin;
	if(sw_type=='f') {
		set_pin(dest, pin, weight);
	} else {
		if(src != dest) {
			switch(src) {
				case NORTH:
					switch(dest) {
						case EAST:
							set_pin(EAST, n_to_e[i], weight);
							break;
						case SOUTH:
							set_pin(SOUTH, i, weight);
							break;
						case WEST:
							set_pin(WEST, n_to_w[i], weight);
							break;
					}
					break;
				case EAST:
					switch(dest) {
						case NORTH:
							set_pin(NORTH, e_to_n[i], weight);
							break;
						case SOUTH:
							set_pin(SOUTH, e_to_s[i], weight);
							break;
						case WEST:
							set_pin(WEST, i, weight);
							break;
					}
					break;
				case SOUTH:
					switch(dest) {
						case NORTH:
							set_pin(NORTH, i, weight);
							break;
						case EAST:
							set_pin(EAST, s_to_e[i], weight);
							break;
						case WEST:
							set_pin(WEST, s_to_w[i], weight);
							break;
					}
					break;
				case WEST:
					switch(dest) {
						case NORTH:
							set_pin(NORTH, w_to_n[i], weight);
							break;
						case EAST:
							set_pin(EAST, i, weight);
							break;
						case SOUTH:
							set_pin(SOUTH, w_to_s[i], weight);
							break;
					}
					break;
			}
		} else {
			set_pin(dest, pin, weight);
		}
	}
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
	int width = w_per_pin;
	int i = pin;
	if(sw_type=='f') {
		return get_pin(dest, pin);
	} else {
		if(src != dest) {
			switch(src) {
				case NORTH:
					switch(dest) {
						case EAST:
							return get_pin(EAST, n_to_e[i]);
						case SOUTH:
							return get_pin(SOUTH, i);
						case WEST:
							return get_pin(WEST, n_to_w[i]);
					}
				case EAST:
					switch(dest) {
						case NORTH:
							return get_pin(NORTH, e_to_n[i]);
						case SOUTH:
							return get_pin(SOUTH, e_to_s[i]);
						case WEST:
							return get_pin(WEST, i);
					}
				case SOUTH:
					switch(dest) {
						case NORTH:
							return get_pin(NORTH, i);
						case EAST:
							return get_pin(EAST, s_to_e[i]);
						case WEST:
							return get_pin(WEST, s_to_w[i]);
					}
				case WEST:
					switch(dest) {
						case NORTH:
							return get_pin(NORTH, w_to_n[i]);
						case EAST:
							return get_pin(EAST, i);
						case SOUTH:
							return get_pin(SOUTH, w_to_s[i]);
					}
			}
		} else {
			get_pin(dest, pin);
		}
	}
}

void Sblck::display_block() {
	int i, j;
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
	vector<int> * src_v, * dest_v;
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
			switch(src) {
				case NORTH:
					switch(dest) {
						case EAST:
							if((*src_v)[pin] >= 0 && (*dest_v)[n_to_e[pin]] == AVAIL) {
									(*dest_v)[n_to_e[pin]]=(*src_v)[pin]+1;
							}
							break;
						case SOUTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
									(*dest_v)[pin]=(*src_v)[pin]+1;
							}
							break;
						case WEST:
							if((*src_v)[pin] >= 0 && (*dest_v)[n_to_w[pin]] == AVAIL) {
									(*dest_v)[n_to_w[pin]]=(*src_v)[pin]+1;
							}
							break;
					}
					break;
				case EAST:
					switch(dest) {
						case NORTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[e_to_n[pin]] == AVAIL) {
									(*dest_v)[e_to_n[pin]]=(*src_v)[pin]+1;
							}
							break;
						case SOUTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[e_to_s[pin]] == AVAIL) {
									(*dest_v)[e_to_s[pin]]=(*src_v)[pin]+1;
							}
							break;
						case WEST:
							if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
									(*dest_v)[pin]=(*src_v)[pin]+1;
							}
							break;
					}
					break;
				case SOUTH:
					switch(dest) {
						case NORTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
									(*dest_v)[pin]=(*src_v)[pin]+1;
							}
							break;
						case EAST:
							if((*src_v)[pin] >= 0 && (*dest_v)[s_to_e[pin]] == AVAIL) {
									(*dest_v)[s_to_e[pin]]=(*src_v)[pin]+1;
							}
							break;
						case WEST:
							if((*src_v)[pin] >= 0 && (*dest_v)[s_to_w[pin]] == AVAIL) {
									(*dest_v)[s_to_w[pin]]=(*src_v)[pin]+1;
							}
							break;
					}
					break;
				case WEST:
					switch(dest) {
						case NORTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[w_to_n[pin]] == AVAIL) {
									(*dest_v)[w_to_n[pin]]=(*src_v)[pin]+1;
							}
							break;
						case EAST:
							if((*src_v)[pin] >= 0 && (*dest_v)[pin] == AVAIL) {
									(*dest_v)[pin]=(*src_v)[pin]+1;
							}
							break;
						case SOUTH:
							if((*src_v)[pin] >= 0 && (*dest_v)[w_to_s[pin]] == AVAIL) {
									(*dest_v)[w_to_s[pin]]=(*src_v)[pin]+1;
							}
							break;
					}
					break;
			}
		}
	}
	
}