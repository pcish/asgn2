#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <list>

using namespace std;
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

//I'm not sure if this is needed to detect invalid attribute values (negative values), or maybe it's ok to put the detection work to rep layer?
class UF32 {
private:
    float value_;
    bool validValue (float _value) {
        if (_value < 0){
            cerr << "Invalid Value for UF32: " << _value << " < 0" << endl;
            return false;
        }
        return true;
    }
public:
    UF32 (float _value = 0) {
        if (validValue (_value))
           value_ = _value;
        else
           value_ = 0;
    }
    float value() {return value_;}
    UF32 operator=(float _value) {
        if (validValue(_value) ) 
            value_ = _value;
        return *this;
    }
    bool operator<(UF32 target){ return value_ < target.value_; } 
    bool operator==(UF32 target){ return value_ == target.value_; } 
    bool operator!=(UF32 target){ return value_ != target.value_; } 
    bool operator>(UF32 target){ return value_ > target.value_; } 
    UF32 operator+(UF32 target){ 
        float result = value_ + target.value_; 
        if (validValue(result) ) {
            return result;
        }
        return 0;
    }
    UF32 operator-(UF32 target){ 
        float result = value_ + target.value_; 
        if (validValue(result) ) {
            return result;
        }
        return 0;
    }
    UF32 operator*(UF32 target){ 
        float result = value_ * target.value_; 
        if (validValue(result) ) {
            return result;
        }
        return 0;
    }
    UF32 operator/(UF32 target){   // / won't over/under flow
        float result = value_ / target.value_; 
        return result;
    }
    UF32 operator++() {
        if (validValue (value_ + 1) ){
            value_++;            
        }
        return *this;
    }
    UF32 operator--() {
        if (validValue (value_ - 1) ){
            value_--;            
        }
        return *this;
    }
};
//typedef unsigned float UF32;
enum TransportationMode {
    truck, boat, plane    
};
//store name in each entity?

class Segment;

class Location : public Fwk::PtrInterface<Location>{
public:
    //declared virtual because subclasses may have additional contraints on the type of segment that connects
    virtual void segmentIs (Segment *seg); 
    virtual void segmentIs (unsigned int index, Segment *seg); // assign segment for nth segment
    unsigned int segments ();
    Segment segment (unsigned int index);
    
protected:
    vector <Segment*> segments_;    
};
class Customer : public Location {
};
class Port : public Location {

};
class Terminal : public Location {
public:
    Terminal (TransportationMode _transMode) : transMode_ (_transMode) {}
    TransportationMode transportationMode () { return transMode_; }
    //a terminal's type should be assigned at the beginning and cannot withstand changing after instantiated
    //void transportationModeIs (TransportationMode _transMode);
    virtual void segmentIs (Segment *seg);
    virtual void segmentIs (unsigned int index, Segment *seg);
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
    UF32 length_; // need to be modified
    Ptr<Segment> returnSegment_;
    UF32 difficulty_;
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
    UF32 speed, capacity, cost;

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
