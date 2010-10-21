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
#include "fwk/BaseNotifiee.h"
#include "fwk/NamedInterface.h"

namespace Shipping {
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

    ExpediteSupport expediteSupport() const{ return expediteSupport_; }
    void expediteSupportIs(const ExpediteSupport expediteSupport){ expediteSupport_ = expediteSupport; }
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
        virtual void notifierIs(Fwk::Ptr<Segment> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Segment::Notifiee> notifieeNew() {
            Fwk::Ptr<Segment::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onSegment() {}
        virtual void onExpediteSupport() {}
        virtual void onSource() {}
        virtual void onReturnSegment() {}
        virtual void onDifficulty() {}
        virtual void onLength() {}
      protected:
        Fwk::Ptr<Segment> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Segment::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Segment(const TransportationMode transportationMode, const string name) : transportationMode_(transportationMode), name_(name) {
        if (notifiee_) notifiee_->onSegment();
    }
    Ptr<Segment::Notifiee> notifiee_;
    void notifieeIs(Segment::Notifiee* n) const {
        Segment* me = const_cast<Segment*>(this);
        me->notifiee_ = n;
    }

  private:
    ExpediteSupport expediteSupport_;
    TransportationMode transportationMode_;
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
        virtual void notifierIs(Fwk::Ptr<Location> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Location::Notifiee> notifieeNew() {
            Fwk::Ptr<Location::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onLocation() {}
        virtual void onSegment() {}
      protected:
        Fwk::Ptr<Location> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Location::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Location(const string name) : name_(name) {
        if (notifiee_) notifiee_->onLocation();
    }
    Ptr<Location::Notifiee> notifiee_;
    void notifieeIs(Location::Notifiee* n) const {
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
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Customer> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Customer::Notifiee> notifieeNew() {
            Fwk::Ptr<Customer::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onCustomer() {}
      protected:
        Fwk::Ptr<Customer> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Customer::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Customer(const string name) : Location(name) {
        if (notifiee_) notifiee_->onCustomer();
    }
    Ptr<Customer::Notifiee> notifiee_;
    void notifieeIs(Customer::Notifiee* n) const {
        Customer* me = const_cast<Customer*>(this);
        me->notifiee_ = n;
    }

  private:
};

class Port : public Location {
  public:
    ~Port(){}
    static Ptr<Port> portNew(const string name) {
        Ptr<Port> m = new Port(name);
        return m;
    }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Port> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Port::Notifiee> notifieeNew() {
            Fwk::Ptr<Port::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onPort() {}
      protected:
        Fwk::Ptr<Port> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Port::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Port(const string name) : Location(name) {
        if (notifiee_) notifiee_->onPort();
    }
    Ptr<Port::Notifiee> notifiee_;
    void notifieeIs(Port::Notifiee* n) const {
        Port* me = const_cast<Port*>(this);
        me->notifiee_ = n;
    }

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
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Terminal> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Terminal::Notifiee> notifieeNew() {
            Fwk::Ptr<Terminal::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onTerminal() {}
      protected:
        Fwk::Ptr<Terminal> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Terminal::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Terminal(const string name, const Segment::TransportationMode transportationMode) : Location(name), transportationMode_(transportationMode) {
        if (notifiee_) notifiee_->onTerminal();
    }
    Ptr<Terminal::Notifiee> notifiee_;
    void notifieeIs(Terminal::Notifiee* n) const {
        Terminal* me = const_cast<Terminal*>(this);
        me->notifiee_ = n;
    }

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
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Fleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Fleet::Notifiee> notifieeNew() {
            Fwk::Ptr<Fleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onFleet() {}
        virtual void onCost() {}
        virtual void onCapacity() {}
        virtual void onTransportationMode() {}
        virtual void onSpeed() {}
      protected:
        Fwk::Ptr<Fleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Fleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Fleet() {
        if (notifiee_) notifiee_->onFleet();
    }
    Ptr<Fleet::Notifiee> notifiee_;
    void notifieeIs(Fleet::Notifiee* n) const {
        Fleet* me = const_cast<Fleet*>(this);
        me->notifiee_ = n;
    }

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
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<TruckFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<TruckFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<TruckFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onTruckFleet() {}
      protected:
        Fwk::Ptr<TruckFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<TruckFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    TruckFleet() : Fleet() {
        if (notifiee_) notifiee_->onTruckFleet();
    }
    Ptr<TruckFleet::Notifiee> notifiee_;
    void notifieeIs(TruckFleet::Notifiee* n) const {
        TruckFleet* me = const_cast<TruckFleet*>(this);
        me->notifiee_ = n;
    }

  private:
};

class BoatFleet : public Fleet {
  public:
    ~BoatFleet(){}
    static Ptr<BoatFleet> boatFleetNew() {
        Ptr<BoatFleet> m = new BoatFleet();
        return m;
    }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<BoatFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<BoatFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<BoatFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onBoatFleet() {}
      protected:
        Fwk::Ptr<BoatFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<BoatFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    BoatFleet() : Fleet() {
        if (notifiee_) notifiee_->onBoatFleet();
    }
    Ptr<BoatFleet::Notifiee> notifiee_;
    void notifieeIs(BoatFleet::Notifiee* n) const {
        BoatFleet* me = const_cast<BoatFleet*>(this);
        me->notifiee_ = n;
    }

  private:
};

class PlaneFleet : public Fleet {
  public:
    ~PlaneFleet(){}
    static Ptr<PlaneFleet> planeFleetNew() {
        Ptr<PlaneFleet> m = new PlaneFleet();
        return m;
    }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<PlaneFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<PlaneFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<PlaneFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onPlaneFleet() {}
      protected:
        Fwk::Ptr<PlaneFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<PlaneFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    PlaneFleet() : Fleet() {
        if (notifiee_) notifiee_->onPlaneFleet();
    }
    Ptr<PlaneFleet::Notifiee> notifiee_;
    void notifieeIs(PlaneFleet::Notifiee* n) const {
        PlaneFleet* me = const_cast<PlaneFleet*>(this);
        me->notifiee_ = n;
    }

  private:
};

} /* end namespace */

#endif
