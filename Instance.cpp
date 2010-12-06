#include <iostream>
#include <stdlib.h>
#include <map>
#include <sstream>

#include "assert.h"
#include "Instance.h"
#include "entities.h"
#include "ShippingNetwork.h"
#include "fwk/Exception.h"
#include "ActivityImpl.h"

#include "Log.h"

namespace Shipping {

using namespace std;

//
// Rep layer classes
//

class ManagerImpl : public Instance::Manager {
public:
    ManagerImpl();

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    template <typename T, typename TRep>
    Ptr<T> cast_instance(const string& v);

    // Manager method
    void instanceDel(const string& name);
    ShippingNetwork* shippingNetwork() { return shippingNetwork_; }
    ~ManagerImpl();

private:
    map<string,Ptr<Instance> > instance_;
    ShippingNetwork* shippingNetwork_;
    bool userCreatedStats;
    bool userCreatedConn;
    bool userCreatedFleet;
};

class SegmentRep;

class LocationRep : public Instance {
  public:
    virtual ~LocationRep() { }
    virtual string attribute(const string& name);
    virtual void attributeIs(const string& name, const string& v);

    virtual Ptr<Location> engineObject() const = 0;
  protected:
    LocationRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {
    }
    ManagerImpl* manager_;

  private:
    int segmentNumber(const string& name);
};

class PortRep : public LocationRep {
  public:
    PortRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->shippingNetwork()->portNew(name);
    }
    ~PortRep() { manager_->shippingNetwork()->portDel(engineObject_); }
    virtual Ptr<Location> engineObject() const { return engineObject_; }

  protected:
    Ptr<Port> engineObject_;
};

class CustomerRep : public LocationRep {
  public:
    CustomerRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->shippingNetwork()->customerNew(name);
    }
    virtual string attribute(const string& name);
    virtual void attributeIs(const string& name, const string& v);
    ~CustomerRep() { manager_->shippingNetwork()->customerDel(engineObject_); }
    virtual Ptr<Location> engineObject() const { return engineObject_; }

  protected:
    Ptr<Customer> engineObject_;
};

class TerminalRep : public LocationRep {
  public:
    TerminalRep(const string& name, ManagerImpl *manager, Segment::TransportationMode _mode) :
        LocationRep(name, manager), mode_(_mode) {
          engineObject_ = manager_->shippingNetwork()->terminalNew(name, _mode);
    }
    ~TerminalRep() { manager_->shippingNetwork()->terminalDel(engineObject_); }
    virtual Ptr<Location> engineObject() const { return engineObject_; }

  protected:
    Ptr<Terminal> engineObject_;
    Segment::TransportationMode mode_;
};

class SegmentRep : public Instance {
  public:
    SegmentRep(const string& name, ManagerImpl* manager, Segment::TransportationMode _mode) :
        Instance(name), manager_(manager), mode_ (_mode) {
        engineObject_ = manager_->shippingNetwork()->segmentNew(_mode, name);
    }
    ~SegmentRep() { manager_->shippingNetwork()->segmentDel(engineObject_); }

    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

    Ptr<Segment> engineObject() const { return engineObject_; }

  protected:
    ManagerImpl* manager_;

  private:
    Ptr<Segment> engineObject_;
    int segmentNumber(const string& name);
    Segment::TransportationMode mode_;
};

class StatsRep : public Instance {
  public:
    static Ptr<StatsRep> instance(const string& name, ManagerImpl *manager) {
        if (instance_ == NULL) {
            instance_ = new StatsRep(name, manager);
        }
        return instance_;
    }
    static void instanceIs(Ptr<StatsRep> _instance) { instance_ = _instance; }
    ~StatsRep() { instance_ = NULL; }

    string attribute(const string &name);
    void attributeIs(const string& name, const string& v) {} // do nothing: quietly ignore the write function

  protected:
    StatsRep(const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager) {}
    ManagerImpl* manager_;

  private:
    static Ptr<StatsRep> instance_;
};
Ptr<StatsRep> StatsRep::instance_ = NULL;

class ConnRep : public Instance {
  public:
    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);
    static Ptr<ConnRep> instance(const string &name, ManagerImpl *manager) {
        if (instance_ == NULL)
            instance_ = new ConnRep(name, manager);
        return instance_;
    }
    static void instanceIs(Ptr<ConnRep> _instance) { instance_ = _instance; }
    ~ConnRep() { instance_ = NULL; }

  protected:
    ConnRep (const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager) { }
    ManagerImpl* manager_;

  private:
    static Ptr<ConnRep> instance_;
};
Ptr<ConnRep> ConnRep::instance_ = NULL;

