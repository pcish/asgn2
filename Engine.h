#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <list>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"


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
typedef unsigned int U32;
enum TransportationMode {
    truck, boat, plane    
};

class Segment;
class Location : public Fwk::PtrInterface<Location>{

protected:
    List<Segment> segments_;
};
class Customer : public Location {
};
class Port : public Location {

};
class Terminal : public Location {
public:
    Terminal (TransportationMode _transMode);
    TransportationMode transportationMode ();
    void transportationModeIs (TransportationMode _transMode);
private:
    TransportationMode transMode_;
};

class Segment : public Fwk::PtrInterface<Segment> {
public:
    Segment (TransportationMode _transMode);
    TransportationMode transportationMode ();
    void transportationModeIs (TransportationMode _transMode);
private:
    TransportationMode transMode_;
    Ptr<Location> source_;
    U32 length_; // need to be modified
    Ptr<Segment> returnSegment_;
    U32 difficulty_;
    bool expediteSupport_;
/*  VehicleType mode;
  Location source;
  Distance length;
  Segment *returnSegment;
  Difficulty difficulty;
  ExpediteSupport expediteSupport;*/
};
class Fleet {
public:
    Fleet (TransportationMode _transMode);
    TransportationMode transportationMode ();
    void transportationModeIs (TransportationMode _transMode);
private:
    TransportationMode transMode_;
    U32 speed, capacity, cost;

};


class Engine {
public:
  static Ptr<Customer> customerNew();
  static Ptr<Port> portNew ();
  static Ptr<Terminal> terminalNew (TransportationMode);
  static Ptr<Segment> segmentNew();
  static Ptr<Fleet> fleetNew ();

};

} /* end namespace */

#endif
