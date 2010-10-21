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
#include "fwk/BaseNotifiee.h"
#include "fwk/NamedInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "types.h"

namespace Shipping {

// Create your rep/engine interface here.
//store name in each entity?
/*
TODO list:
1. Add destructor for every class 
2. Delete segment from Location
*/
/*
template <class T>
class BaseNotifiee {
    public:
        BaseNotifiee(T* _notifier) {  notifier_ = _notifier; }
        void notifierIs (T* _notifier) {
        }
    protected:
        T* notifier_;
};
*/

class Customer;
class PlaneFleet;
class TruckFleet;
class Terminal;
class Location;
class BoatFleet;
class Fleet;
class Segment;
class Port;
class Segment : public Fwk::PtrInterface<Segment> {
  public:
    ~Segment(){}
    enum TransportationMode {
        truck_,
        boat_,
        plane_
    };
    static inline TransportationMode truck() { return truck_; }
    static inline TransportationMode boat() { return boat_; }
    static inline TransportationMode plane() { return plane_; }

    enum ExpediteSupport {
        available_,
        unavailable_
    };
    static inline ExpediteSupport available() { return available_; }
    static inline ExpediteSupport unavailable() { return unavailable_; }
/*
<<<<<<< HEAD
    string name() const { return name_; }
    TransportationMode const transportationMode() { return transMode_; }
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> _source) { source_ = _source; }
    Mile length() const { return length_; }
    void lengthIs(const Mile& _length) { length_ = _length; } 
    Ptr<Segment> returnSegment() const { return returnSegment_; }
    void returnSegmentIs(const Ptr<Segment>& _returnSegment) { returnSegment_ = _returnSegment; }
    SegmentDifficultyUnit difficulty() const { return difficulty_; }
    void difficultyIs (const SegmentDifficultyUnit& _difficulty) { difficulty_ = _difficulty; }
    bool expediteSupport() const { return expediteSupport_; }
    void expediteSupportIs (const bool _expediteSupport) { expediteSupport_ = _expediteSupport; }
    

protected:
    string name_;
private:
    Segment (const string& _name, const TransportationMode& _transMode) : name_(_name), transMode_(_transMode) {}
    TransportationMode transMode_;
=======
*/
    ExpediteSupport expediteSupport() const{ return expediteSupport_; }
    void expediteSupportIs(const ExpediteSupport expediteSupport){ expediteSupport_ = expediteSupport; 
        if (notifiee_)
            notifiee_->onExpediteSupport();
    }
    TransportationMode transportationMode() const{ return transportationMode_; }
    Ptr<Location> source() const{ return source_; }
    void sourceIs(const Ptr<Location> source){ source_ = source; }
    Ptr<Segment> returnSegment() const{ return returnSegment_; }
    void returnSegmentIs(const Ptr<Segment> returnSegment){ returnSegment_ = returnSegment; }
    SegmentDifficultyUnit difficulty() const{ return difficulty_; }
    void difficultyIs(const SegmentDifficultyUnit difficulty){ difficulty_ = difficulty; }
    Mile length() const{ return length_; }
    void lengthIs(const Mile length){ length_ = length; }
    string name() const{ return name_; }
    static Ptr<Segment> segmentNew(const TransportationMode transportationMode, const string name) {
        Ptr<Segment> m = new Segment(transportationMode, name);
        return m;
    }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
        public:
            virtual void onExpediteSupport() {}
            virtual void onSegment() {}
            virtual void notifierIs(Fwk::Ptr<Segment> _notifier) {
                if (notifier_ == _notifier)
                    return;
                if (notifier_) 
                    notifier_->notifieeIs(0);
                notifier_ = _notifier;
                notifier_->notifieeIs(this);
            }
            static Fwk::Ptr<Segment::Notifiee> notifieeNew () {
                Fwk::Ptr<Segment::Notifiee> n = new Notifiee ();
                return n;
            }
        protected:
            Fwk::Ptr<Segment> notifier_;
            Notifiee () : notifier_(0) {}
    };
    Ptr<Segment::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Segment(const TransportationMode transportationMode, const string name) : transportationMode_(transportationMode), name_(name) {
        if (notifiee_)
            notifiee_->onSegment();
    }
    Ptr<Segment::Notifiee> notifiee_;
    void notifieeIs( Segment::Notifiee* n) const {
        Segment* me = const_cast<Segment*>(this);
        me->notifiee_ = n;
    }
  private:
    ExpediteSupport expediteSupport_;
    TransportationMode transportationMode_;
//>>>>>>> 2387774f5e204824112b78c020628b670849d652
    Ptr<Location> source_;
    Ptr<Segment> returnSegment_;
    SegmentDifficultyUnit difficulty_;
    Mile length_;
    string name_;
};