class FleetRep : public Instance {
  public:
    static Ptr<FleetRep> instance(const string &name, ManagerImpl *manager) {
        if (instance_ == NULL)
            instance_ = new FleetRep(name, manager);
        return instance_;
    }
    static void instanceIs(Ptr<FleetRep> _instance) { instance_ = _instance; }

    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);
    ~FleetRep() { cout << "fleetrep destructor" << endl; instance_ = NULL; }

  protected:
    FleetRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {
        manager->shippingNetwork()->truckFleetNew(name+"_truck");
        manager->shippingNetwork()->planeFleetNew(name+"_plane");
        manager->shippingNetwork()->boatFleetNew(name+"_boat");
    }

  private:
    static Ptr<FleetRep> instance_;
    ManagerImpl* manager_;
};
Ptr<FleetRep> FleetRep::instance_ = NULL;

ManagerImpl::ManagerImpl() {
    shippingNetwork_ = new ShippingNetwork();
    instance_["defaultFleet"] = FleetRep::instance("defaultFleet", this);
    userCreatedFleet = false;
    instance_["defaultStats"] = StatsRep::instance("defaultStats", this);
    userCreatedStats = false;
    instance_["defaultConn"] = ConnRep::instance("defaultConn", this);
    userCreatedConn = false;
}

ManagerImpl::~ManagerImpl() {
    instance_.clear();
    
    /*for (map<string, Ptr<Instance> >::iterator i = instance_.begin(); i != instance_.end(); i++) {
        (*i).second->deleteRef();
        instance_.erase(i);
    }
    */
    FleetRep::instanceIs(NULL);
    ConnRep::instanceIs(NULL);
    StatsRep::instanceIs(NULL);
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    Ptr<Instance> t;
    if (type == "Stats") {
        if (!userCreatedStats) {
            instanceDel("defaultStats");
            userCreatedStats = true;
        }
        t = StatsRep::instance(name, this);
    } else if (type == "Fleet") {
        if (!userCreatedFleet) {
            instanceDel("defaultFleet");
            userCreatedStats = true;
        }
        t = FleetRep::instance(name, this);
    } else if (type == "Conn") {
        if (!userCreatedConn) {
            instanceDel("defaultConn");
            userCreatedConn = true;
        }
        t = ConnRep::instance(name, this);
    }
    if (t != NULL) {
        instance_[t->name()] = t;
        return t;
    }
    if (instance_.find(name) != instance_.end()) {
//        cerr << "Attempt to new instances of the same names!" << endl;
        throw Fwk::NameInUseException("Attempt to new instances of the same name: "+name);
//        return NULL;
    }
    if (type == "Truck terminal") {
        t = new TerminalRep(name, this, Segment::truck());
    } else if (type == "Truck segment") {
        t = new SegmentRep(name, this, Segment::truck());
    } else if (type == "Boat terminal") {
        t = new TerminalRep(name, this, Segment::boat());
    } else if (type == "Boat segment") {
        t = new SegmentRep(name, this, Segment::boat());
    } else if (type == "Plane terminal") {
        t = new TerminalRep(name, this, Segment::plane());
    } else if (type == "Plane segment") {
        t = new SegmentRep(name, this, Segment::plane());
    } else if (type == "Port") {
        t = new PortRep(name, this);
    } else if (type == "Customer") {
        t = new CustomerRep(name, this);
    } else {
        //No matching type
        throw Fwk::UnknownArgException("Unsupported type: " + type);
//        return NULL;
    }
    instance_[t->name()] = t;
    return t;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
    map<string,Ptr<Instance> >::iterator t = instance_.find(name);
    if (t != instance_.end()) {
        while (instance_[name]->references() > 1) instance_[name]->referencesDec();
        instance_.erase(t);
    } else {
//        cerr << "attempting to delete non-existant instance " << name << endl;
    }
}

template <typename T, typename TRep>
Ptr<T> ManagerImpl::cast_instance(const string& v) {
    Ptr<Instance> ins = instance(v);
    if (ins == NULL)
        throw Fwk::EntityNotFoundException("Instance lookup for "+v + " failed");
        //cerr << "instance lookup for " << v << " failed" << endl;
    return Ptr<TRep>((TRep*) ins.ptr())->engineObject();
}

