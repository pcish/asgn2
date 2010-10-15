#ifndef ENGINE_H
#define ENGINE_H

#include <string>

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
enum TransportationMode {
    truck, boat, plane    
};
class Location : public Fwk::PtrInterface<Location>{
};
class Customer : public Location {
};
class Port : public Location {

};
class Terminal : public Location {
};

class Segment : public Fwk::PtrInterface<Segment> {
/*  VehicleType mode;
  Location source;
  Distance length;
  Segment *returnSegment;
  Difficulty difficulty;
  ExpediteSupport expediteSupport;*/
};
class Fleet {

};


class Engine {
public:
  static Ptr<Customer> customerNew();
  static Ptr<Port> portNew ();
  static Ptr<Terminal> terminalNew ();
  static Ptr<Segment> segmentNew();
  static Ptr<Fleet> fleetNew ();

};

} /* end namespace */

#endif
