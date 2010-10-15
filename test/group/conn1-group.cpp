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
    fleet->attributeIs("Truck, cost", "6");
    fleet->attributeIs("Boat, cost", "4");
    fleet->attributeIs("Plane, cost", "20");

    // --- Create instances
    // -- Locations
    // customers
    Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");  
    Ptr<Instance> customer2 = manager->instanceNew("customer2", "Customer");  
    Ptr<Instance> customer3 = manager->instanceNew("customer3", "Customer");  
    Ptr<Instance> customer4 = manager->instanceNew("customer4", "Customer");  
    Ptr<Instance> customer5 = manager->instanceNew("customer5", "Customer");  
    // ports
    Ptr<Instance> port1 = manager->instanceNew("port1", "Port");  
    // terminals
    Ptr<Instance> truckTerm1 = manager->instanceNew("truckTerm1", "Truck terminal");

    // -- Segments
    // plane
    Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");  
    Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");  
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");  
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");  
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");  
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");  
    Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");  
    Ptr<Instance> truckSeg4 = manager->instanceNew("truckSeg4", "Truck segment");  
    Ptr<Instance> truckSeg5 = manager->instanceNew("truckSeg5", "Truck segment");  
    Ptr<Instance> truckSeg6 = manager->instanceNew("truckSeg6", "Truck segment");  

    // connections
    // customer1 <---> port1
    truckSeg1->attributeIs("source", "customer1");
    truckSeg2->attributeIs("source", "port1");
    truckSeg1->attributeIs("return segment", "truckSeg2");
    
    // customer2 <---> port1
    boatSeg1->attributeIs("source", "customer2");
    boatSeg2->attributeIs("source", "port1");
    boatSeg1->attributeIs("return segment", "boatSeg2");

    // customer3 <---> port1
    planeSeg1->attributeIs("source", "customer3");
    planeSeg2->attributeIs("source", "port1");
    planeSeg1->attributeIs("return segment", "planeSeg2");

    // customer4 <---> truckTerm1
    truckSeg3->attributeIs("source", "customer4");
    truckSeg4->attributeIs("source", "truckTerm1");
    truckSeg3->attributeIs("return segment", "truckSeg4");
    
    // customer5 <---> truckTerm1
    truckSeg5->attributeIs("source", "customer5");
    truckSeg6->attributeIs("source", "truckTerm1");
    truckSeg5->attributeIs("return segment", "truckSeg6");

    // -- Segment lengths
    planeSeg1->attributeIs("length", "1000");
    planeSeg2->attributeIs("length", "1000");
    boatSeg1->attributeIs("length", "400");
    boatSeg2->attributeIs("length", "400");
    truckSeg1->attributeIs("length", "900");
    truckSeg2->attributeIs("length", "900");
    truckSeg3->attributeIs("length", "100");
    truckSeg4->attributeIs("length", "100");
    truckSeg5->attributeIs("length", "200");
    truckSeg6->attributeIs("length", "200");

    // -- Segment difficulties
    planeSeg1->attributeIs("difficulty", "1");
    planeSeg2->attributeIs("difficulty", "1");
    boatSeg1->attributeIs("difficulty", "1");
    boatSeg2->attributeIs("difficulty", "1");
    truckSeg1->attributeIs("difficulty", "1");
    truckSeg2->attributeIs("difficulty", "1");
    truckSeg3->attributeIs("difficulty", "1");
    truckSeg4->attributeIs("difficulty", "1");
    truckSeg5->attributeIs("difficulty", "1");
    truckSeg6->attributeIs("difficulty", "1");
    
    // -- Segment expedite support
    planeSeg1->attributeIs("expedite support", "yes");
    planeSeg2->attributeIs("expedite support", "yes");
    boatSeg1->attributeIs("expedite support", "yes");
    boatSeg2->attributeIs("expedite support", "yes");
    truckSeg1->attributeIs("expedite support", "yes");
    truckSeg2->attributeIs("expedite support", "yes");
    truckSeg3->attributeIs("expedite support", "yes");
    truckSeg4->attributeIs("expedite support", "yes");
    truckSeg5->attributeIs("expedite support", "yes");
    truckSeg6->attributeIs("expedite support", "yes");

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    cout << "**** explore customer1 : distance 1500 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 1500") << endl;
    cout << endl;
    cout << "**** explore customer5 : distance 5000 ****" << endl;
    cout << conn->attribute("explore customer5 : distance 5000") << endl;
    cout << endl;

    cout << "*** connect customer3 : customer1 ****" << endl;
    cout << conn->attribute("connect customer3 : customer1") << endl;
    cout << endl;

    cerr << "Done!" << endl;

    return 0;
}
