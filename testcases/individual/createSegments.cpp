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

    cerr << "Done!" << endl;

    return 0;
}
