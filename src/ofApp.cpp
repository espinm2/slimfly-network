#include "ofApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>  


//--------------------------------------------------------------
// Default OpenFrameworks Functions
//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::setup(){

    // Give the boot if not used correctly
    if (arguments.size() != 7){
        printf("Improper Usage!\n");
        printf("Usage Example: ./slimfly-network -timestep <timestep> -spring_constants <c1> <c2> <c3>\n");
        return;
    }

    timestep = atof(arguments[2].c_str());
    c1 = atof(arguments[4].c_str());
    c2 = atof(arguments[5].c_str());
    c3 = atof(arguments[6].c_str());
    // spring_constant = atof(arguments[4].c_str());
    spring_rest_length = 10;
    simulation_time = 0;

    // Load in the data in memory
    // loadLog("../data/Min-UR-100/slimfly_router_sends_recvs_log.txt","router");
    // loadLog("../data/Min-UR-100/slimfly_terminal_sends_recvs_log.txt" ,"terminal");
    loadTopology("../data/MMS.19.9.bsconf");
    // loadTopology("../data/debug.bsconf");
    // createSampleData();
    pause = false;
    printSystem();

    cx = 500, cy = 400, r = 350;
    for (size_t i=0; i < routers.size(); ++i) {
        double theta = i * 2 * M_PI / routers.size();
        routers[i]->pos.x = cx + (r * cos(theta));
        routers[i]->pos.y = cy + (r * sin(theta));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
	if (pause) return;

    printf("Update loop: %i\n" , simulation_time);

    // Go though all our routers
    for(unsigned int i = 0; i < routers.size(); i++){

        Router * router_a = routers[i];
        // printf("Router %3i|%4f %4f|%4f %4f|\n",
        // router_a->id,
        // router_a->pos.x,
        // router_a->pos.y,
        // router_a->updatedPos.x,
        // router_a->updatedPos.y);

        ofVec2f total_force;

        // We want to the total force felt by all nodes that share an edge with me
        for(unsigned int j = 0; j < routers.size(); j++){

            Router * router_b = routers[j];
            ofVec2f force;

            // Do not include myself in this calculation
            if( router_a == router_b)
                continue;

            // Do I share a link with this router?
            if(isConnectedto(router_a,router_b)){
                getSpringForce(router_a->pos, router_b->pos, force);
                // getRepulsiveForce(router_a->pos, router_b->pos, force);
                // printf("\tRouter %i applies spring    <%4f,%4f>\n", router_b->id, force.x, force.y);
            }else{
                getRepulsiveForce(router_a->pos, router_b->pos, force);
                // printf("\tRouter %i applies repulsive <%4f,%4f>\n", router_b->id, force.x, force.y);
            }

            total_force += force;
        }

        // printf("\tRouter %i total_force <%4f,%4f>\n", router_a->id, total_force.x, total_force.y);

        applyForceToUpdatedPos(router_a,total_force);
    }


    // Go though all our routers and update state
    for(unsigned int i = 0; i < routers.size(); i++){
        routers[i]->pos = routers[i]->updatedPos;
    }

    simulation_time++;
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (pause) return;
	ofBackgroundGradient(ofColor(76, 76, 76), ofColor(0, 0, 0));

	for (auto &w: wires) {
        ofFill();
        double displacement = spring_rest_length - w->a_ptr->pos.distance(w->b_ptr->pos);
        if( displacement > 0 ){
            ofSetColor(0,0,255); // blue (repulsive)
        }else{
            ofSetColor(0,255,0); // green (attractive)
        }
		ofDrawLine(w->a_ptr->pos.x, w->a_ptr->pos.y, w->b_ptr->pos.x, w->b_ptr->pos.y);
	}

    ofSetColor(1,1,1); // blue (repulsive)
    for(unsigned int i = 0; i < routers.size(); i++){
        ofDrawCircle(routers[i]->pos.x, routers[i]->pos.y, 5);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	// same key bindings to pause & seek as YouTube uses
	switch (key) {
		case 'j':
			break;
		case 'k':
			pause = !pause;
			break;
		case 'l':
			break;
	}
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

//--------------------------------------------------------------
// Setup Util Methods
//--------------------------------------------------------------
bool ofApp::loadLog(const std::string & filepath, const std::string & entity){

    // Opening router log file
    std::ifstream inputFile;
    inputFile.open(filepath);

    if ( ! (entity == "router" || entity == "terminal" ) ){
        // printf("Invalid entity in load_log arguments\n");
        return false;
    }

    if (!inputFile){
        // printf("Unable to open %s\n", filepath.c_str() );
        return false;
    }

    // printf("Reading file: %s\n", filepath.c_str());
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

        // printf("Cur Line: %s\n", line.c_str());

        // Router we will be creating connections for
        Router * cur_router;

        unsigned int index = 0;     // We will use to keep track of even / odds
        std::stringstream ss(line);
        std::string str;
        bool flush = false;

        while (getline (ss, str, ' '))
        {

            // printf(" %10s:  ", str.c_str());

            if(flush){
                // printf(" Flushing...\n");
                continue;
            }

            if( index == 0 ){

                // printf(" Entity type: %s\n",str.c_str() );

            } else if (index == 1 ){

                // Set our cur_router
                // printf(" Entity id(get): %i ", atoi(str.c_str()));
                cur_router = addGetRouterById(atoi(str.c_str()));
                // printf(" Entity id(got): %i\n", cur_router->id);

            } else if (index % 2 == 0) {

                // printf(" Connection type: %s\n",str.c_str() );

                if( str == "node"){
                    flush = true;
                    continue;
                }


            }else{

                // printf("  Connection id: %i\n",atoi(str.c_str()) );

                // Add connection
                Router * adj_router = addGetRouterById(atoi(str.c_str()));
                Wire * w = addGetWireByIds(cur_router->id, adj_router->id);
                cur_router->connections.push_back(w); // add to our connections

            }

            index++;
        }

    }



    int cx = 500, cy = 400, r = 350;

    for (size_t i=0; i < routers.size(); ++i) {
        double theta = i * 2 * M_PI / routers.size();
        routers[i]->pos.set(cx + r * cos(theta), cy + r * sin(theta));
    }

    printf("Done loadTopology\n");

    return true;
}

//--------------------------------------------------------------
// Accessors/Modifer Methods
//--------------------------------------------------------------
Wire * ofApp::addGetWireByIds(unsigned int a, unsigned int b){

    // We are looking if this wire exist
    for(unsigned int i = 0; i < wires.size(); i++){

        Wire * w = wires[i];

        if(w->a_ptr->id == a && w->b_ptr->id == b){
            return w;
        }

        if(w->a_ptr->id == b && w->b_ptr->id == a){
            return w;
        }

    }

    // We have to add this wire
    Router * router_a = addGetRouterById(a);
    Router * router_b = addGetRouterById(b);
    Wire * w = new Wire(router_a,router_b);
    router_a->connections.push_back(w);
    router_b->connections.push_back(w);
    wires.push_back(w);
    return w;
}

Router * ofApp::addGetRouterById(unsigned int i){

   // // printf("Looking for Router with Id %i\n", i);

    for(unsigned int j = 0; j < routers.size(); j++){

        if( routers[j]->id == i){
            // // printf("Found Router %i\n", i);
            return routers[j];
        }
    }

    // If we pass this point we need to add our new router
    Router * r = new Router(i);

    routers.push_back(r);

    return r;
}

bool ofApp::isConnectedto(Router * a, Router * b){
    for(unsigned int i = 0; i < a->connections.size(); i++ ){

        // Check both ends of this wire
        Wire * w = a->connections[i];

        // printf("\tChecking Wire (%3i,%3i)\n", w->a_ptr->id, w->b_ptr->id);

        if(w->a_ptr->id == a->id && w->b_ptr->id == b->id){
            return true;
        }

        if(w->a_ptr->id == b->id && w->b_ptr->id == a->id){
            return true;
        }

    }

    return false;
}

//--------------------------------------------------------------
// Debug Methods
//--------------------------------------------------------------
void ofApp::printSystem(){

    // printf("Routers Size: %i\n",routers.size() );
    // printf("Wires Size: %i\n",wires.size() );
    for(int i=0; i < routers.size(); i++){

        Router * cur_router = routers[i];

        // printf("Router(%i) ", cur_router->id);

        for(int j=0; j< cur_router->connections.size(); j++){

            Wire * w = cur_router->connections[j];
            unsigned int other_router;

            if ( w->a_ptr == cur_router)
                other_router = w->b_ptr->id;
            else
                other_router = w->a_ptr->id;

            // printf("%i ", other_router);

        }
        // printf("\n");
    }
}

bool ofApp::createSampleData(){
    // for (int i = 1; i < 21; ++i) {
    //  routers.push_back(new Router(i));
    // }
    // for (auto &r1: routers) {
    //  for (auto &r2: routers) {
    //      if (r2->id == r1->id)
    //          continue;
    //      wires.push_back(new Wire(r1, r2));
    //  }
    // }
    // return true;

    addGetRouterById(1);
    addGetRouterById(2);
    addGetRouterById(3);
    addGetRouterById(4);
    addGetRouterById(5);
    addGetRouterById(6);

    addGetWireByIds(1,2);
    addGetWireByIds(1,3);
    addGetWireByIds(2,4);
    addGetWireByIds(2,5);
    addGetWireByIds(2,6);
    addGetWireByIds(1,7);

    int cx = 500, cy = 400, r = 350;

    for (size_t i=0; i < routers.size(); ++i) {
        double theta = i * 2 * M_PI / routers.size();
        routers[i]->pos.set(cx + r * cos(theta), cy + r * sin(theta));
    }
}

//--------------------------------------------------------------
// Force Related Calculation Methods
//--------------------------------------------------------------

void ofApp::applyForceToUpdatedPos(Router * r, ofVec2f & f){

    // Applying force to updated position
    r->updatedPos.set(
        r->pos.x + (f.x * timestep) ,
        r->pos.y + (f.y * timestep)
    );
}

void ofApp::getSpringForce(const ofVec2f & a, const ofVec2f & b, ofVec2f & res){
    // returns false by ref via res

    // Step 1: Get the distance between two vectors
    double dist = a.distance(b);
    // double displacement = spring_rest_length - dist;

    // Step 2: Calculate absolute repulsive force
   double force = -1*c1*log(dist/c2);

    // Step 3: Find the force vector components
    ofVec2f axis(1,0);
    ofVec2f dir = (a-b).getNormalized();
    double radian = (double)(axis.angleRad(dir));

    // Step 4: Set the res vector
    res.set(force * cos(radian),force * sin(radian));
}

void ofApp::getRepulsiveForce(const ofVec2f & a, const ofVec2f & b, ofVec2f & res){
    // Step 0: We are going to repel only if we are beyond resting length

    // Step 1: Get the distance between two vectors
    double dist = a.distance(b);

    // Step 2: Calculate absolute repulsive force
    double force = c3 / (dist * dist);

    // Step 3: Find the force vector components
    ofVec2f axis(1,0);
    ofVec2f dir = (a-b).getNormalized();
    double radian = (double)(axis.angleRad(dir));

    // Step 4: Set the res vector
    res.set(force * cos(radian),force * sin(radian));
}
