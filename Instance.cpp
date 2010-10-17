#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include "Instance.h"
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

    // Manager method
    void instanceDel(const string& name);

private:
    map<string,Ptr<Instance> > instance_;
};

class LocationRep : public Instance {
public:

    LocationRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {}

    // Instance method
    string attribute(const string& name);

    // Instance method
    void attributeIs(const string& name, const string& v);

protected:
    Ptr<Location> location_;

private:
    Ptr<ManagerImpl> manager_;
    map<string, Segment> segments; // do we need segment here or in Location?
    int segmentNumber(const string& name);

};
class PortRep : public LocationRep {
public:
    PortRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager)
    {
//        location_ = Engine::truckTerminalNew();
    }

};
class CustomerRep : public LocationRep {
public:
    CustomerRep(const string& name, ManagerImpl *manager) :
        LocationRep(name, manager)
    {
//        location_ = Engine::truckTerminalNew();
    }

};


class TerminalRep : public LocationRep {
public:

    TerminalRep(const string& name, ManagerImpl *manager, Segment::TransportationMode _mode) :
        LocationRep(name, manager), mode_(_mode)
    {
//        location_ = Engine::truckTerminalNew();
    }
protected:
    Segment::TransportationMode mode_;
};

class SegmentRep : public Instance {
public:

    SegmentRep(const string& name, ManagerImpl* manager, Segment::TransportationMode _mode) :
        Instance(name), manager_(manager), mode_ (_mode)
    {
        // Nothing else to do.
    }

    // Instance method
    string attribute(const string& name);

    // Instance method
    void attributeIs(const string& name, const string& v);

protected:
    Ptr<Segment> segment_;    
private:
    Ptr<ManagerImpl> manager_;
    int segmentNumber(const string& name);
    Segment::TransportationMode mode_;

};

class StatsRep : public Instance {
public:
    string attribute (const string &name) {}
    void attributeIs(const string& name, const string& v) {}
    static Ptr<StatsRep> instance (const string& name, ManagerImpl *manager) {
        if (instance_ == NULL)
            instance_ = new StatsRep (name, manager);
        return instance_;
    }
protected:
    StatsRep (const string& name, ManagerImpl *manager) : 
        Instance(name), manager_(manager)
        { instance_ = this; }
private:
    Ptr<ManagerImpl> manager_;
    static Ptr<StatsRep> instance_;
};
Ptr<StatsRep> StatsRep::instance_ = NULL;

class ConnRep : public Instance {
public:
    string attribute(const string& name) {}
    void attributeIs(const string& name, const string& v) {}
    static Ptr<ConnRep> instance (const string &name, ManagerImpl *manager) {
        if (instance_ == NULL) 
            instance_ = new ConnRep (name, manager);
        return instance_;
    }
protected:
    ConnRep (const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager)
    { instance_ = this;   }
private:
    Ptr<ManagerImpl> manager_;
    static Ptr<ConnRep> instance_;
};
Ptr<ConnRep> ConnRep::instance_ = NULL;

ManagerImpl::ManagerImpl() {
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if (instance_.find(name) != instance_.end() ){
        cerr << "Attempt to new instances of the same names!" << endl;
        return NULL;
    }
    if (type == "Truck terminal") {
        Ptr<TerminalRep> t = new TerminalRep(name, this, Segment::truck() );
        instance_[name] = t;
        return t;
    }
    if (type == "Truck segment") {
        Ptr<SegmentRep> t = new SegmentRep(name, this, Segment::truck());
        instance_[name] = t;
        return t;
    }
    if (type == "Boat terminal") {
    }
    if (type == "Boat segment") {
    }
    if (type == "Plane terminal") {
    }
    if (type == "Plane segment") {
    }
    if (type == "Port") {}
    if (type == "Customer") {}
    if (type == "StatsRep") {
        Ptr<StatsRep> t = StatsRep::instance (name, this);
        instance_[name] = t;
        return t;    
    }
    if (type == "Fleet") {}
    if (type == "ConnRep") {
        Ptr<ConnRep> t = ConnRep::instance (name, this);
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


string LocationRep::attribute(const string& name) {
    int i = segmentNumber(name);
    if (i != 0) {
        cout << "Tried to read interface " << i;
    }
    return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

string SegmentRep::attribute(const string& name) {
    int i = 1;//segmentNumber(name);
    if (i != 0) {
        cout << "Tried to read interface " << i;
    }
    return "";
}


void SegmentRep::attributeIs(const string& name, const string& v) {
    //nothing to do
    if (name == "source") {
        //segment_.sourceIs();
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