class Location : public Fwk::PtrInterface<Location> {
  public:
    ~Location(){}
    virtual Ptr<Segment> segment(const unsigned int index) const;
    virtual void segmentIs(const Ptr<Segment> segment);
    string name() const{ return name_; }
    static Ptr<Location> locationNew(const string name) {
        Ptr<Location> m = new Location(name);
        return m;
    }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
        public:
            virtual void onSegment(const Fwk::Ptr<Segment> segment){}
            virtual void onLocation() {}
            virtual void notifierIs(Fwk::Ptr<Location> _notifier) {
                if (notifier_ == _notifier)
                    return;
                if (notifier_) 
                    notifier_->notifieeIs(0);
                notifier_ = _notifier;
                notifier_->notifieeIs(this);
            }
            static Fwk::Ptr<Location::Notifiee> notifieeNew () {
                Fwk::Ptr<Location::Notifiee> n = new Notifiee ();
                return n;
            }
        protected:
            Fwk::Ptr<Location> notifier_;
            Notifiee () : notifier_(0) {}
    };
    Ptr<Location::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Location(const string name) : name_(name) {
        if (notifiee_)
            notifiee_->onLocation();
    }
    Ptr<Location::Notifiee> notifiee_;
    void notifieeIs( Location::Notifiee* n) const {
        Location* me = const_cast<Location*>(this);
        me->notifiee_ = n;
    }
  private:
    std::vector<Ptr<Segment> > segments_;
    string name_;
};

class Customer : public Location {
  public:
    ~Customer(){}
    static Ptr<Customer> customerNew(const string name) {
        Ptr<Customer> m = new Customer(name);
        return m;
    }
  protected:
    Customer(const string name) : Location(name) {}
  private:
};

class Port : public Location {
  public:
    ~Port(){}
    static Ptr<Port> portNew(const string name) {
        Ptr<Port> m = new Port(name);
        return m;
    }
  protected:
    Port(const string name) : Location(name) {}
  private:
};

class Terminal : public Location {
  public:
    ~Terminal(){}
    Segment::TransportationMode transportationMode() const{ return transportationMode_; }
    static Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode) {
        Ptr<Terminal> m = new Terminal(name, transportationMode);
        return m;
    }
  protected:
    Terminal(const string name, const Segment::TransportationMode transportationMode) : Location(name), transportationMode_(transportationMode) {}
  private:
    Segment::TransportationMode transportationMode_;
};

class Fleet : public Fwk::PtrInterface<Fleet> {
  public:
    ~Fleet(){}
    USD cost() const{ return cost_; }
    void costIs(const USD cost){ cost_ = cost; }
    PackageUnit capacity() const{ return capacity_; }
    void capacityIs(const PackageUnit capacity){ capacity_ = capacity; }
    Segment::TransportationMode transportationMode() const{ return transportationMode_; }
    void transportationModeIs(const Segment::TransportationMode transportationMode){ transportationMode_ = transportationMode; }
    Mile speed() const{ return speed_; }
    void speedIs(const Mile speed){ speed_ = speed; }
    static Ptr<Fleet> fleetNew() {
        Ptr<Fleet> m = new Fleet();
        return m;
    }
  protected:
    Fleet() {}
  private:
    USD cost_;
    PackageUnit capacity_;
    Segment::TransportationMode transportationMode_;
    Mile speed_;
};

class TruckFleet : public Fleet {
  public:
    ~TruckFleet(){}
    static Ptr<TruckFleet> truckFleetNew() {
        Ptr<TruckFleet> m = new TruckFleet();
        return m;
    }
  protected:
    TruckFleet() : Fleet() {}
  private:
};

class BoatFleet : public Fleet {
  public:
    ~BoatFleet(){}
    static Ptr<BoatFleet> boatFleetNew() {
        Ptr<BoatFleet> m = new BoatFleet();
        return m;
    }
  protected:
    BoatFleet() : Fleet() {}
  private:
};

class PlaneFleet : public Fleet {
  public:
    ~PlaneFleet(){}
    static Ptr<PlaneFleet> planeFleetNew() {
        Ptr<PlaneFleet> m = new PlaneFleet();
        return m;
    }
  protected:
    PlaneFleet() : Fleet() {}
  private:
};

} /* end namespace */

#endif
