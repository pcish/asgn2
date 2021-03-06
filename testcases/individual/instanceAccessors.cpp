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

    // 1. Setting segment attributes
    // Truck segments
    Ptr<Instance> us101n = manager->instance("US101N");
    us101n->attributeIs("source", "SFO");
    us101n->attributeIs("length", "32.57");

    Ptr<Instance> us101s = manager->instance("US101S");
    us101s->attributeIs("source", "PA");
    us101s->attributeIs("length", "32.61");

    us101n->attributeIs("return segment", "US101S");

    cout << "Segment: US101N" << endl;
    cout << us101n->attribute("source") << endl;
    cout << us101n->attribute("length") << endl;
    cout << us101n->attribute("return segment") << endl;
    cout << "Segment: US101S" << endl;
    cout << us101s->attribute("source") << endl;
    cout << us101s->attribute("length") << endl;
    cout << us101s->attribute("return segment") << endl;

    // Boat segments
    Ptr<Instance> boatSeg1 = manager->instance("BoatSeg1");
    boatSeg1->attributeIs("source", "SF");
    boatSeg1->attributeIs("length", "45.67");

    Ptr<Instance> boatSeg2 = manager->instance("BoatSeg2");
    boatSeg2->attributeIs("source", "SJC"); // Cannot connect boat seg to plane terminal
    boatSeg2->attributeIs("source", "Gates");  // Rectify this
    boatSeg2->attributeIs("length", "55.88");

    boatSeg1->attributeIs("return segment", "BoatSeg2");

    cout << "Segment: BoatSeg1" << endl;
    cout << boatSeg1->attribute("source") << endl;
    cout << boatSeg1->attribute("length") << endl;
    cout << boatSeg1->attribute("return segment") << endl;

    cout << "Segment: BoatSeg2" << endl;
    cout << boatSeg2->attribute("source") << endl;
    cout << boatSeg2->attribute("length") << endl;
    cout << boatSeg2->attribute("return segment") << endl;

    // Plane segments
    Ptr<Instance> planeSeg3 = manager->instance("PlaneSeg3");
    Ptr<Instance> planeSeg4 = manager->instance("PlaneSeg4");
    Ptr<Instance> planeSeg5 = manager->instanceNew("PlaneSeg5", "Plane segment");
    Ptr<Instance> planeSeg6 = manager->instanceNew("PlaneSeg6", "Plane segment");
    planeSeg3->attributeIs("source", "LAX");
    planeSeg3->attributeIs("length", "413.97");
    planeSeg4->attributeIs("source", "SJC");
    planeSeg4->attributeIs("length", "415.62");
    planeSeg5->attributeIs("source", "SJC");
    planeSeg5->attributeIs("length", "20.13");
    planeSeg6->attributeIs("source", "SF"); // Incorrect
    planeSeg6->attributeIs("source", "Stanford"); // Rectify
    planeSeg6->attributeIs("length", "20.58");

    cout << "Segment: PlaneSeg3" << endl;
    cout << planeSeg3->attribute("source") << endl;
    cout << planeSeg3->attribute("length") << endl;
    cout << "Segment: PlaneSeg4" << endl;
    cout << planeSeg4->attribute("source") << endl;
    cout << planeSeg4->attribute("length") << endl;
    cout << "Segment: PlaneSeg5" << endl;
    cout << planeSeg5->attribute("source") << endl;
    cout << planeSeg5->attribute("length") << endl;
    cout << "Segment: PlaneSeg6" << endl;
    cout << planeSeg6->attribute("source") << endl;
    cout << planeSeg6->attribute("length") << endl;

    cerr << "Done!" << endl;

    return 0;
}
