#include <iostream>
#include <vector>
#include "stdlib.h"
#include "time.h"
#include "Instance.h"
#include "fwk/Exception.h"
#include "ActivityImpl.h"
#include "Log.h"

using namespace std;

class ExperimentNetwork {
  public:
    Ptr<Instance::Manager> manager;
    Ptr<Instance> destination;
    Ptr<Instance> root_terminal;
    vector<Ptr<Instance> > leaf_terminal;
    vector<Ptr<Instance> > segment;
    vector<Ptr<Instance> > source;
    ExperimentNetwork() {
        manager = shippingInstanceManager();
    }
    void setup() {
        try {
            destination = manager->instanceNew("destination", "Customer");
            root_terminal = manager->instanceNew("root_terminal", "Truck terminal");
            createSegmentPair("dest_seg", 0, "destination", "root_terminal");

            for (int i = 0; i < 10; i++) {
                stringstream term_name;
                term_name << "term" << i;
                leaf_terminal.push_back(manager->instanceNew(term_name.str(), "Truck terminal"));

                createSegmentPair("term_seg", i, term_name.str(), "root_terminal");

                for (int s = 0; s < 10; s++) {
                    stringstream source_name;
                    source_name << "source" << i*10 + s;
                    Ptr<Instance> src = manager->instanceNew(source_name.str(), "Customer");
                    source.push_back(src);

                    createSegmentPair("source_seg", i*10 + s, source_name.str(), term_name.str());
                }
            }
        } catch (Fwk::Exception e) {
            cout << e.what() << endl;
        } catch (...) {}
    }
    string explore() {
        Ptr<Instance> conn = manager->instanceNew("conn", "Conn");
        return conn->attribute("explore destination : distance 100000000");
    }
    void startShipments(bool random_shipment_size=false) {
        try {
            srand(time(NULL));
            for (int s = 0; s < 100; s++) {
                source[s]->attributeIs("destination", "destination");
                stringstream size_str;
                int size = random_shipment_size ? rand() % 1000 + 1 : 100;
                size_str << size;
                source[s]->attributeIs("shipment size", size_str.str());
                source[s]->attributeIs("transfer rate", "1");
            }
        } catch (Fwk::Exception e) {
            cout << e.what() << endl;
        } catch (...) {}
    }
    inline void createSegmentPair(const string basename, int name_suffix, string locA, string locB) {
        stringstream seg_name;
        seg_name << basename << name_suffix;
        Ptr<Instance> seg = manager->instanceNew(seg_name.str(), "Truck segment");
        seg->attributeIs("source", locA);
        segment.push_back(seg);

        stringstream retseg_name;
        retseg_name << basename << name_suffix << "_ret";
        Ptr<Instance> retseg = manager->instanceNew(retseg_name.str(), "Truck segment");
        retseg->attributeIs("source", locB);
        retseg->attributeIs("return segment", seg_name.str());
        segment.push_back(retseg);
    }
};

int main(int argc, char* argv[]) {
    ExperimentNetwork * network;
    try {
        CerrLog::instance()->logLevelIs(Log::Null);
        Activity::Manager::Ptr activityManager = activityManagerInstance();
        Ptr<Instance> stats;

        cout << "Simulation 1: all sources send 100 packages" << endl;
        network = new ExperimentNetwork();
        network->setup();
        network->startShipments();
        activityManager->nowIs(23.0);
        cout << "Shipments received at destination: " << network->destination->attribute("shipments received") << endl;
        cout << "Average latency of received shipments: " << network->destination->attribute("average latency") << endl;
        stats = network->manager->instanceNew("stats", "Stats");
        cout << "Average shipments received by segments: " << stats->attribute("average shipments received by segments") << endl;
        cout << "Average shipments refused by segments: " << stats->attribute("average shipments refused by segments") << endl;
        cout << "Total cost of received shipments: " << network->destination->attribute("total cost") << endl;
        if (network) delete network;
        cout << "========================================" << endl;
        activityManager = activityManagerInstance();
        activityManager = NULL;
        activityManagerInstanceIs(NULL);
        activityManager = activityManagerInstance();
        cout << "Simulation 2: all sources send rand(1, 1000) packages" << endl;
        network = new ExperimentNetwork();
        network->setup();
        network->startShipments(true);
        activityManager->nowIs(23.0);
        cout << "Shipments received at destination: " << network->destination->attribute("shipments received") << endl;
        cout << "Average latency of received shipments: " << network->destination->attribute("average latency") << endl;
        stats = network->manager->instanceNew("stats", "Stats");
        cout << "Average shipments received by segments: " << stats->attribute("average shipments received by segments") << endl;
        cout << "Average shipments refused by segments: " << stats->attribute("average shipments refused by segments") << endl;
        cout << "Total cost of received shipments: " << network->destination->attribute("total cost") << endl;
        delete network;
    } catch (Fwk::Exception e) {
        cerr << e.what() << endl;
    }
    return 0;
}
