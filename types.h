#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include "Engine.h"

namespace Shipping {

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

}

#endif
