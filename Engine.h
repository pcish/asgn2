#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>

using namespace std;
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "types.h"

namespace Shipping {

// Create your rep/engine interface here.
/*
class Package;
class Shipment {
  Location source, destination;
  Package *packages;
};
class Location;
class CutsomterLocation : public Location;
class Port : public Location;
class Terminal : public Location;
class Fleet;
*/



//store name in each entity?

class Location;

class Segment : public Fwk::PtrInterface<Segment> {
public:
    enum TransportationMode {
        truck_, boat_, plane_
    };
    static inline TransportationMode truck () {return truck_; }
    static inline TransportationMode boat () {return boat_; }
    static inline TransportationMode plane () {return plane_; }
    Segment (TransportationMode _transMode);
    TransportationMode transportationMode () {return transMode_;}

private:
    Segment::TransportationMode transMode_;
    Ptr<Location> source_;
    Mile length_; // need to be modified
    Ptr<Segment> returnSegment_;
    SegmentDifficultyUnit difficulty_;
    bool expediteSupport_;
};

class Location : public Fwk::PtrInterface<Location>{
public:
    
    //declared virtual because subclasses may have additional contraints on the type of segment that connects
    virtual void segmentIs (Ptr<Segment> seg);
    virtual void segmentIs (unsigned int index, Ptr<Segment> seg); // assign segment for nth segment
    unsigned int segments ();
    Segment segment (unsigned int index);

protected:
    vector<Ptr<Segment> > segments_;
};

class Customer : public Location {
};
class Port : public Location {

};
class Terminal : public Location {
public:
    Terminal (Segment::TransportationMode _transMode) : transMode_ (_transMode) {}
    Segment::TransportationMode transportationMode () { return transMode_; }
    //a terminal's type should be assigned at the beginning and cannot withstand changing after instantiated
    //void transportationModeIs (Segment::TransportationMode _transMode);
    virtual void segmentIs (Ptr<Segment> seg);
    virtual void segmentIs (unsigned int index, Ptr<Segment> seg);
private:
    Segment::TransportationMode transMode_;
};

class Fleet {
public:
    static Ptr<Fleet> instance();

    Segment::TransportationMode transportationMode();
    void transportationModeIs (Segment::TransportationMode _transMode);
private:
    Fleet(Segment::TransportationMode _transMode);
    Segment::TransportationMode transMode_;
    Mile speed;
    PackageUnit capacity;
    USD cost;

};


class Engine {
public:
  static Ptr<Customer> customerNew();
  static Ptr<Port> portNew ();
  static Ptr<Terminal> terminalNew (Segment::TransportationMode);
  static Ptr<Segment> segmentNew();
  static Ptr<Fleet> fleetNew ();

};

} /* end namespace */

#endif