string LocationRep::attribute(const string& name) {
    int i = segmentNumber(name);
    if (i >= 0) {
        if (engineObject()->segment(i) != NULL) {
            return engineObject()->segment(i)->name();
        }
    }
    return "";
}

void LocationRep::attributeIs(const string& name, const string& v) {
    //location has no writable attributes
}

string CustomerRep::attribute(const string& name) {
    ostringstream os;
    if (name == "destination") {
        if (engineObject_->destination() != NULL) {
            os << engineObject_->destination()->name();
        }
    } else if (name == "shipment size") {
        os << engineObject_->shipmentSize().value();
    } else if (name == "shipments received") {
        os << engineObject_->shipmentsReceived().value();
    } else if (name == "average latency") {
        os << engineObject_->averageLatency().value();
    } else if (name == "total cost") {
        os << engineObject_->totalCost().value();
    } else {
        throw Fwk::UnknownArgException("Unsupported attribute: " + name);
    }
    return os.str();
}

void CustomerRep::attributeIs(const string& name, const string& v) {
    if (name == "destination") {
        if (v != "") {
            Ptr<Location> location = manager_->cast_instance<Location, LocationRep>(v);
            engineObject_->destinationIs(location);
        }
        else engineObject_->destinationIs(NULL);
    } else if (name == "shipment size") {
        engineObject_->shipmentSizeIs(PackageUnit(atoi(v.c_str())));
    } else if (name == "transfer rate") {
        engineObject_->transferRateIs(ShipmentCount(atoi(v.c_str())));
    } else {
        throw Fwk::UnknownArgException("Unsupported attribute: " + name);
    }
}

string SegmentRep::attribute(const string& name) {
    ostringstream os;
    if (name == "source") {
        if (engineObject_->source() != NULL) {
            os << engineObject_->source()->name();
        }
    } else if (name == "length") {
        os.setf(ios::fixed,ios::floatfield);
        os.precision(2);
        os << engineObject_->length().value();
    } else if (name == "return segment") {
        if (engineObject_->returnSegment() != NULL) {
            os << engineObject_->returnSegment()->name();
        }
    } else if (name == "difficulty") {
        os.setf(ios::fixed,ios::floatfield);
        os.precision(2);
        os << engineObject_->difficulty().value();
    } else if (name == "expedite support") {
        Segment::ExpediteSupport support = engineObject_->expediteSupport();
        if (support == Segment::available()) os << "yes";
        else if (support == Segment::unavailable()) os << "no";
    } else if (name == "shipments refused") {
        os << engineObject_->shipmentsRefused().value();
    } else if (name == "shipments received") {
        os << engineObject_->shipmentsReceived().value();
    } else if (name == "capacity") {
        os << engineObject_->capacity().value();
    } else {
        LOG_CRITICAL("attribute", "Unsupported attribute: " + name);
        throw Fwk::UnknownArgException("Unsupported attribute: " + name);
    }
    return os.str();
}

void SegmentRep::attributeIs(const string& name, const string& v) {
    if (name == "source") {
        if (v != "") {
            Ptr<Location> location = manager_->cast_instance<Location, LocationRep>(v);
            engineObject_->sourceIs(location);
        }
        else engineObject_->sourceIs(NULL);
    } else if (name == "length") {
        engineObject_->lengthIs(atof(v.c_str()));
    } else if (name == "return segment") {
        if (v != "") {
            Ptr<Segment> segment = manager_->cast_instance<Segment, SegmentRep>(v);
            engineObject_->returnSegmentIs(segment);
        }
        else engineObject_->returnSegmentIs(NULL);
    } else if (name == "difficulty") {
        engineObject_->difficultyIs(atof(v.c_str()));
    } else if (name == "expedite support") {
        if (v == "yes") engineObject_->expediteSupportIs(Segment::available());
        else if (v == "no") engineObject_->expediteSupportIs(Segment::unavailable());
    } else if (name == "capacity") {
        engineObject_->capacityIs(ShipmentCount(atoi(v.c_str())));
    } else {
        throw Fwk::UnknownArgException("Unsupported attribute: " + name);
    }
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t) - 1;
    }
    return -1;
}

