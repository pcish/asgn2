#include "Instance.h"
#include "fwk/Exception.h"
#include "ActivityImpl.h"
#include "Log.h"

using namespace std;

class Network {
  public:
    Ptr<Instance::Manager> manager;
    Ptr<Instance> destination;
    Ptr<Instance> root_terminal;
    vector<Ptr<Instance> > location;
    vector<Ptr<Instance> > segment;
    vector<Ptr<Instance> > source;
    Network() {
        manager = shippingInstanceManager();
    }
    void setup() {
        try {
            location.push_back(manager->instanceNew("location0", "Customer"));
            location.push_back(manager->instanceNew("location1", "Customer"));
            location.push_back(manager->instanceNew("location2", "Truck terminal"));
            location.push_back(manager->instanceNew("location3", "Truck terminal"));
            location.push_back(manager->instanceNew("location4", "Truck terminal"));
            location.push_back(manager->instanceNew("location5", "Truck terminal"));
            location.push_back(manager->instanceNew("location6", "Truck terminal"));
            location.push_back(manager->instanceNew("location7", "Truck terminal"));
            location.push_back(manager->instanceNew("location8", "Port"));
            location.push_back(manager->instanceNew("location9", "Customer"));

            createSegmentPair(0, "location0", "location1", "Plane segment");
            manager->instance("segment0")->attributeIs("capacity", "1");
            createSegmentPair(1, "location0", "location8", "Boat segment");
            createSegmentPair(2, "location1", "location8", "Boat segment");
            createSegmentPair(3, "location1", "location2", "Truck segment");
            createSegmentPair(4, "location2", "location3", "Truck segment");
            createSegmentPair(5, "location3", "location4", "Truck segment");
            createSegmentPair(6, "location4", "location5", "Truck segment");
            createSegmentPair(7, "location5", "location6", "Truck segment");
            createSegmentPair(8, "location6", "location7", "Truck segment");
            createSegmentPair(9, "location7", "location0", "Truck segment");
            Ptr<Instance> fleet = manager->instanceNew("fleet", "Fleet");
            fleet->attributeIs("Truck cost from 3 to 4", "100");
        } catch (Fwk::Exception e) {
            cout << e.what() << endl;
        } catch (...) {}
    }
    void startShipments() {
        location[0]->attributeIs("destination", "location1");
        stringstream size_str;
        int size = 100;
        size_str << size;
        location[0]->attributeIs("shipment size", size_str.str());
        location[0]->attributeIs("transfer rate", "10");

        location[1]->attributeIs("destination", "location9");
        location[1]->attributeIs("shipment size", "100");
        location[1]->attributeIs("transfer rate", "10");
    }
    inline void createSegmentPair(int name_index, string locA, string locB, const string segtype) {
        stringstream seg_name;
        seg_name << "segment" << name_index;
        Ptr<Instance> seg = manager->instanceNew(seg_name.str(), segtype);
        seg->attributeIs("source", locA);
        segment.push_back(seg);

        stringstream retseg_name;
        retseg_name << "segment" << name_index << "_ret";
        Ptr<Instance> retseg = manager->instanceNew(retseg_name.str(), segtype);
        retseg->attributeIs("source", locB);
        retseg->attributeIs("return segment", seg_name.str());
        segment.push_back(retseg);
    }
};

int main(int argc, char* argv[]) {
    CerrLog::instance()->logLevelIs(Log::Debug);
    Activity::Manager::Ptr activityManager = activityManagerInstance();

    cout << "Simulation 1: all sources send 100 packages" << endl;
    Network * network = new Network();
    network->setup();
    network->startShipments();
    activityManager->nowIs(47.0);
    cout << "Shipments received at destination: " << network->location[1]->attribute("shipments received") << endl;
    cout << "Average latency of received shipments: " << network->location[1]->attribute("average latency") << endl;
    delete network;
}

/*
     0 --- 1
   /  \   / \
 /      8     \
7              2
|              |
|       9      |
6              3
 \            /
   \        /
     5 --- 4
*/
