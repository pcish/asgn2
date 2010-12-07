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

    // Before removing segments
    Ptr<Instance> port = manager->instanceNew("testPort", "Port");
    Ptr<Instance> t;
    t = manager->instanceNew("truckSeg1", "Truck segment");
    t->attributeIs("source", "testPort");
    t = manager->instanceNew("boatSeg1", "Boat segment");
    t->attributeIs("source", "testPort");
    t = manager->instanceNew("planeSeg1", "Plane segment");
    t->attributeIs("source", "testPort");
    t = manager->instanceNew("boatSeg2", "Boat segment");
    t->attributeIs("source", "testPort");
    t = manager->instanceNew("truckSeg2", "Truck segment");
    t->attributeIs("source", "testPort");
    t = manager->instanceNew("planeSeg2", "Plane segment");
    t->attributeIs("source", "testPort");

    cout << "Before removing segments from testPort:" << endl;
    cout << port->attribute("segment1") << endl;
    cout << port->attribute("segment2") << endl;
    cout << port->attribute("segment3") << endl;
    cout << port->attribute("segment4") << endl;
    cout << port->attribute("segment5") << endl;
    cout << port->attribute("segment6") << endl;

    // Remove segments
    Ptr<Instance> customer = manager->instanceNew("testCustomer", "Customer");
    t = manager->instance("planeSeg1");
    t->attributeIs("source", "testCustomer");

    cout << "After re-sourcing planeSeg1:" << endl;
    cout << port->attribute("segment1") << endl;
    cout << port->attribute("segment2") << endl;
    cout << port->attribute("segment3") << endl;
    cout << port->attribute("segment4") << endl;
    cout << port->attribute("segment5") << endl;

    manager->instanceDel("truckSeg2");
    cout << "After deleting truckSeg2:" << endl;
    cout << port->attribute("segment1") << endl;
    cout << port->attribute("segment2") << endl;
    cout << port->attribute("segment3") << endl;
    cout << port->attribute("segment4") << endl;

    t = manager->instance("boatSeg1");
    t->attributeIs("source", "");
    cout << "After setting source to null" << endl;
    cout << port->attribute("segment1") << endl;
    cout << port->attribute("segment2") << endl;
    cout << port->attribute("segment3") << endl;

    t->attributeIs("source", "testPort");
    cout << "After adding the segment back" << endl;
    cout << port->attribute("segment1") << endl;
    cout << port->attribute("segment2") << endl;
    cout << port->attribute("segment3") << endl;
    cout << port->attribute("segment4") << endl;

    cerr << "Done!" << endl;

    return 0;
}

