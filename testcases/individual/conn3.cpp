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
    Ptr<Instance> customer3 = manager->instanceNew("customer3", "Customer");  
    // ports
    Ptr<Instance> port1 = manager->instanceNew("port1", "Port");  
    Ptr<Instance> port2 = manager->instanceNew("port2", "Port");  
    Ptr<Instance> port3 = manager->instanceNew("port3", "Port");  
    // terminals 
    Ptr<Instance> planeTerm1 = manager->instanceNew("planeTerm1", "Plane terminal");  
    Ptr<Instance> boatTerm1 = manager->instanceNew("boatTerm1", "Boat terminal");  
    Ptr<Instance> truckTerm1 = manager->instanceNew("truckTerm1", "Truck terminal");  

    // -- Segments
    // plane
    Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");  
    Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");  
    Ptr<Instance> planeSeg3 = manager->instanceNew("planeSeg3", "Plane segment");  
    Ptr<Instance> planeSeg4 = manager->instanceNew("planeSeg4", "Plane segment");  
    Ptr<Instance> planeSeg5 = manager->instanceNew("planeSeg5", "Plane segment");  
    Ptr<Instance> planeSeg6 = manager->instanceNew("planeSeg6", "Plane segment");  
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");  
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");  
    Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment");  
    Ptr<Instance> boatSeg4 = manager->instanceNew("boatSeg4", "Boat segment");  
    Ptr<Instance> boatSeg5 = manager->instanceNew("boatSeg5", "Boat segment");  
    Ptr<Instance> boatSeg6 = manager->instanceNew("boatSeg6", "Boat segment");  
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");  
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");  
    Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");  
    Ptr<Instance> truckSeg4 = manager->instanceNew("truckSeg4", "Truck segment");  
    Ptr<Instance> truckSeg5 = manager->instanceNew("truckSeg5", "Truck segment");  
    Ptr<Instance> truckSeg6 = manager->instanceNew("truckSeg6", "Truck segment");  

    // connections
    // customer1 <---> planeTerm1
    planeSeg1->attributeIs("source", "customer1");
    planeSeg2->attributeIs("source", "planeTerm1");
    planeSeg1->attributeIs("return segment", "planeSeg2");
    // customer2 <---> boatTerm1
    boatSeg1->attributeIs("source", "customer2");
    boatSeg2->attributeIs("source", "boatTerm1");
    boatSeg1->attributeIs("return segment", "boatSeg2");
    // customer3 <---> truckTerm1
    truckSeg1->attributeIs("source", "customer3");
    truckSeg2->attributeIs("source", "truckTerm1");
    truckSeg1->attributeIs("return segment", "truckSeg2");

    // planeTerm1 <---> port1
    planeSeg3->attributeIs("source", "planeTerm1");
    planeSeg4->attributeIs("source", "port1");
    planeSeg3->attributeIs("return segment", "planeSeg4");
    // boatTerm1 <---> port2
    boatSeg3->attributeIs("source", "boatTerm1");
    boatSeg4->attributeIs("source", "port2");
    boatSeg3->attributeIs("return segment", "boatSeg4");
    // truckTerm1 <---> port3
    truckSeg3->attributeIs("source", "truckTerm1");
    truckSeg4->attributeIs("source", "port3");
    truckSeg3->attributeIs("return segment", "truckSeg4");

    // port1 <---> port2
    planeSeg5->attributeIs("source", "port1");
    planeSeg6->attributeIs("source", "port2");
    planeSeg5->attributeIs("return segment", "planeSeg6");
    
    // port2 <---> port3
    boatSeg5->attributeIs("source", "port2");
    boatSeg6->attributeIs("source", "port3");
    boatSeg5->attributeIs("return segment", "boatSeg6");
    
    // port3 <---> port1
    truckSeg5->attributeIs("source", "port3");
    truckSeg6->attributeIs("source", "port1");
    truckSeg5->attributeIs("return segment", "truckSeg6");

    // -- Segment lengths
    // Customer <--> term <--> port
    planeSeg1->attributeIs("length", "500");
    planeSeg2->attributeIs("length", "500");
    planeSeg3->attributeIs("length", "1500");
    planeSeg4->attributeIs("length", "1500");
    boatSeg1->attributeIs("length", "300");
    boatSeg2->attributeIs("length", "300");
    boatSeg3->attributeIs("length", "1000");
    boatSeg4->attributeIs("length", "1000");
    truckSeg1->attributeIs("length", "250");
    truckSeg2->attributeIs("length", "250");
    truckSeg3->attributeIs("length", "450");
    truckSeg4->attributeIs("length", "450");

    // port <--> port
    planeSeg5->attributeIs("length", "1000");
    planeSeg6->attributeIs("length", "1000");
    boatSeg5->attributeIs("length", "650");
    boatSeg6->attributeIs("length", "650");
    truckSeg5->attributeIs("length", "550");
    truckSeg6->attributeIs("length", "550");

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    cout << "**** explore customer1 : distance 1000 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 1000") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 1500 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 1500") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 2050 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 2050") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 2999 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 2999") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 3001 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 3001") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 3201 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 3201") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 3251 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 3251") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 4001 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 4001") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 4301 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 4301") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 4401 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 4401") << endl;
    cout << endl;
    cout << "**** explore customer1 : distance 4501 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 4501") << endl;

    cerr << "Done!" << endl;

    return 0;
}
