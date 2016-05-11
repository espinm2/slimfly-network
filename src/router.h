#pragma once
#include <vector>
#include "wire.h"

class Wire;

class Router {

	public:

		Router(unsigned int i){
			id = i;
			occupancy = 0;
			pos.set(0,0);
			updatedPos.set(0,0);
			pinned = false;
			label.loadFont("arial.ttf", 8);
		}

		void drawSelectedLabel(){
			label.loadFont("arial.ttf", 12);
			drawLabel();
			label.loadFont("arial.ttf", 8);
		}

		void drawLabel(){
			char id_str[255]; // an array of chars
			sprintf(id_str, "%i", id);
			label.drawString(id_str, pos.x+6,pos.y-6);
		}



		// Member var
		unsigned int id;
		unsigned int occupancy;
		std::vector<Wire *> connections;
		ofVec2f pos;
		ofVec2f updatedPos;
		bool pinned;
		ofTrueTypeFont label;
		bool selected;

};