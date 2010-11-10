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
    fleet->attributeIs("Truck, speed", "-80");
    fleet->attributeIs("Truck, speed", "8.8");
    fleet->attributeIs("Truck, speed", "6bc");
    fleet->attributeIs("Truck, speed", "b5c");
    fleet->attributeIs("Truck, capacity", "5.5");
    fleet->attributeIs("Truck, capacity", "-50");
    fleet->attributeIs("Truck, cost", "6.6");
    fleet->attributeIs("Truck, cost", "-100");
    cout << "===== Fleet attributes =====" << endl;
    cout << "[Truck]" << endl;
    cout << "Speed: " << fleet->attribute("Truck, speed") << endl;
    cout << "Capacity: " << fleet->attribute("Truck, capacity") << endl;
    cout << "Cost: " << fleet->attribute("Truck, cost") << endl;

    Ptr<Instance> t = manager->instanceNew("ts2", "Truck segment");
    t->attributeIs("difficulty", "0.5");
    t->attributeIs("length", "0.5");
    t->attributeIs("length", "-5");
    cout << t->attribute("length") << endl;

    cerr << "Done!" << endl;

    return 0;
}
