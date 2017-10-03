#ifndef __ROUTER_H_INCLUDED__
#define __ROUTER_H_INCLUDED__

#include "cirdefs.h"
#include "circuit.h"

class Router {
	private:
		Circuit circuit;
	public:
		Router(Circuit circuit){
			this->circuit=circuit;
		}
		void begin_search();
		void search();
		void begin_traceback();
		void traceback();
		void check_for_target();
		
};


#endif
