#ifndef CIRDEFS_H_ 
#define CIRDEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

#define DISCONNECTED -20	//defined for the lblck to sblck.
#define CONNECTED -10		//
#define UNAVAIL -5			//
#define USED -2				//
#define AVAIL -1			//

#define TARGET -25			// for lblck
#define SOURCE 0			//

#define SOUTH 1 			// for block pins
#define EAST 2 				//
#define NORTH 3 			//
#define WEST 0 				//

#define MAX_WEIGHT 10000	// For determining min pin assignment.

#endif