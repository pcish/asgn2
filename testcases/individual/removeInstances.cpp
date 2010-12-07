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

void
segmentNotFound(string segmentName) {
    cout << "Segment not found: " << segmentName << endl;
}

void
extraSegmentFound(string segmentName) {
    cout << "Extra segment found: " << segmentName << endl;
}

void
checkExistingSegment(Ptr<Instance::Manager> manager, string segmentName) {
    if( manager->instance(segmentName) == NULL ) {
        segmentNotFound(segmentName);
    }
    else cout << "Segment found: " << segmentName << endl;
}

void
checkNonexistentSegment(Ptr<Instance::Manager> manager, string segmentName) {
    if( manager->instance(segmentName) != NULL ) {
        extraSegmentFound(segmentName);
    }
}

int main(int argc, char *argv[]) {
    Ptr<Instance::Manager> manager = shippingInstanceManager();

    Ptr<Instance> t = manager->instanceNew("US101N", "Truck segment");
    t = manager->instanceNew("US101S", "Truck segment");
    t = manager->instanceNew("BoatSeg1", "Boat segment");
    t = manager->instanceNew("BoatSeg2", "Boat segment");
    t = manager->instanceNew("PlaneSeg3", "Plane segment");
    t = manager->instanceNew("PlaneSeg4", "Plane segment");

    t = manager->instanceNew("SFO", "Port");
    t = manager->instanceNew("LAX", "Port");
    t = manager->instanceNew("SJC", "Plane terminal");
    t = manager->instanceNew("SF", "Boat terminal");
    t = manager->instanceNew("PA", "Truck terminal");
    t = manager->instanceNew("Stanford", "Customer");
    t = manager->instanceNew("Gates", "Customer");

    // 1. Check existing segments and locations:
    //    should output nothing
    checkExistingSegment(manager, "US101N");
    checkExistingSegment(manager, "US101S");
    checkExistingSegment(manager, "BoatSeg1");
    checkExistingSegment(manager, "BoatSeg2");
    checkExistingSegment(manager, "PlaneSeg3");
    checkExistingSegment(manager, "PlaneSeg4");

    checkNonexistentSegment(manager, "foo");
    checkNonexistentSegment(manager, "bar");
    checkNonexistentSegment(manager, "Boatseg3");
    checkNonexistentSegment(manager, "planeSeg1");
    checkNonexistentSegment(manager, "US101W");

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

    // 2. Delete some existing instances
    // rid of some segments
    manager->instanceDel("BoatSeg1");
    manager->instanceDel("PlaneSeg4");
    // rid of some locations
    manager->instanceDel("SF");
    manager->instanceDel("PA");

    // 3. Check existing segments and locations:
    //    should output segment / location not found 
    //    for deleted instances
    checkExistingSegment(manager, "US101N");
    checkExistingSegment(manager, "US101S");
    checkExistingSegment(manager, "BoatSeg1");
    checkExistingSegment(manager, "BoatSeg2");
    checkExistingSegment(manager, "PlaneSeg3");
    checkExistingSegment(manager, "PlaneSeg4");

    checkExistingLocation(manager, "SFO");
    checkExistingLocation(manager, "LAX");
    checkExistingLocation(manager, "SJC");
    checkExistingLocation(manager, "SF");
    checkExistingLocation(manager, "PA");
    checkExistingLocation(manager, "Stanford");
    checkExistingLocation(manager, "Gates");

    // 4. Delete some more instances
    manager->instanceDel("US101S");
    manager->instanceDel("BoatSeg2");
    manager->instanceDel("SJC");
    manager->instanceDel("Stanford");

    // 5. Check existing segments and locations:
    //    should output segment / location not found 
    //    for deleted instances
    checkExistingSegment(manager, "US101N");
    checkExistingSegment(manager, "US101S");
    checkExistingSegment(manager, "BoatSeg1");
    checkExistingSegment(manager, "BoatSeg2");
    checkExistingSegment(manager, "PlaneSeg3");
    checkExistingSegment(manager, "PlaneSeg4");

    checkExistingLocation(manager, "SFO");
    checkExistingLocation(manager, "LAX");
    checkExistingLocation(manager, "SJC");
    checkExistingLocation(manager, "SF");
    checkExistingLocation(manager, "PA");
    checkExistingLocation(manager, "Stanford");
    checkExistingLocation(manager, "Gates");

    cerr << "Done!" << endl;

    return 0;
}