string StatsRep::attribute(const string& name) {
    ostringstream os;
    if (name == "Customer") {
        os << manager_->shippingNetwork()->statistics()->customers();
    } else if (name == "Truck terminal") {
        os << manager_->shippingNetwork()->statistics()->truckTerminals();
    } else if (name == "Plane terminal") {
        os << manager_->shippingNetwork()->statistics()->planeTerminals();
    } else if (name == "Boat terminal") {
        os << manager_->shippingNetwork()->statistics()->boatTerminals();
    } else if (name == "Port") {
        os << manager_->shippingNetwork()->statistics()->ports();
    } else if (name == "Truck segment") {
        os << manager_->shippingNetwork()->statistics()->truckSegments();
    } else if (name == "Plane segment") {
        os << manager_->shippingNetwork()->statistics()->planeSegments();
    } else if (name == "Boat segment") {
        os << manager_->shippingNetwork()->statistics()->boatSegments();
    } else if (name == "expedite percentage") {
        int expediteAvailable = 0;
        manager_->shippingNetwork()->statistics()->expediteIs(Segment::available());
        expediteAvailable += manager_->shippingNetwork()->statistics()->truckSegments();
        expediteAvailable += manager_->shippingNetwork()->statistics()->planeSegments();
        expediteAvailable += manager_->shippingNetwork()->statistics()->boatSegments();
        int totalSegments = 0;
        manager_->shippingNetwork()->statistics()->expediteIs(Segment::allAvailabilities());
        totalSegments += manager_->shippingNetwork()->statistics()->truckSegments();
        totalSegments += manager_->shippingNetwork()->statistics()->planeSegments();
        totalSegments += manager_->shippingNetwork()->statistics()->boatSegments();
        os.setf(ios::fixed,ios::floatfield);
        os.precision(2);
        os << ((float) expediteAvailable / (float) totalSegments) * 100.0;
    } else {
        throw Fwk::UnknownArgException("Unsupported attribute: " + name);
        //return "";
    }
    return os.str();
}

void ConnRep::attributeIs(const string& name, const string& v) {
    if (name.compare("routing") == 0) {
        Ptr<ShippingNetwork> network = manager_->shippingNetwork();
        if (v.compare("dijkstra") == 0) {
            network->routingIs(ShippingNetwork::dijkstra());
        }
        else if (v.compare("bfs") == 0) {
            network->routingIs(ShippingNetwork::bfs());
        }
        else if (v.compare("randomwalk") == 0) {
            network->routingIs(ShippingNetwork::randomwalk());
        }
    }
}
string ConnRep::attribute(const string& name) {
    istringstream is(name);
    ostringstream os;
    string cmd, attr, val, token, source, dest, dummy;
    Segment::ExpediteSupport expedite = Segment::unavailable();
    Mile maxDistance;
    USD maxCost;
    Hour maxTime;

    Ptr<ShippingNetwork> network = manager_->shippingNetwork();

    is >> cmd;
    if (cmd.compare("explore") == 0) {
        is >> source;
        is >> dummy; // colon
        while (!is.eof ()) {
            is >> attr;
            if (attr.compare("expedited") == 0)
                expedite = Segment::available();
            else {
                //is >> val;
                if (attr.compare("distance") == 0) {
                    double raw;
                    is >> raw;
                    maxDistance = raw;
                }
                else if (attr.compare("cost") == 0) {
                    double raw;
                    is >> raw;
                    maxCost = raw;
                }
                else if (attr.compare("time") == 0) {
                    double raw;
                    is >> raw;
                    maxTime = raw;
                }
            }
        }
        network->sourceIs(Ptr<LocationRep>((LocationRep*) manager_->
                          instance(source).ptr())->engineObject()
        );
        network->destinationIs(NULL);
        network->expediteIs(expedite);
        network->maxCostIs(maxCost);
        network->maxDistanceIs(maxDistance);
        network->maxTimeIs(maxTime);
    }
    else if (cmd.compare("connect") == 0) {
        Ptr<ShippingNetwork> network = manager_->shippingNetwork();
        is >> source >> dummy >> dest;
        network->sourceIs(Ptr<LocationRep>((LocationRep*)manager_->
                          instance(source).ptr() )->engineObject()
        );
        network->destinationIs(Ptr<LocationRep>((LocationRep*)manager_->
                               instance(dest).ptr() )->engineObject()
        );
        network->maxCostIs(0);
        network->maxDistanceIs(0);
        network->maxTimeIs(0);
    }
    else if (cmd.compare("routing") == 0) {
        ShippingNetwork::Routing r = network->routing();
        switch (r){
            case ShippingNetwork::dijkstra__:
                return "Dijkstra";
            case ShippingNetwork::bfs__:
                return "BFS";
            default:
            //this shouldn't happen
                return "Unspecified";
        }
    }
    os.setf(ios::fixed, ios::floatfield);
    os.precision(2);
    for (unsigned int i = 0; i < network->paths(); i++) {
        Ptr<Path> path = network->path(i);
        if (path) {
            if (dest != "") {
                os << path->cost().value() << " " << path->hour().value() << " " <<
                ((path->expedite() == Segment::available()) ? "yes" : "no") << "; ";
            }
            for (unsigned int j = 0; j < path->locations(); j++) {
                Ptr<Location> loc = path->location(j);
                os << loc->name();
                if (j != path->locations() - 1) {
                    WeakPtr<Segment> seg = path->segment(j);
                    os << "(" << seg->name() << ":" << seg->length().value() <<
                    ":" << seg->returnSegment()->name() << ") ";
                }
            }
            if (i < network->paths() - 1) os << endl;
        }
    }
    return os.str();
}

