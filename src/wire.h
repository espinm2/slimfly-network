#pragma once
#include "router.h"

class Router;

class Wire {

	public:

		Wire(Router * a, Router * b ){
			a_ptr = a;
			b_ptr = b;
			isActive = false;
		}

		// Memeber var
		Router * a_ptr;
		Router * b_ptr;
		bool isActive;
		bool selected;

};