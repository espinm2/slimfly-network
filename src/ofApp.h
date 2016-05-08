#pragma once
#include <ofMain.h>
#include <vector>
#include <string>
#include "wire.h"
#include "router.h"

typedef std::vector<std::vector<int> > IntTable;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // Our defined functions
        bool loadLog(const std::string & filepath, const std::string & entity);
        bool loadTopology(const std::string & filepath);
        bool createSampleData();
        Router * addGetRouterById(unsigned int i);
        Wire * addGetWireByIds(unsigned int i, unsigned int j);
        void printSystem();
        void getSpringForce(const ofVec2f & a, const ofVec2f & b, ofVec2f & res);
		void getRepulsiveForce(const ofVec2f & a, const ofVec2f & b, ofVec2f & res);
        void applyForceToUpdatedPos(Router * r, ofVec2f & f);
		bool isConnectedto(Router * a, Router * b);
		
		vector<string> arguments;

        // Member Variables
        IntTable router_send_table;
        IntTable router_recv_table;
        IntTable terminal_send_table;
        IntTable terminal_recv_table;
        std::vector<Router *>routers;
        std::vector<Wire *> wires;

        double timestep;
        unsigned int simulation_time;
        double repulsive_force;
        double spring_constant;
        double spring_rest_length;
		
};
