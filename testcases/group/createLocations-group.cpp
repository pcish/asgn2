#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void
locationNotFound(string locationName) {
    cout << "Location not found: " << locationName << endl;
}

void
extraLocationFound(string locationName) {
    cout << "Extra location found: " << locationName << endl;
}

void
checkExistingLocation(Ptr<Instance::Manager> manager, string locationName) {
    if( manager->instance(locationName) == NULL ) {
        locationNotFound(locationName);
    }
    else cout << "Location found: " << locationName << endl;
}

void
checkNonexistentLocation(Ptr<Instance::Manager> manager, string locationName) {
    if( manager->instance(locationName) != NULL ) {
        extraLocationFound(locationName);
    }
}

int main(int argc, char *argv[]) {
    Ptr<Instance::Manager> manager = shippingInstanceManager();

    Ptr<Instance> t = manager->instanceNew("SFO", "Port");
    t = manager->instanceNew("LAX", "Port");
    t = manager->instanceNew("SJC", "Plane terminal");
    t = manager->instanceNew("SF", "Boat terminal");
    t = manager->instanceNew("PA", "Truck terminal");
    t = manager->instanceNew("Stanford", "Customer");
    t = manager->instanceNew("Gates", "Customer");

    checkExistingLocation(manager, "SFO");
    checkExistingLocation(manager, "LAX");
    checkExistingLocation(manager, "SJC");
    checkExistingLocation(manager, "SF");
    checkExistingLocation(manager, "PA");
    checkExistingLocation(manager, "Stanford");
    checkExistingLocation(manager, "Gates");

    checkNonexistentLocation(manager, "SFO3");
    checkNonexistentLocation(manager, "LAX4");
    checkNonexistentLocation(manager, "Stanford2");
    checkNonexistentLocation(manager, "PA5");
    checkNonexistentLocation(manager, "SF9");
    checkNonexistentLocation(manager, "Utopia");

    cerr << "Done!" << endl;

    return 0;
}
