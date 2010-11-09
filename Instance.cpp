#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <sstream>


#include "Instance.h"
#include "entities.h"
#include "Engine.h"


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
    EngineManager* engineManager() { return engineManager_; }

private:
    map<string,Ptr<Instance> > instance_;
    EngineManager* engineManager_;
};

class SegmentRep;

class LocationRep : public Instance {
public:
    virtual string attribute(const string& name);
    virtual void attributeIs(const string& name, const string& v);

    Ptr<Location> engineObject() const { return engineObject_; }
  protected:
    LocationRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {
    }
    Ptr<ManagerImpl> manager_;
    Ptr<Location> engineObject_;

  private:
    int segmentNumber(const string& name);
    //Ptr<Location> engineObject_;
};

class PortRep : public LocationRep {
  public:
    PortRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->engineManager()->portNew(name);
    }
    //Ptr<Port> engineObject() const { return engineObject_; }

  //private:
  //  Ptr<Port> engineObject_;
};

class CustomerRep : public LocationRep {
  public:
    CustomerRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->engineManager()->customerNew(name);
    }
    //Ptr<Customer> engineObject() const { return engineObject_; }

  //private:
  //  Ptr<Customer> engineObject_;
};

class TerminalRep : public LocationRep {
  public:
    TerminalRep(const string& name, ManagerImpl *manager, Segment::TransportationMode _mode) :
        LocationRep(name, manager), mode_(_mode) {
          engineObject_ = manager_->engineManager()->terminalNew(name, _mode);
    }
    //Ptr<Terminal> engineObject() const { return engineObject_; }

  protected:
    Segment::TransportationMode mode_;

  //private:
   // Ptr<Terminal> engineObject_;
};

class SegmentRep : public Instance {
  public:
    SegmentRep(const string& name, ManagerImpl* manager, Segment::TransportationMode _mode) :
        Instance(name), manager_(manager), mode_ (_mode) {
        engineObject_ = manager_->engineManager()->segmentNew(_mode, name);
    }

    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

    Ptr<Segment> engineObject() const { return engineObject_; }

  protected:
    Ptr<ManagerImpl> manager_;

  private:
    Ptr<Segment> engineObject_;
    int segmentNumber(const string& name);
    Segment::TransportationMode mode_;
};

class StatsRep : public Instance {
  public:
    static Ptr<StatsRep> instance(const string& name, ManagerImpl *_manager) {
        if (instance_ == NULL)
            instance_ = new StatsRep(name, _manager);
        return instance_;
    }

    string attribute(const string &name);
    void attributeIs(const string& name, const string& v) {} // do nothing: quietly ignore the write function

  protected:
    StatsRep(const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager) {}
    Ptr<ManagerImpl> manager_;

  private:
    static Ptr<StatsRep> instance_;
};
Ptr<StatsRep> StatsRep::instance_ = NULL;

class ConnRep : public Instance {
  public:
    string attribute(const string& name);
    void attributeIs(const string& name, const string& v) {}
    static Ptr<ConnRep> instance(const string &name, ManagerImpl *manager) {
        if (instance_ == NULL)
            instance_ = new ConnRep (name, manager);
        return instance_;
    }

  protected:
    ConnRep (const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager) { }
    Ptr<ManagerImpl> manager_;

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

    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

  protected:
    FleetRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {
        truckfleet_ = manager->engineManager()->truckFleetNew();
        planefleet_ = manager->engineManager()->planeFleetNew();
        boatfleet_ = manager->engineManager()->boatFleetNew();
    }
    Ptr<Fleet> truckfleet_, boatfleet_, planefleet_;

  private:
    static Ptr<FleetRep> instance_;
    Ptr<ManagerImpl> manager_;
};
Ptr<FleetRep> FleetRep::instance_ = NULL;

