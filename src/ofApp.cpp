#include "ofApp.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

//--------------------------------------------------------------
bool ofApp::loadLog(const std::string & filepath, const std::string & entity){

    // Opening router log file
    std::ifstream inputFile;
    inputFile.open(filepath);

    if ( ! (entity == "router" || entity == "terminal" ) ){
        printf("Invalid entity in load_log arguments\n");
        return false;
    }

    if (!inputFile){
        printf("Unable to open %s\n", filepath.c_str() );
        return false;
    }

    printf("Reading file: %s\n", filepath.c_str());
    std::string line;
    int g_time;
    while (getline( inputFile, line ))
    {

        std::stringstream ss(line);
        std::string strNum;
        unsigned int index = 0;
        unsigned int entityNum = 0;

        while (getline (ss, strNum, ','))
        {

            if ( index == 0 ) {

                // g_time is here
                g_time = atoi(strNum.c_str());

            } else if ( index % 2 == 1 ){

                // send data
                int rawData[] = {g_time,entityNum,atoi(strNum.c_str())}; // (x,y,z)
                std::vector<int> dataPoint (rawData,
                        rawData + sizeof(rawData) / sizeof(int) );

                if( entity == "router")
                    router_send_table.push_back(dataPoint);
                else
                    terminal_send_table.push_back(dataPoint);


            } else {
                // recv data
                int rawData[] = {g_time,entityNum,atoi(strNum.c_str())}; // (x,y,z)
                std::vector<int> dataPoint (rawData,
                        rawData + sizeof(rawData) / sizeof(int) );

                if( entity == "router")
                    router_recv_table.push_back(dataPoint);
                else
                    terminal_recv_table.push_back(dataPoint);

                // next entity
                entityNum++;
            }

            // incr index
            index++;
        }
    }


    return true;

}

//--------------------------------------------------------------

Router * ofApp::addGetRouterById(unsigned int i){

   // printf("Looking for Router with Id %i\n", i);

    for(int j = 0; j < routers.size(); j++){

        if( routers[j]->id == i){
            // printf("Found Router %i\n", i);
            return routers[j];
        }
    }

    // If we pass this point we need to add our new router
    Router * r = new Router(i);

    routers.push_back(r);

    return r;
}

bool ofApp::createSampleData(){
	for (int i = 1; i < 21; ++i) {
		routers.push_back(new Router(i));
	}
	for (auto &r1: routers) {
		for (auto &r2: routers) {
			if (r2->id == r1->id)
				continue;
			wires.push_back(new Wire(r1, r2));
		}
	}
	return true;

    Router * a = new Router(1);
    Router * b = new Router(2);
    Router * c = new Router(3);
    Router * d = new Router(4);
    Router * e = new Router(5);
    Router * f = new Router(6);
    Router * g = new Router(8);

    routers.push_back(a);
    routers.push_back(b);
    routers.push_back(c);
    routers.push_back(d);
    routers.push_back(e);
    routers.push_back(f);
    routers.push_back(g);


    wires.push_back(new Wire(a,b));
    wires.push_back(new Wire(a,c));
    wires.push_back(new Wire(a,d));
    wires.push_back(new Wire(b,e));
    wires.push_back(new Wire(b,f));
    wires.push_back(new Wire(e,g));

}

//--------------------------------------------------------------
bool ofApp::loadTopology(const std::string & filepath){

    printf("Running loadTopology\n");

    // Opening router log file
    std::ifstream inputFile;
    inputFile.open(filepath);

    if (!inputFile){
        printf("Unable to open %s\n", filepath.c_str() );
        return false;
    }

    std::string line;
    while (getline( inputFile, line))
    {

        printf("Cur Line: %s\n", line.c_str());

        // Router we will be creating connections for
        Router * cur_router;

        unsigned int index = 0;     // We will use to keep track of even / odds
        std::stringstream ss(line);
        std::string str;
        bool flush = false;

        while (getline (ss, str, ' '))
        {

            printf(" %10s:  ", str.c_str());

            if(flush){
                printf(" Flushing...\n");
                continue;
            }

            if( index == 0 ){

                printf(" Entity type: %s\n",str.c_str() );

            } else if (index == 1 ){

                // Set our cur_router
                printf(" Entity id(get): %i ", atoi(str.c_str()));
                cur_router = addGetRouterById(atoi(str.c_str()));
                printf(" Entity id(got): %i\n", cur_router->id);

            } else if (index % 2 == 0) {

                printf(" Connection type: %s\n",str.c_str() );

                if( str == "node"){
                    flush = true;
                    continue;
                }


            }else{

                printf("  Connection id: %i\n",atoi(str.c_str()) );

                // Add connection
                Router * adj_router = addGetRouterById(atoi(str.c_str()));
                Wire * w = addGetWireByIds(cur_router->id, adj_router->id);
                cur_router->connections.push_back(w); // add to our connections

            }

            index++;
        }

    }


    printf("Done loadTopology\n");

    return true;
}

//--------------------------------------------------------------
Wire * ofApp::addGetWireByIds(unsigned int a, unsigned int b){

    // We are looking if this wire exist
    for(int i = 0; i < wires.size(); i++){

        Wire * w = wires[i];

        if(w->a_ptr->id == a && w->b_ptr->id == b){
            return w;
        }

        if(w->b_ptr->id == b && w->a_ptr->id == a){
            return w;
        }

    }

    // We have to add this wire
    Wire * w = new Wire(addGetRouterById(a), addGetRouterById(b));

    return w;

}


void ofApp::printSystem(){

    printf("Routers Size: %i\n",routers.size() );
    printf("Wires Size: %i\n",wires.size() );
    for(int i=0; i < routers.size(); i++){

        Router * cur_router = routers[i];

        printf("Router(%i) ", cur_router->id);

        for(int j=0; j< cur_router->connections.size(); j++){

            Wire * w = cur_router->connections[j];
            unsigned int other_router;

            if ( w->a_ptr == cur_router)
                other_router = w->b_ptr->id;
            else
                other_router = w->a_ptr->id;

            printf("%i ", other_router);

        }
        printf("\n");
    }
}

//--------------------------------------------------------------
void ofApp::setup(){

    // Load in the data in memory
    loadLog("../data/Min-UR-100/slimfly_router_sends_recvs_log.txt","router");
    loadLog("../data/Min-UR-100/slimfly_terminal_sends_recvs_log.txt" ,"terminal");
    loadTopology("../data/MMS.19.9.bsconf");
    // loadTopology("../data/debug.bsconf");
    // createSampleData();
    printSystem();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(76, 76, 76), ofColor(0, 0, 0));
	int cx = 500, cy = 400, r = 350;

	for (size_t i=0; i < routers.size(); ++i) {
		double theta = i * 2 * M_PI / routers.size();
		routers[i]->x = cx + r * cos(theta);
		routers[i]->y = cy + r * sin(theta);
	}

	for (auto &w: wires) {
		ofDrawSphere(w->a_ptr->x, w->a_ptr->y, 5);
		ofDrawSphere(w->b_ptr->x, w->b_ptr->y, 5);
		ofDrawLine(w->a_ptr->x, w->a_ptr->y, w->b_ptr->x, w->b_ptr->y);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
