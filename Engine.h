#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"

namespace Shipping {

// Create your rep/engine interface here.
class Engine {
 public:
  class Location;

  class Segment;

  static Ptr<Terminal> terminalNew();  
  static Ptr<TruckSegment> truckSegmentNew();
};
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

class Engine::Terminal {
public:

};
class Segment : public Fwk::PtrInterface<Segment> {

/*  VehicleType mode;
  Location source;
  Distance length;
  Segment *returnSegment;
  Difficulty difficulty;
  ExpediteSupport expediteSupport;*/
};

class Engine::TruckSegment : public Engine::Segment {
};

} /* end namespace */

#endif
