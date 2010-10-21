#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <sstream>


#include "Instance.h"
#include "EngineManager.h"
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

class SegmentRep;

class LocationRep : public Instance {
public:

    LocationRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) { }

    // Instance method
    string attribute(const string& name);

    // Instance method
    void attributeIs(const string& name, const string& v);


    Ptr<Location> location_;

private:
    Ptr<ManagerImpl> manager_;
    //map<string, Segment> segments; // do we need segment here or in Location?
    //vector<string> segmentNames_;
    int segmentNumber(const string& name);

//    void segmentRepIs (Ptr<SegmentRep> segRep);
//    unsigned int segments () { return segmentName.size(); }


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
        location_ = Terminal::terminalNew(name, _mode);
    }
protected:
    Segment::TransportationMode mode_;
};

class SegmentRep : public Instance {
public:
    class SegmentReactor : public Segment::Notifiee {
        public:
            void onExpediteSupport () {
                cout << "Expedite Support changed" << endl;
            }
    };

    SegmentRep(const string& name, ManagerImpl* manager, Segment::TransportationMode _mode) :
        Instance(name), manager_(manager), mode_ (_mode)
    {
        // Nothing else to do.
        segment_ = Segment::segmentNew(_mode, name);
        /* test code */
        SegmentReactor *t = new SegmentReactor ();
        t->notifierIs(segment_);
        segment_->expediteSupportIs(Segment::available());
        /**/
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
    string attribute (const string &name);
    void attributeIs(const string& name, const string& v) {} // do nothing: quietly ignore the write function

    static Ptr<StatsRep> instance (const string& name, ManagerImpl *_manager) {
        if (instance_ == NULL)
            instance_ = new StatsRep (name, _manager);
        return instance_;
    }
protected:
    StatsRep (const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager) { }
private:
    Ptr<ManagerImpl> manager_;
    static Ptr<StatsRep> instance_;
};
Ptr<StatsRep> StatsRep::instance_ = NULL;

class ConnRep : public Instance {
public:
    string attribute(const string& name);
    void attributeIs(const string& name, const string& v) {} //do nothing: quitely ignore the write function
    static Ptr<ConnRep> instance (const string &name, ManagerImpl *manager) {
        if (instance_ == NULL)
            instance_ = new ConnRep (name, manager);
        return instance_;
    }

protected:
    ConnRep (const string& name, ManagerImpl *manager) :
        Instance(name), manager_(manager)
    {}

private:
    Ptr<ManagerImpl> manager_;
    static Ptr<ConnRep> instance_;
};
Ptr<ConnRep> ConnRep::instance_ = NULL;

class FleetRep : public Instance {
public:
    FleetRep(const string& name, ManagerImpl* manager) :
        Instance(name), manager_(manager) {}

    // Instance method
    string attribute(const string& name);

    // Instance method
    void attributeIs(const string& name, const string& v);

protected:
    Ptr<Fleet> truckfleet_, boatfleet_, planefleet_;
private:
    Ptr<ManagerImpl> manager_;
};

ManagerImpl::ManagerImpl() {}

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
        //cout << "Tried to read interface " << i;
        //if (i < 0 || i >= segmentNames_.size() ){
        //    cerr << "Segment # out of bound" << endl;
        //}
        //return segmentNames_[i];
        return location_->segment(i)->name();
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
        Ptr<Instance> instance = manager_->instance(v);
        if (instance == NULL)
            cerr << "instance lookup for " << v << " failed" << endl;
        Ptr<Location> location = Ptr<LocationRep>((LocationRep*) instance.ptr())->location_;
        segment_->sourceIs(location);
        location->segmentIs(segment_);
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
    return "";
}
string ConnRep::attribute(const string& name) {
    return "";
}

string FleetRep::attribute (const string& name) {
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

    if (property == "speed") {
        ostringstream os;
        os << fleet_->speed ().value();
        return os.str();
    }
    if (property == "cost"){
        ostringstream os;
        os << fleet_->cost ().value();
        return os.str();
    }
    if (property == "capacity"){
        ostringstream os;
        os << fleet_->capacity ().value();
        return os.str();
    }
    return "";
}
void FleetRep::attributeIs (const string& name, const string& v) {
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
        fleet_->speedIs (Mile(propertyValue) );
    }
    if (property == "cost") {
        fleet_->costIs (USD(propertyValue) );
    }
    if (property == "capacity") {
        fleet_->capacityIs (PackageUnit(propertyValue) );
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
