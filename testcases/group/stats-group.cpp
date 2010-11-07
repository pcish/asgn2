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

    Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");

    // Segments
    Ptr<Instance> t = manager->instanceNew("ts1", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts2", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts3", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts4", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts5", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts6", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts7", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts8", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts9", "Truck segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ts10", "Truck segment");
    t->attributeIs("expedite support", "no");
    
    t = manager->instanceNew("bs1", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs2", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs3", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs4", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs5", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs6", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs7", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs8", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs9", "Boat segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("bs10", "Boat segment");
    t->attributeIs("expedite support", "no");

    t = manager->instanceNew("ps1", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps2", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps3", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps4", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps5", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps6", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps7", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps8", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps9", "Plane segment");
    t->attributeIs("expedite support", "no");
    t = manager->instanceNew("ps10", "Plane segment");
    t->attributeIs("expedite support", "no");

    // Terminals
    t = manager->instanceNew("tt1", "Truck terminal");
    t = manager->instanceNew("tt2", "Truck terminal");
    t = manager->instanceNew("tt3", "Truck terminal");
    t = manager->instanceNew("tt4", "Truck terminal");
    t = manager->instanceNew("tt5", "Truck terminal");
    t = manager->instanceNew("tt6", "Truck terminal");
    t = manager->instanceNew("tt7", "Truck terminal");
    t = manager->instanceNew("tt8", "Truck terminal");
    t = manager->instanceNew("tt9", "Truck terminal");
    t = manager->instanceNew("tt10", "Truck terminal");
    
    t = manager->instanceNew("bt1", "Boat terminal");
    t = manager->instanceNew("bt2", "Boat terminal");
    t = manager->instanceNew("bt3", "Boat terminal");
    t = manager->instanceNew("bt4", "Boat terminal");
    t = manager->instanceNew("bt5", "Boat terminal");
    t = manager->instanceNew("bt6", "Boat terminal");
    t = manager->instanceNew("bt7", "Boat terminal");
    t = manager->instanceNew("bt8", "Boat terminal");
    t = manager->instanceNew("bt9", "Boat terminal");
    t = manager->instanceNew("bt10", "Boat terminal");

    t = manager->instanceNew("pt1", "Plane terminal");
    t = manager->instanceNew("pt2", "Plane terminal");
    t = manager->instanceNew("pt3", "Plane terminal");
    t = manager->instanceNew("pt4", "Plane terminal");
    t = manager->instanceNew("pt5", "Plane terminal");
    t = manager->instanceNew("pt6", "Plane terminal");
    t = manager->instanceNew("pt7", "Plane terminal");
    t = manager->instanceNew("pt8", "Plane terminal");
    t = manager->instanceNew("pt9", "Plane terminal");
    t = manager->instanceNew("pt10", "Plane terminal");

    // Ports
    t = manager->instanceNew("port1", "Port");
    t = manager->instanceNew("port2", "Port");
    t = manager->instanceNew("port3", "Port");
    t = manager->instanceNew("port4", "Port");
    t = manager->instanceNew("port5", "Port");
    t = manager->instanceNew("port6", "Port");
    t = manager->instanceNew("port7", "Port");
    t = manager->instanceNew("port8", "Port");
    t = manager->instanceNew("port9", "Port");
    t = manager->instanceNew("port10", "Port");

    // Customers
    t = manager->instanceNew("customer1", "Customer");
    t = manager->instanceNew("customer2", "Customer");
    t = manager->instanceNew("customer3", "Customer");
    t = manager->instanceNew("customer4", "Customer");
    t = manager->instanceNew("customer5", "Customer");
    t = manager->instanceNew("customer6", "Customer");
    t = manager->instanceNew("customer7", "Customer");
    t = manager->instanceNew("customer8", "Customer");
    t = manager->instanceNew("customer9", "Customer");
    t = manager->instanceNew("customer10", "Customer");

    // Expediting support
    t = manager->instance("ts8");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("ts9");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("ts10");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("bs7");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("bs8");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("bs9");
    t->attributeIs("expedite support", "yes");
    t = manager->instance("ps5");
    t->attributeIs("expedite support", "yes");

    cout << "===== Stats attributes (before disabling expedite support) =====" << endl;
    cout << " --- Segments --- " << endl;
    cout << "# Truck segments : " << stats->attribute("Truck segment") << endl;
    cout << "# Boat segments  : " << stats->attribute("Boat segment") << endl;
    cout << "# Plane segments : " << stats->attribute("Plane segment") << endl;
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;
    cout << " --- Terminals --- " << endl;
    cout << "# Truck terminals: " << stats->attribute("Truck terminal") << endl;
    cout << "# Boat terminals : " << stats->attribute("Boat terminal") << endl;
    cout << "# Plane terminals: " << stats->attribute("Plane terminal") << endl;
    cout << " --- Ports and customers --- " << endl;
    cout << "# Ports          : " << stats->attribute("Port") << endl;
    cout << "# Customers      : " << stats->attribute("Customer") << endl;

    // Disable some expedite support
    t = manager->instance("ts9");
    t->attributeIs("expedite support", "no");
    t = manager->instance("bs7");
    t->attributeIs("expedite support", "no");

    cout << "===== Stats attributes (after disabling expedite support) =====" << endl;
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;

    // Delete instances
    // Segments
    manager->instanceDel("ts8");
    manager->instanceDel("bs1");
    manager->instanceDel("bs7");
    manager->instanceDel("bs9");
    // Terminals
    manager->instanceDel("tt1");
    manager->instanceDel("tt6");
    manager->instanceDel("bt4");
    manager->instanceDel("bt1");
    manager->instanceDel("bt7");
    manager->instanceDel("bt9");
    manager->instanceDel("bt8");
    manager->instanceDel("pt10");
    manager->instanceDel("pt3");
    manager->instanceDel("pt2");
    manager->instanceDel("pt5");
    manager->instanceDel("pt1");
    manager->instanceDel("pt6");
    manager->instanceDel("pt9");
    // Ports
    manager->instanceDel("port5");
    manager->instanceDel("port8");
    manager->instanceDel("port1");
    manager->instanceDel("port2");
    manager->instanceDel("port4");
    manager->instanceDel("port9");
    // Customers
    manager->instanceDel("customer4");
    manager->instanceDel("customer2");
    manager->instanceDel("customer1");
    manager->instanceDel("customer6");

    cout << "===== Stats attributes (after deleting instances) =====" << endl;
    cout << " --- Segments --- " << endl;
    cout << "# Truck segments : " << stats->attribute("Truck segment") << endl;
    cout << "# Boat segments  : " << stats->attribute("Boat segment") << endl;
    cout << "# Plane segments : " << stats->attribute("Plane segment") << endl;
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;
    cout << " --- Terminals --- " << endl;
    cout << "# Truck terminals: " << stats->attribute("Truck terminal") << endl;
    cout << "# Boat terminals : " << stats->attribute("Boat terminal") << endl;
    cout << "# Plane terminals: " << stats->attribute("Plane terminal") << endl;
    cout << " --- Ports and customers --- " << endl;
    cout << "# Ports          : " << stats->attribute("Port") << endl;
    cout << "# Customers      : " << stats->attribute("Customer") << endl;

    cerr << "Done!" << endl;

    return 0;
}
