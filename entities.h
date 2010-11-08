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

class EngineManager;
class ShippingNetwork;
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
    friend class EngineManager;
  public:
    ~Segment() { if (notifiee_) notifiee_->onDel(this); }
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
        unavailable_,
        allAvailabilities_
    };
    static inline ExpediteSupport available() { return available_; }
    static inline ExpediteSupport unavailable() { return unavailable_; }
    static inline ExpediteSupport allAvailabilities() { return allAvailabilities_; }

    ExpediteSupport expediteSupport() const { return expediteSupport_; }
    void expediteSupportIs(const ExpediteSupport expediteSupport) { if (expediteSupport_ == expediteSupport) return; expediteSupport_ = expediteSupport; if (notifiee_) notifiee_->onExpediteSupport(); }
    string name() const { return name_; }
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> source) { if (source_ == source) return; source_ = source; if (notifiee_) notifiee_->onSource(); }
    Ptr<Segment> returnSegment() const { return returnSegment_; }
    void returnSegmentIs(const Ptr<Segment> returnSegment) { if (returnSegment_ == returnSegment) return; returnSegment_ = returnSegment; if (notifiee_) notifiee_->onReturnSegment(); }
    TransportationMode transportationMode() const { return transportationMode_; }
    SegmentDifficultyUnit difficulty() const { return difficulty_; }
    void difficultyIs(const SegmentDifficultyUnit difficulty) { if (difficulty_ == difficulty) return; difficulty_ = difficulty; if (notifiee_) notifiee_->onDifficulty(); }
    Mile length() const { return length_; }
    void lengthIs(const Mile length) { if (length_ == length) return; length_ = length; if (notifiee_) notifiee_->onLength(); }
    ShippingNetwork* shippingNetwork() const { return shippingNetwork_; }
    void shippingNetworkIs(ShippingNetwork* shippingNetwork) { if (shippingNetwork_ == shippingNetwork) return; shippingNetwork_ = shippingNetwork; if (notifiee_) notifiee_->onShippingNetwork(); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Segment> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Segment::Notifiee> notifieeNew() {
            Fwk::Ptr<Segment::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onExpediteSupport() {}
        virtual void onSource() {}
        virtual void onReturnSegment() {}
        virtual void onDifficulty() {}
        virtual void onLength() {}
        virtual void onShippingNetwork() {}
        virtual void onDel(Segment *p) {}
      protected:
        Fwk::Ptr<Segment> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Segment::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Segment(const string name, const TransportationMode transportationMode) : name_(name), transportationMode_(transportationMode) {
        expediteSupport_ = unavailable_;
    }
    Ptr<Segment::Notifiee> notifiee_;
    void notifieeIs(Segment::Notifiee* n) const {
        Segment* me = const_cast<Segment*>(this);
        me->notifiee_ = n;
    }

  private:
    ExpediteSupport expediteSupport_;
    string name_;
    Ptr<Location> source_;
    Ptr<Segment> returnSegment_;
    TransportationMode transportationMode_;
    SegmentDifficultyUnit difficulty_;
    Mile length_;
    ShippingNetwork* shippingNetwork_;
    Segment(const Segment& o);
};

class Location : public Fwk::PtrInterface<Location> {
    friend class EngineManager;
  public:
    ~Location() { if (notifiee_) notifiee_->onDel(this); }
    virtual Ptr<Segment> segment(const unsigned int index) const;
    virtual void segmentIs(const Ptr<Segment> segment);
    virtual void segmentIs(const unsigned int index, Ptr<Segment> segment);
    virtual unsigned int segments() const { return segments_.size(); }
    string name() const { return name_; }
    ShippingNetwork* shippingNetwork() const { return shippingNetwork_; }
    void shippingNetworkIs(ShippingNetwork* shippingNetwork) { if (shippingNetwork_ == shippingNetwork) return; shippingNetwork_ = shippingNetwork; if (notifiee_) notifiee_->onShippingNetwork(); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Location> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Location::Notifiee> notifieeNew() {
            Fwk::Ptr<Location::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onSegment() {}
        virtual void onShippingNetwork() {}
        virtual void onDel(Location *p) {}
      protected:
        Fwk::Ptr<Location> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Location::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Location(const string name) : name_(name) {}
    Ptr<Location::Notifiee> notifiee_;
    void notifieeIs(Location::Notifiee* n) const {
        Location* me = const_cast<Location*>(this);
        me->notifiee_ = n;
    }

  private:
    std::vector<Ptr<Segment> > segments_;
    string name_;
    ShippingNetwork* shippingNetwork_;
    Location(const Location& o);
};

class Customer : public Location {
    friend class EngineManager;
  public:
    ~Customer() { if (notifiee_) notifiee_->onDel(this); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Customer> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Customer::Notifiee> notifieeNew() {
            Fwk::Ptr<Customer::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(Customer *p) {}
      protected:
        Fwk::Ptr<Customer> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Customer::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Customer(const string name) : Location(name) {}
    Ptr<Customer::Notifiee> notifiee_;
    void notifieeIs(Customer::Notifiee* n) const {
        Customer* me = const_cast<Customer*>(this);
        me->notifiee_ = n;
    }

  private:
    Customer(const Customer& o);
};

class Port : public Location {
    friend class EngineManager;
  public:
    ~Port() { if (notifiee_) notifiee_->onDel(this); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Port> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Port::Notifiee> notifieeNew() {
            Fwk::Ptr<Port::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(Port *p) {}
      protected:
        Fwk::Ptr<Port> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Port::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Port(const string name) : Location(name) {}
    Ptr<Port::Notifiee> notifiee_;
    void notifieeIs(Port::Notifiee* n) const {
        Port* me = const_cast<Port*>(this);
        me->notifiee_ = n;
    }

  private:
    Port(const Port& o);
};

class Terminal : public Location {
    friend class EngineManager;
  public:
    ~Terminal() { if (notifiee_) notifiee_->onDel(this); }
    Segment::TransportationMode transportationMode() const { return transportationMode_; }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Terminal> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Terminal::Notifiee> notifieeNew() {
            Fwk::Ptr<Terminal::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(Terminal *p) {}
      protected:
        Fwk::Ptr<Terminal> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Terminal::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Terminal(const string name, const Segment::TransportationMode transportationMode) : Location(name), transportationMode_(transportationMode) {}
    Ptr<Terminal::Notifiee> notifiee_;
    void notifieeIs(Terminal::Notifiee* n) const {
        Terminal* me = const_cast<Terminal*>(this);
        me->notifiee_ = n;
    }

  private:
    Segment::TransportationMode transportationMode_;
    Terminal(const Terminal& o);
};

class Fleet : public Fwk::PtrInterface<Fleet> {
    friend class EngineManager;
  public:
    ~Fleet() { if (notifiee_) notifiee_->onDel(this); }
    USD cost() const { return cost_; }
    void costIs(const USD cost) { if (cost_ == cost) return; cost_ = cost; if (notifiee_) notifiee_->onCost(); }
    PackageUnit capacity() const { return capacity_; }
    void capacityIs(const PackageUnit capacity) { if (capacity_ == capacity) return; capacity_ = capacity; if (notifiee_) notifiee_->onCapacity(); }
    Segment::TransportationMode transportationMode() const { return transportationMode_; }
    void transportationModeIs(const Segment::TransportationMode transportationMode) { if (transportationMode_ == transportationMode) return; transportationMode_ = transportationMode; if (notifiee_) notifiee_->onTransportationMode(); }
    Mile speed() const { return speed_; }
    void speedIs(const Mile speed) { if (speed_ == speed) return; speed_ = speed; if (notifiee_) notifiee_->onSpeed(); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Fleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Fleet::Notifiee> notifieeNew() {
            Fwk::Ptr<Fleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onCost() {}
        virtual void onCapacity() {}
        virtual void onTransportationMode() {}
        virtual void onSpeed() {}
        virtual void onDel(Fleet *p) {}
      protected:
        Fwk::Ptr<Fleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Fleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Fleet() {}
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
    Fleet(const Fleet& o);
};

class TruckFleet : public Fleet {
    friend class EngineManager;
  public:
    ~TruckFleet() { if (notifiee_) notifiee_->onDel(this); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<TruckFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<TruckFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<TruckFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(TruckFleet *p) {}
      protected:
        Fwk::Ptr<TruckFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<TruckFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    TruckFleet() : Fleet() {}
    Ptr<TruckFleet::Notifiee> notifiee_;
    void notifieeIs(TruckFleet::Notifiee* n) const {
        TruckFleet* me = const_cast<TruckFleet*>(this);
        me->notifiee_ = n;
    }

  private:
    TruckFleet(const TruckFleet& o);
};

class BoatFleet : public Fleet {
    friend class EngineManager;
  public:
    ~BoatFleet() { if (notifiee_) notifiee_->onDel(this); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<BoatFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<BoatFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<BoatFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(BoatFleet *p) {}
      protected:
        Fwk::Ptr<BoatFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<BoatFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    BoatFleet() : Fleet() {}
    Ptr<BoatFleet::Notifiee> notifiee_;
    void notifieeIs(BoatFleet::Notifiee* n) const {
        BoatFleet* me = const_cast<BoatFleet*>(this);
        me->notifiee_ = n;
    }

  private:
    BoatFleet(const BoatFleet& o);
};

class PlaneFleet : public Fleet {
    friend class EngineManager;
  public:
    ~PlaneFleet() { if (notifiee_) notifiee_->onDel(this); }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<PlaneFleet> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->referencesDec();
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<PlaneFleet::Notifiee> notifieeNew() {
            Fwk::Ptr<PlaneFleet::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onDel(PlaneFleet *p) {}
      protected:
        Fwk::Ptr<PlaneFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<PlaneFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    PlaneFleet() : Fleet() {}
    Ptr<PlaneFleet::Notifiee> notifiee_;
    void notifieeIs(PlaneFleet::Notifiee* n) const {
        PlaneFleet* me = const_cast<PlaneFleet*>(this);
        me->notifiee_ = n;
    }

  private:
    PlaneFleet(const PlaneFleet& o);
};

} /* end namespace */

#endif
