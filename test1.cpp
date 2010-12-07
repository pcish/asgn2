#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"
#include "fwk/Exception.h"
#include "ActivityImpl.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
 try {
    Activity::Manager::Ptr activityManagerInstance();
    Ptr<Instance::Manager> manager = shippingInstanceManager();

    Ptr<Instance> instance1 = manager->instanceNew("fleet1", "Fleet");
    Ptr<Instance> instance2 = manager->instanceNew("fleet2", "Fleet");
    if (instance1->name() != instance2->name()) {
        cout << "Fleet instance names do not match!" << endl;
    }
    instance1 = manager->instanceNew("conn1", "Conn");
    instance2 = manager->instanceNew("conn2", "Conn");
    if (instance1->name() != instance2->name()) {
        cout << "Conn instance names do not match!" << endl;
    }
    instance1 = manager->instanceNew("stats1", "Stats");
    instance2 = manager->instanceNew("stats2", "Stats");
    if (instance1->name() != instance2->name()) {
        cout << "Stats instance names do not match!" << endl;
    }
    instance2 = manager->instanceNew("conn2", "Fleet");
    cout << instance2->attribute("Truck, speed") << endl;

    instance1 = manager->instanceNew("truckSeg1", "Truck segment");
    instance2 = manager->instanceNew("truckSeg1", "Truck segment");
    if (instance2 != NULL) {
        cout << "duplicate instanceNew did not return NULL!" << endl;
    }

    cerr << "Done!" << endl;
  } catch (Fwk::Exception& e) {
      cout << e.what() << endl;
  }
    return 0;
}