ManagerImpl::ManagerImpl() {
    engineManager_ = new EngineManager();
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if (instance_.find(name) != instance_.end()) {
        cerr << "Attempt to new instances of the same names!" << endl;
        return NULL;
    }
    Ptr<Instance> t;
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
    } else if (type == "Stats") {
        t = StatsRep::instance(name, this);
    } else if (type == "Fleet") {
        t = FleetRep::instance(name, this);
    } else if (type == "Conn") {
        t = ConnRep::instance(name, this);
    } else {
        return NULL;
    }
    instance_[name] = t;
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
        cerr << "attempting to delete non-existant instance " << name << endl;
    }
}

template <typename T, typename TRep>
Ptr<T> ManagerImpl::cast_instance(const string& v) {
    Ptr<Instance> ins = instance(v);
    if (ins == NULL)
        cerr << "instance lookup for " << v << " failed" << endl;
    return Ptr<TRep>((TRep*) ins.ptr())->engineObject();
}

string LocationRep::attribute(const string& name) {
    int i = segmentNumber(name);
    if (i >= 0) {
        return engineObject_->segment(i)->name();
    }
    return "";
}

void LocationRep::attributeIs(const string& name, const string& v) {
    //location has no writable attributes
}

string SegmentRep::attribute(const string& name) {
    ostringstream os;
    if (name == "source") {
        os << engineObject_->source()->name();
    } else if (name == "length") {
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
    } else {
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
    } else {
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
        os << manager_->engineManager()->shippingNetwork()->customers();
    } else if (name == "Truck terminal") {
        os << manager_->engineManager()->shippingNetwork()->truckTerminals();
    } else if (name == "Plane terminal") {
        os << manager_->engineManager()->shippingNetwork()->planeTerminals();
    } else if (name == "Boat terminal") {
        os << manager_->engineManager()->shippingNetwork()->boatTerminals();
    } else if (name == "Port") {
        os << manager_->engineManager()->shippingNetwork()->ports();
    } else if (name == "Truck segment") {
        os << manager_->engineManager()->shippingNetwork()->truckSegments();
    } else if (name == "Plane segment") {
        os << manager_->engineManager()->shippingNetwork()->planeSegments();
    } else if (name == "Boat segment") {
        os << manager_->engineManager()->shippingNetwork()->boatSegments();
    } else if (name == "expedite percentage") {
        int expediteAvailable = 0;
        manager_->engineManager()->shippingNetwork()->expediteIs(Segment::available());
        expediteAvailable += manager_->engineManager()->shippingNetwork()->truckSegments();
        expediteAvailable += manager_->engineManager()->shippingNetwork()->planeSegments();
        expediteAvailable += manager_->engineManager()->shippingNetwork()->boatSegments();
        int totalSegments = 0;
        manager_->engineManager()->shippingNetwork()->expediteIs(Segment::allAvailabilities());
        totalSegments += manager_->engineManager()->shippingNetwork()->truckSegments();
        totalSegments += manager_->engineManager()->shippingNetwork()->planeSegments();
        totalSegments += manager_->engineManager()->shippingNetwork()->boatSegments();
        os.setf(ios::fixed,ios::floatfield);
        os.precision(2);
        os << ((float) expediteAvailable / (float) totalSegments) * 100.0;
    } else {
        return "";
    }
    return os.str();
}
string ConnRep::attribute(const string& name) {
    istringstream is(name);
    ostringstream os;
    string cmd, attr, val, token, source, dest, dummy;
    Segment::ExpediteSupport expedite = Segment::unavailable();
    Mile maxDistance;
    USD maxCost;
    int maxTime;

    Ptr<ShippingNetwork> network = manager_->engineManager()->shippingNetwork();

    is >> cmd;
    if (cmd.compare("explore") == 0) {
        is >> source;
        is >> dummy; // colon
        while (!is.eof () ){
            is >> attr;
            if (attr.compare("expedite") == 0)
                expedite = Segment::available();
            else {
                //is >> val;
                if (attr.compare("distance") == 0){
                    int raw;
                    is >> raw;
                    maxDistance = raw;
                }
                else if (attr.compare("cost") == 0) {
                    double raw;
                    is >> raw;
                    maxCost = raw;
                }
                else if (attr.compare("time") == 0){
                    int raw;
                    is >> raw;
                    maxTime = raw;
                }
            }
        }
        network->sourceIs( Ptr<LocationRep>((LocationRep*)manager_->instance(source).ptr() )->engineObject() );
        network->destinationIs(NULL);
        network->maxCostIs(maxCost);
        network->maxDistanceIs(maxDistance);
        network->maxTimeIs(maxTime);
    }
    else if (cmd.compare("connect") == 0) {
        Ptr<ShippingNetwork> network = manager_->engineManager()->shippingNetwork();
        is >> source >> dummy >> dest;
        network->sourceIs( Ptr<LocationRep>((LocationRep*)manager_->instance(source).ptr() )->engineObject() );
        network->destinationIs( Ptr<LocationRep>((LocationRep*)manager_->instance(dest).ptr() )->engineObject() );
    }
    os.setf(ios::fixed,ios::floatfield);
    os.precision(2);
    for (unsigned int i = 0; i < network->paths(); i ++) {
        Ptr<Path> path = network->path(i);
        if (path) {
            if ( dest != "") {
                os << path->cost().value() << " " << path->hour().value() << " " << ((path->expedite()==Segment::available())?"yes":"no") << "; ";
            }
            for (unsigned int j = 0; j < path->locations(); j ++) {
                Ptr<Location> loc = path->location(j);
                os << loc->name();
                if (j != path->locations() - 1) {
                    WeakPtr<Segment> seg = path->segment(j);
                    os << "(" << seg->name() << ":" << seg->length().value() << ":" << seg->returnSegment()->name() << ") ";
                }
            }
            os << endl;
        }
    }

    return os.str();
}

