#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
    Ptr<Instance::Manager> manager = shippingInstanceManager();

    Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");
    fleet->attributeIs("Truck, speed", "80");
    fleet->attributeIs("Boat, speed", "60");
    fleet->attributeIs("Plane, speed", "400");
    fleet->attributeIs("Truck, capacity", "50");
    fleet->attributeIs("Boat, capacity", "600");
    fleet->attributeIs("Plane, capacity", "150");

    // --- Create instances
    // -- Locations
    // customers
    Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");  
    Ptr<Instance> customer2 = manager->instanceNew("customer2", "Customer");  
    // ports
    Ptr<Instance> port1 = manager->instanceNew("port1", "Port");  
    Ptr<Instance> port2 = manager->instanceNew("port2", "Port");  

    // -- Segments
    // plane
    Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");  
    Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");  
    Ptr<Instance> planeSeg3 = manager->instanceNew("planeSeg3", "Plane segment");  
    Ptr<Instance> planeSeg4 = manager->instanceNew("planeSeg4", "Plane segment");  
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");  
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");  
    Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment");  
    Ptr<Instance> boatSeg4 = manager->instanceNew("boatSeg4", "Boat segment");  
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");  
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");  
    Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");  
    Ptr<Instance> truckSeg4 = manager->instanceNew("truckSeg4", "Truck segment");  

    // connections
    // customer1 <---> port1
    truckSeg1->attributeIs("source", "customer1");
    truckSeg2->attributeIs("source", "port1");
    truckSeg1->attributeIs("return segment", "truckSeg2");

    // port1 <---> port2
    planeSeg1->attributeIs("source", "port1");
    planeSeg2->attributeIs("source", "port2");
    planeSeg1->attributeIs("return segment", "planeSeg2");
    
    boatSeg1->attributeIs("source", "port1");
    boatSeg2->attributeIs("source", "port2");
    boatSeg1->attributeIs("return segment", "boatSeg2");
    
    truckSeg3->attributeIs("source", "port1");
    truckSeg4->attributeIs("source", "port2");
    truckSeg3->attributeIs("return segment", "truckSeg4");

    // port2 <---> customer2
    planeSeg3->attributeIs("source", "port2");
    planeSeg4->attributeIs("source", "customer2");
    planeSeg3->attributeIs("return segment", "planeSeg4");
    
    boatSeg3->attributeIs("source", "port2");
    boatSeg4->attributeIs("source", "customer2");
    boatSeg3->attributeIs("return segment", "boatSeg4");

    // -- Segment lengths
    planeSeg1->attributeIs("length", "400");
    planeSeg2->attributeIs("length", "400");
    planeSeg3->attributeIs("length", "400");
    planeSeg4->attributeIs("length", "400");
    boatSeg1->attributeIs("length", "200");
    boatSeg2->attributeIs("length", "200");
    boatSeg3->attributeIs("length", "200");
    boatSeg4->attributeIs("length", "200");
    truckSeg1->attributeIs("length", "100");
    truckSeg2->attributeIs("length", "100");
    truckSeg3->attributeIs("length", "100");
    truckSeg4->attributeIs("length", "100");

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    cout << "**** explore customer1 : distance 450 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 450") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 1000 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 1000") << endl;
    cout << endl;

    cerr << "Done!" << endl;

    return 0;
}
