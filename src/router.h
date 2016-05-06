#pragma once
#include <vector>
#include "wire.h"

class Wire;

class Router {

	public:

		Router(unsigned int i){
			id = i;
			occupancy = 0;
			x = 0;
			y = 0;
		}

		// Member var
		unsigned int id;
		unsigned int occupancy;
		std::vector<Wire *> connections;



		unsigned int x;
		unsigned int y;


};