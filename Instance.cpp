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

  private:
    Ptr<Location> engineObject_;
    int segmentNumber(const string& name);
};

class PortRep : public LocationRep {
  public:
    PortRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->engineManager()->portNew(name);
    }
    Ptr<Port> engineObject() const { return engineObject_; }

  private:
    Ptr<Port> engineObject_;
};

class CustomerRep : public LocationRep {
  public:
    CustomerRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager) {
        engineObject_ = manager->engineManager()->customerNew(name);
    }
    Ptr<Customer> engineObject() const { return engineObject_; }

  private:
    Ptr<Customer> engineObject_;
};

class TerminalRep : public LocationRep {
  public:
    TerminalRep(const string& name, ManagerImpl *manager, Segment::TransportationMode _mode) :
        LocationRep(name, manager), mode_(_mode) {
          engineObject_ = manager_->engineManager()->terminalNew(name, _mode);
    }

  protected:
    Segment::TransportationMode mode_;

  private:
    Ptr<Terminal> engineObject_;
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
        Instance(name), manager_(manager) {}
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
        Instance(name), manager_(manager) {}
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
    if (type == "Truck terminal") {
        Ptr<TerminalRep> t = new TerminalRep(name, this, Segment::truck());
        instance_[name] = t;
        return t;
    }
    if (type == "Truck segment") {
        Ptr<SegmentRep> t = new SegmentRep(name, this, Segment::truck());
        instance_[name] = t;
        return t;
    }
    if (type == "Boat terminal") {
        Ptr<TerminalRep> t = new TerminalRep(name, this, Segment::boat());
        instance_[name] = t;
        return t;
    }
    if (type == "Boat segment") {
        Ptr<SegmentRep> t = new SegmentRep(name, this, Segment::boat());
        instance_[name] = t;
        return t;
    }
    if (type == "Plane terminal") {
        Ptr<TerminalRep> t = new TerminalRep(name, this, Segment::plane());
        instance_[name] = t;
        return t;
    }
    if (type == "Plane segment") {
        Ptr<SegmentRep> t = new SegmentRep(name, this, Segment::plane());
        instance_[name] = t;
        return t;
    }
    if (type == "Port") {
        Ptr<PortRep> t = new PortRep(name, this);
        instance_[name] = t;
        return t;
    }
    if (type == "Customer") {
        Ptr<CustomerRep> t = new CustomerRep(name, this);
        instance_[name] = t;
        return t;
    }
    if (type == "Stats") {
        Ptr<StatsRep> t = StatsRep::instance(name, this);
        instance_[name] = t;
        return t;
    }
    if (type == "Fleet") {
        Ptr<FleetRep> t = FleetRep::instance(name, this);
        instance_[name] = t;
        return t;
    }
    if (type == "ConnRep") {
        Ptr<ConnRep> t = ConnRep::instance(name, this);
        instance_[name] = t;
        return t;
    }
    return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
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
    if (i != 0) {
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
        os << engineObject_->returnSegment()->name();
    } else if (name == "difficulty") {
        os << engineObject_->difficulty().value();
    } else if (name == "expedite support") {
        int support = engineObject_->expediteSupport();
        if (support == Segment::available()) os << "yes";
        else if (support == Segment::unavailable()) os << "no";
    } else {
    }
    return os.str();
}

void SegmentRep::attributeIs(const string& name, const string& v) {
    if (name == "source") {
        Ptr<Location> location = manager_->cast_instance<Location, LocationRep>(v);
        engineObject_->sourceIs(location);
        location->segmentIs(engineObject_);
    } else if (name == "length") {
        engineObject_->lengthIs(atoi(v.c_str()));
    } else if (name == "return segment") {
        Ptr<Segment> segment = manager_->cast_instance<Segment, SegmentRep>(v);
        engineObject_->returnSegmentIs(segment);
    } else if (name == "difficulty") {
        engineObject_->difficultyIs(atof(v.c_str()));
    } else if (name == "expedite support") {
        int support = engineObject_->expediteSupport();
        if (support == Segment::available()) ;
        else if (support == Segment::unavailable()) ;
    } else {
    }
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t);
    }
    return 0;
}

string StatsRep::attribute(const string& name) {
    ostringstream os;
    if (name == "Customer") {
        os << manager_->engineManager()->shippingNetwork()->customers();
    } else if (name == "Truck terminal") {
        os << manager_->engineManager()->shippingNetwork()->terminals();
    } else if (name == "Plane terminal") {
        os << manager_->engineManager()->shippingNetwork()->terminals();
    } else if (name == "Boat terminal") {
        os << manager_->engineManager()->shippingNetwork()->terminals();
    } else if (name == "Port") {
        os << manager_->engineManager()->shippingNetwork()->ports();
    } else if (name == "Truck segment") {
        os << manager_->engineManager()->shippingNetwork()->segments();
    } else if (name == "Plane segment") {
        os << manager_->engineManager()->shippingNetwork()->segments();
    } else if (name == "Boat segment") {
        os << manager_->engineManager()->shippingNetwork()->segments();
    } else {
        return "";
    }
    return os.str();
}
string ConnRep::attribute(const string& name) {
    istringstream os(name);
  //  string temp
//    while
    return "";
}

string FleetRep::attribute(const string& name) {
    int commaPos = name.find_first_of (',');
    string mode = name.substr (0, commaPos), property = name.substr (commaPos + 1);
    //trim the string
    mode = mode.substr (mode.find_first_not_of(' '));
    mode = mode.substr (0, mode.find_last_not_of (' ') + 1);
    property = property.substr (property.find_first_not_of(' ') );
    property = property.substr (0, property.find_first_not_of(' ') + 1);
    Ptr<Fleet> fleet_;
    if (mode == "Truck") fleet_ = truckfleet_;
    if (mode == "Boat") fleet_ = boatfleet_;
    if (mode == "Plane") fleet_ = planefleet_;

    ostringstream os;
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
    string mode = name.substr (0, commaPos), property = name.substr (commaPos + 1);
    //trim the string
    mode = mode.substr (mode.find_first_not_of(' '));
    mode = mode.substr (0, mode.find_last_not_of (' ') + 1);
    property = property.substr (property.find_first_not_of(' ') );
    property = property.substr (0, property.find_first_not_of(' ') + 1);
    Ptr<Fleet> fleet_;
    if (mode == "Truck") fleet_ = truckfleet_;
    if (mode == "Boat") fleet_ = boatfleet_;
    if (mode == "Plane") fleet_ = planefleet_;

    istringstream is(v);
    int propertyValue;
    is >> propertyValue;
    if (property == "speed") {
        fleet_->speedIs(Mile(propertyValue));
    }
    if (property == "cost") {
        fleet_->costIs(USD(propertyValue));
    }
    if (property == "capacity") {
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
