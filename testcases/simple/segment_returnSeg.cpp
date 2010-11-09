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
    Ptr<Instance> portA = manager->instanceNew("PortA", "Port");
    Ptr<Instance> portB = manager->instanceNew("PortB", "Port");
    Ptr<Instance> t, r;
    t = manager->instanceNew("truckSeg1", "Truck segment");
    t->attributeIs("source", "PortA");
    t = manager->instanceNew("boatSeg1", "Boat segment");
    t->attributeIs("source", "PortA");
    t = manager->instanceNew("planeSeg1", "Plane segment");
    t->attributeIs("source", "PortA");
    t = manager->instanceNew("boatSeg2", "Boat segment");
    t->attributeIs("source", "PortB");
    t = manager->instanceNew("truckSeg2", "Truck segment");
    t->attributeIs("source", "PortB");
    t = manager->instanceNew("planeSeg2", "Plane segment");
    t->attributeIs("source", "PortB");

    t = manager->instance("truckSeg1");
    r = manager->instance("truckSeg2");
    t->attributeIs("return segment", "truckSeg2");
    cout << t->attribute("return segment") << endl;
    cout << r->attribute("return segment") << endl;
    manager->instanceDel("truckSeg2");
    cout << t->attribute("return segment") << endl;

    t = manager->instance("planeSeg1");
    r = manager->instance("planeSeg2");
    r->attributeIs("return segment", "planeSeg1");
    cout << t->attribute("return segment") << endl;
    cout << r->attribute("return segment") << endl;
    t->attributeIs("return segment", "");
    cout << t->attribute("return segment") << endl;
    cout << r->attribute("return segment") << endl;

    t = manager->instance("boatSeg1");
    r = manager->instance("boatSeg2");
    r->attributeIs("return segment", "boatSeg1");
    cout << t->attribute("return segment") << endl;
    cout << r->attribute("return segment") << endl;
    t->attributeIs("return segment", "truckSeg1");
    cout << t->attribute("return segment") << endl;
    cout << r->attribute("return segment") << endl;

    cerr << "Done!" << endl;

    return 0;
}

