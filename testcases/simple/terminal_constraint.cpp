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
    Ptr<Instance> term = manager->instanceNew("term", "Truck terminal");
    Ptr<Instance> t;
    t = manager->instanceNew("truckSeg1", "Truck segment");
    t->attributeIs("source", "term");
    t = manager->instanceNew("boatSeg1", "Boat segment");
    t->attributeIs("source", "term");
    t = manager->instanceNew("planeSeg1", "Plane segment");
    t->attributeIs("source", "term");

    cout << "Before removing segments from term:" << endl;
    cout << term->attribute("segment1") << endl;
    cout << term->attribute("segment2") << endl;
    cout << term->attribute("segment3") << endl;

    Ptr<Instance> port = manager->instanceNew("port", "Port");
    t = manager->instance("boatSeg1");
    t->attributeIs("source", "port");
    t->attributeIs("return segment", "truckSeg1");

    cout << "After setting an invalid return segment:" << endl;
    cout << term->attribute("segment1") << endl;
    cout << term->attribute("segment2") << endl;
    cout << term->attribute("segment3") << endl;
    cout << term->attribute("segment4") << endl;
    cout << t->attribute("return segment") << endl;
    t = manager->instance("truckSeg1");
    cout << t->attribute("return segment") << endl;

    cerr << "Done!" << endl;

    return 0;
}

