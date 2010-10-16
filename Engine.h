#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <list>
#include <exception>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"

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

class Error : public std::exception {
  public:
    Error(string errorMessage) : std::exception() {
        errorMessage_ = errorMessage;
    }
    ~Error() throw();
    virtual const char* what() const throw() {
        return errorMessage_.c_str();
    }
  private:
    string errorMessage_;
};

class ValueError : public Error {
  public:
    ValueError(string errorMessage) : Error(errorMessage) {
    }
};

class Mile : public Ordinal<Mile, unsigned int> {
  public:
    Mile(unsigned int num) : Ordinal<Mile, unsigned int>(num) {
    }
};

class USD : public Ordinal<USD, double> {
  public:
    USD(double num) : Ordinal<USD, double>(num) {
    }
};

class PackageUnit : public Ordinal<PackageUnit, unsigned int> {
  public:
    PackageUnit(unsigned int num) : Ordinal<PackageUnit, unsigned int>(num) {
    }
};

class SegmentDifficultyUnit : public Ordinal<SegmentDifficultyUnit, float> {
  public:
    SegmentDifficultyUnit(float num) : Ordinal<SegmentDifficultyUnit, float>(num) {
        if (num < 1.0 || num > 5.0) throw ValueError("Segment Difficulty must lie between 1.0 and 5.0");
    }
};

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
    Mile length_; // need to be modified
    Ptr<Segment> returnSegment_;
    SegmentDifficultyUnit difficulty_;
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
    static Fleet* instance();

    TransportationMode transportationMode();
    void transportationModeIs (TransportationMode _transMode);
private:
    Fleet(TransportationMode _transMode);
    TransportationMode transMode_;
    Mile speed;
    PackageUnit capacity;
    USD cost;

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
