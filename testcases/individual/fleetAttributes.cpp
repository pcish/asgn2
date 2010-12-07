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
    fleet->attributeIs("Truck, capacity", "50");
    fleet->attributeIs("Boat, speed", "60");
    fleet->attributeIs("Boat, capacity", "600");
    fleet->attributeIs("Plane, speed", "400");
    fleet->attributeIs("Plane, capacity", "150");

    cout << "===== Fleet attributes =====" << endl;
    cout << "[Truck]" << endl;
    cout << "Speed: " << fleet->attribute("Truck, speed") << endl;
    cout << "Capacity: " << fleet->attribute("Truck, capacity") << endl;
    cout << "[Boat]" << endl;
    cout << "Speed: " << fleet->attribute("Boat, speed") << endl;
    cout << "Capacity: " << fleet->attribute("Boat, capacity") << endl;
    cout << "[Plane]" << endl;
    cout << "Speed: " << fleet->attribute("Plane, speed") << endl;
    cout << "Capacity: " << fleet->attribute("Plane, capacity") << endl;

    cerr << "Done!" << endl;

    return 0;
}
