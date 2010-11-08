#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include "error.h"

namespace Shipping {

class Mile : public Ordinal<Mile, double> {
  public:
    Mile(double num = 0) : Ordinal<Mile, double>(num) {
    }
};

class USD : public Ordinal<USD, double> {
  public:
    USD(double num = 0.0) : Ordinal<USD, double>(num) {
    }
};

class PackageUnit : public Ordinal<PackageUnit, unsigned int> {
  public:
    PackageUnit(unsigned int num = 0) : Ordinal<PackageUnit, unsigned int>(num) {
    }
};

class SegmentDifficultyUnit : public Ordinal<SegmentDifficultyUnit, float> {
  public:
    static const float minimum = 1.0;
    static const float maximum = 5.0;
    SegmentDifficultyUnit(float num = 1.0) : Ordinal<SegmentDifficultyUnit, float>(num) {
        if (num < minimum || num > maximum) throw ValueError("Segment Difficulty must lie between 1.0 and 5.0");
    }
};

class SegmentCount : public Ordinal<SegmentCount, unsigned int> {
  public:
    SegmentCount(unsigned int num = 0) : Ordinal<SegmentCount, unsigned int>(num) {
    }
};

class TruckSegmentCount : public Ordinal<TruckSegmentCount, unsigned int> {
  public:
    TruckSegmentCount(unsigned int num = 0) : Ordinal<TruckSegmentCount, unsigned int>(num) {
    }
};

class BoatSegmentCount : public Ordinal<BoatSegmentCount, unsigned int> {
  public:
    BoatSegmentCount(unsigned int num = 0) : Ordinal<BoatSegmentCount, unsigned int>(num) {
    }
};

class PlaneSegmentCount : public Ordinal<PlaneSegmentCount, unsigned int> {
  public:
    PlaneSegmentCount(unsigned int num = 0) : Ordinal<PlaneSegmentCount, unsigned int>(num) {
    }
};

class LocationCount : public Ordinal<LocationCount, unsigned int> {
  public:
    LocationCount(unsigned int num = 0) : Ordinal<LocationCount, unsigned int>(num) {
    }
};

class PortCount : public Ordinal<PortCount, unsigned int> {
  public:
    PortCount(unsigned int num = 0) : Ordinal<PortCount, unsigned int>(num) {
    }
};

class TerminalCount : public Ordinal<TerminalCount, unsigned int> {
  public:
    TerminalCount(unsigned int num = 0) : Ordinal<TerminalCount, unsigned int>(num) {
    }
};

class TruckTerminalCount : public Ordinal<TruckTerminalCount, unsigned int> {
  public:
    TruckTerminalCount(unsigned int num = 0) : Ordinal<TruckTerminalCount, unsigned int>(num) {
    }
};

class BoatTerminalCount : public Ordinal<BoatTerminalCount, unsigned int> {
  public:
    BoatTerminalCount(unsigned int num = 0) : Ordinal<BoatTerminalCount, unsigned int>(num) {
    }
};

class PlaneTerminalCount : public Ordinal<PlaneTerminalCount, unsigned int> {
  public:
    PlaneTerminalCount(unsigned int num = 0) : Ordinal<PlaneTerminalCount, unsigned int>(num) {
    }
};
class Hour : public Ordinal<Hour, double> {
  public:
    Hour (double num = 0.0) : Ordinal<Hour, double>(num) {
    }
};

}

#endif