string FleetRep::attribute(const string& name) {
    int commaPos = name.find_first_of(',');
    string mode = name.substr(0, commaPos);
    string property = name.substr(commaPos + 1, name.length());
    //trim the string
    mode = mode.substr(mode.find_first_not_of(' '));
    mode = mode.substr(0, mode.find_last_not_of (' ') + 1);
    property = property.substr(property.find_first_not_of(' ') );
    property = property.substr(0, property.find_last_not_of(' ') + 1);
    Ptr<Fleet> fleet_;
    if (mode == "Truck") fleet_ = manager_->shippingNetwork()->truckFleet();
    if (mode == "Plane") fleet_ = manager_->shippingNetwork()->planeFleet();
    if (mode == "Boat") fleet_ = manager_->shippingNetwork()->boatFleet();

    ostringstream os;
    os.setf(ios::fixed,ios::floatfield);
    os.precision(2);
    if (property == "speed") {
        os << fleet_->speed().value();
    } else if (property == "cost") {
        os << fleet_->cost().value();
    } else if (property == "capacity") {
        os << fleet_->capacity().value();
    } else {
        return "";
    }
    return os.str();
}

void FleetRep::attributeIs(const string& name, const string& v) {
    //try {
        int commaPos = name.find_first_of(',');
        string mode = name.substr(0, commaPos);
        string property = name.substr(commaPos + 1, name.length());
        //trim the string
        mode = mode.substr(mode.find_first_not_of(' '));
        mode = mode.substr(0, mode.find_last_not_of (' ') + 1);
        property = property.substr(property.find_first_not_of(' '));
        property = property.substr(0, property.find_last_not_of(' ') + 1);
        Ptr<Fleet> fleet_;
        if (mode == "Truck") fleet_ = manager_->shippingNetwork()->truckFleet();
        if (mode == "Plane") fleet_ = manager_->shippingNetwork()->planeFleet();
        if (mode == "Boat") fleet_ = manager_->shippingNetwork()->boatFleet();


        istringstream is(v);
        istringstream issprop(property);
        vector<string> propList;
        string temp;
        while(issprop >> temp) {
            propList.push_back(temp);
        }

        if (propList.size() == 1) {
            string propertyValue;
            is >> propertyValue;
            if (propList[0] == "speed") {
                fleet_->speedIs(Mile(atof(propertyValue.c_str())));
            }
            if (propList[0] == "cost") {
                fleet_->costIs(USD(atof(propertyValue.c_str())));
            }
            if (propList[0] == "capacity") {
                fleet_->capacityIs(PackageUnit(atoi(propertyValue.c_str())));
            }
        }
        else if (propList.size() == 5) {
            if (propList[1] == "from" && propList[3] == "to") {
                string propertyValue;
                is >> propertyValue;
                int from, to;
                from = atoi(propList[2].c_str());
                to = atoi(propList[4].c_str());
                //cout << propList[0] << " from " << from << " to " << to << endl;
                //LOG_DEBUG("FleetRep::attributeIs", "scheduled " + mode);
                for (int i = from; i <= to; i ++) {
                    if (propList[0] == "speed") {
                        fleet_->scheduledSpeedIs(i, Mile(atof(propertyValue.c_str())));
                    }
                    if (propList[0] == "cost") {
                        fleet_->scheduledCostIs(i, USD(atof(propertyValue.c_str())));
                    }
                    if (propList[0] == "capacity") {
                        fleet_->scheduledCapacityIs(i, PackageUnit(atoi(propertyValue.c_str())));
                    }
                }
            }
        }

        //} catch (ValueError e) {
        //    cerr << e.what() << endl;
        //}
}

}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
    return new Shipping::ManagerImpl();
}
