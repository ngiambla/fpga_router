#include "cirdefs.h"
#include "lblck.h"

void Lblck::disconnect_pin(int side, int pin) {
	vector<int> lblck_side=get_side(side);
	if(pin > w_per_pin) {
		throw "Pin Larger than Width";
	}
	lblck_side[pin]=DISCONNECTED;
	set_side(side, lblck_side);
} 

void Lblck::connect_pin(int side, int pin) {
	vector<int> lblck_side=get_side(side);
	if(pin > w_per_pin) {

	}
	lblck_side[pin]=CONNECTED;
	set_side(side, lblck_side);
}

void Lblck::display_block(){
	int i=0;
	for(i=0; i < w_per_pin; ++i) {
		printf("[%d][%d][%d][%d]\n",n_pins[i],e_pins[i],s_pins[i],w_pins[i]);
	}
	printf("\n");
}