string FleetRep::attribute(const string& name) {
    int commaPos = name.find_first_of (',');
    string mode = name.substr (0, commaPos), property = name.substr (commaPos + 1, name.length());
    //trim the string
    mode = mode.substr (mode.find_first_not_of(' '));
    mode = mode.substr (0, mode.find_last_not_of (' ') + 1);
    property = property.substr (property.find_first_not_of(' ') );
    property = property.substr (0, property.find_last_not_of(' ') + 1);
    Ptr<Fleet> fleet_;
    if (mode == "Truck") fleet_ = truckfleet_;
    if (mode == "Boat") fleet_ = boatfleet_;
    if (mode == "Plane") fleet_ = planefleet_;

    ostringstream os;
    os.setf(ios::fixed,ios::floatfield);
    os.precision(2);
    if (property == "speed") {
        os << fleet_->speed().value();
    } else if (property == "cost"){
        os << fleet_->cost().value();
    } else if (property == "capacity"){
        os << fleet_->capacity().value();
    } else {
        return "";
    }
    return os.str();
}

void FleetRep::attributeIs(const string& name, const string& v) {
    int commaPos = name.find_first_of (',');
    string mode = name.substr (0, commaPos), property = name.substr (commaPos + 1, name.length());
    //trim the string
    mode = mode.substr (mode.find_first_not_of(' '));
    mode = mode.substr (0, mode.find_last_not_of (' ') + 1);
    property = property.substr (property.find_first_not_of(' ') );
    property = property.substr (0, property.find_last_not_of(' ') + 1);
    Ptr<Fleet> fleet_;
    if (mode == "Truck") fleet_ = truckfleet_;
    if (mode == "Boat") fleet_ = boatfleet_;
    if (mode == "Plane") fleet_ = planefleet_;

    istringstream is(v);
    if (property == "speed") {
        double propertyValue;
        is >> propertyValue;
        fleet_->speedIs(Mile(propertyValue));
    }
    if (property == "cost") {
        double propertyValue;
        is >> propertyValue;
        fleet_->costIs(USD(propertyValue));
    }
    if (property == "capacity") {
        int propertyValue;
        is >> propertyValue;
        fleet_->capacityIs(PackageUnit(propertyValue));
    }
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
