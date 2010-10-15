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
class Segment : public Fwk::PtrInterface<Segment> {
/*  VehicleType mode;
  Location source;
  Distance length;
  Segment *returnSegment;
  Difficulty difficulty;
  ExpediteSupport expediteSupport;*/
};

class TruckSegment : public Segment {
};

class Engine {
 public:
  void terminalNew() {
  }
  static Ptr<Segment> truckSegmentNew() {
    Ptr<Segment> m = new Segment();
    return m;
  }
};
} /* end namespace */

#endif
