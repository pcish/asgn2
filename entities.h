#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>

using namespace std;
#include "Ptr.h"
#include "WeakPtr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "types.h"
#include "fwk/BaseNotifiee.h"
#include "fwk/NamedInterface.h"
using Fwk::WeakPtr;
namespace Shipping {

class ShippingNetwork;
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

class Shipment : public Fwk::NamedInterface {
    friend class ShippingNetwork;
  public:
    ~Shipment() { if (notifiee_) notifiee_->onDel(this); }
    PackageUnit load() const { return load_; }
    void loadIs(const PackageUnit load) { if (load_ == load) return; load_ = load; if (notifiee_) notifiee_->onLoad(); }
    Ptr<Customer> destination() const { return destination_; }
    void destinationIs(const Ptr<Customer> destination) { if (destination_ == destination) return; destination_ = destination; if (notifiee_) notifiee_->onDestination(); }
    Ptr<Customer> source() const { return source_; }
    void sourceIs(const Ptr<Customer> source) { if (source_ == source) return; source_ = source; if (notifiee_) notifiee_->onSource(); }
    USD cost() const { return cost_; }
    void costInc(USD increment) { cost_ = cost_.value() + increment.value(); }
    Hour transitTime() const { return transitTime_; }
    void transitTimeInc(Hour increment) { transitTime_ = transitTime_.value() + increment.value(); }
    Ptr<Location> currentLocation() const { return currentLocation_; }
    void currentLocationIs(const Ptr<Location> currentLocation) { if (currentLocation_ == currentLocation) return; currentLocation_ = currentLocation; if (notifiee_) notifiee_->onCurrentLocation(); }
    ShippingNetwork* shippingNetwork() const { return shippingNetwork_; }
    void shippingNetworkIs(ShippingNetwork* shippingNetwork) { if (shippingNetwork_ == shippingNetwork) return; shippingNetwork_ = shippingNetwork; }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<Shipment> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Shipment::Notifiee> notifieeNew() {
            Fwk::Ptr<Shipment::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onLoad() {}
        virtual void onDestination() {}
        virtual void onSource() {}
        virtual void onCurrentLocation() {}
        virtual void onDel(Shipment *p) {}
      protected:
        Fwk::WeakPtr<Shipment> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Shipment::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Shipment(Fwk::String name) : NamedInterface(name) {}
    Ptr<Shipment::Notifiee> notifiee_;
    void notifieeIs(Shipment::Notifiee* n) const {
        Shipment* me = const_cast<Shipment*>(this);
        me->notifiee_ = n;
    }

  private:
    PackageUnit load_;
    Ptr<Customer> destination_;
    Ptr<Customer> source_;
    USD cost_;
    Hour transitTime_;
    Ptr<Location> currentLocation_;
    ShippingNetwork* shippingNetwork_;
    Shipment(const Shipment& o);
};

class Segment : public Fwk::NamedInterface {
    friend class ShippingNetwork;
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
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> source);
    WeakPtr<Segment> returnSegment() const { return returnSegment_; }
    void returnSegmentIs(const Ptr<Segment> returnSegment);
    TransportationMode transportationMode() const { return transportationMode_; }
    SegmentDifficultyUnit difficulty() const { return difficulty_; }
    void difficultyIs(const SegmentDifficultyUnit difficulty) { if (difficulty_ == difficulty) return; difficulty_ = difficulty; if (notifiee_) notifiee_->onDifficulty(); }
    Mile length() const { return length_; }
    void lengthIs(const Mile length) { if (length_ == length) return; length_ = length; if (notifiee_) notifiee_->onLength(); }
    ShipmentCount capacity() const { return capacity_; }
    void capacityIs(const ShipmentCount capacity) { if (capacity_ == capacity) return; capacity_ = capacity; if (notifiee_) notifiee_->onCapacity(); }
    ShipmentCount availableCapacity() const { return ShipmentCount(capacity_.value() - usedCapacity_.value()); }
    void usedCapacityInc() { if (usedCapacity_ >= capacity_) throw Fwk::InternalException("no available capacity"); usedCapacity_ = usedCapacity_.value() + 1; }
    void usedCapacityDec() { usedCapacity_ = usedCapacity_.value() - 1; }
    ShipmentCount shipmentsRefused() { return shipmentsRefused_; }
    ShipmentCount shipmentsReceived() { return shipmentsReceived_; }
    void shipmentsRefusedInc() { shipmentsRefused_ = shipmentsRefused_.value() + 1; }
    void shipmentsReceivedInc() { shipmentsReceived_ = shipmentsReceived_.value() + 1; }
    ShippingNetwork* shippingNetwork() const { return shippingNetwork_; }
    void shippingNetworkIs(ShippingNetwork* shippingNetwork) { if (shippingNetwork_ == shippingNetwork) return; shippingNetwork_ = shippingNetwork; if (notifiee_) notifiee_->onShippingNetwork(); }
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
        virtual void onExpediteSupport() {}
        virtual void onSource() {}
        virtual void onReturnSegment() {}
        virtual void onDifficulty() {}
        virtual void onLength() {}
        virtual void onCapacity() {}
        virtual void onShippingNetwork() {}
        virtual void onDel(Segment *p) {}
      protected:
        Fwk::WeakPtr<Segment> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Segment::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Segment(const string name, const TransportationMode transportationMode) : NamedInterface(name), transportationMode_(transportationMode) {
        expediteSupport_ = unavailable_;
        capacity_ = ShipmentCount(10);
        length_ = Mile(1);
    }
    Ptr<Segment::Notifiee> notifiee_;
    void notifieeIs(Segment::Notifiee* n) const {
        Segment* me = const_cast<Segment*>(this);
        me->notifiee_ = n;
    }

  private:
    ExpediteSupport expediteSupport_;
    Ptr<Location> source_;
    WeakPtr<Segment> returnSegment_;
    TransportationMode transportationMode_;
    SegmentDifficultyUnit difficulty_;
    Mile length_;
    ShipmentCount capacity_;
    ShipmentCount usedCapacity_;
    ShipmentCount shipmentsRefused_;
    ShipmentCount shipmentsReceived_;
    ShippingNetwork* shippingNetwork_;
    Segment(const Segment& o);
};

class Location : public Fwk::NamedInterface {
    friend class ShippingNetwork;
  public:
    enum LocationType {
        undefined_,
        customer_,
        port_,
        terminal_
    };
    static inline LocationType customer() { return customer_; }
    static inline LocationType port() { return port_; }
    static inline LocationType terminal() { return terminal_; }
    virtual LocationType locationType() const { return locationType_; }

    ~Location() { if (notifiee_) notifiee_->onDel(this); }
    virtual WeakPtr<Segment> segment(const unsigned int index) const;
    virtual unsigned int segments() const { return segments_.size(); }
    virtual void segmentIs(const WeakPtr<Segment> segment);
    virtual void segmentIs(const unsigned int index, WeakPtr<Segment> segment);
    ShippingNetwork* shippingNetwork() const { return shippingNetwork_; }
    void shippingNetworkIs(ShippingNetwork* shippingNetwork) { if (shippingNetwork_ == shippingNetwork) return; shippingNetwork_ = shippingNetwork; if (notifiee_) notifiee_->onShippingNetwork(); }
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
        virtual void onSegment() {}
        virtual void onShippingNetwork() {}
        virtual void onDel(Location *p) {}
      protected:
        Fwk::WeakPtr<Location> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Location::Notifiee> notifiee() const { return notifiee_; }
  protected:
    LocationType locationType_;
    Location(const string name) : NamedInterface(name) {
        locationType_ = undefined_;
    }
    Ptr<Location::Notifiee> notifiee_;
    void notifieeIs(Location::Notifiee* n) const {
        Location* me = const_cast<Location*>(this);
        me->notifiee_ = n;
    }

  private:
    std::vector<WeakPtr<Segment> > segments_;
    ShippingNetwork* shippingNetwork_;
    Location(const Location& o);
};

class Customer : public Location {
    friend class ShippingNetwork;
  public:
    ~Customer() { if (notifiee_) notifiee_->onDel(this); }
    WeakPtr<Location> destination() const { return destination_; }
    void destinationIs(const Ptr<Location> destination) { if (destination_ == destination) return; destination_ = destination; if (notifiee_) notifiee_->onDestination(); }
    PackageUnit shipmentSize() const { return shipmentSize_; }
    void shipmentSizeIs(const PackageUnit shipmentSize) { if (shipmentSize_ == shipmentSize) return; shipmentSize_ = shipmentSize; if (notifiee_) notifiee_->onShipmentSize(); }
    ShipmentCount transferRate() const { return transferRate_; }
    void transferRateIs(const ShipmentCount transferRate) { if (transferRate_ == transferRate) return; transferRate_ = transferRate; if (notifiee_) notifiee_->onTransferRate(); }
    ShipmentCount shipmentsReceived() const { return shipmentsReceived_; }
    void shipmentsReceivedInc() { shipmentsReceived_ = shipmentsReceived_.value() + 1; }
    Hour averageLatency() const { return ((shipmentsReceived_.value() != 0)? totalLatency_.value() / shipmentsReceived_.value() : 0); }
    void totalLatencyInc(Hour increment) { totalLatency_ = totalLatency_.value() + increment.value(); }
    USD totalCost() const { return totalCost_; }
    void totalCostInc(USD increment) { totalCost_ = totalCost_.value() + increment.value(); }
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
        virtual void onDel(Customer *p) {}
        virtual void onDestination() {}
        virtual void onShipmentSize() {}
        virtual void onTransferRate() {}
      protected:
        Fwk::WeakPtr<Customer> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Customer::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Customer(const string name) : Location(name) {
        locationType_ = customer_;
    }
    Ptr<Customer::Notifiee> notifiee_;
    void notifieeIs(Customer::Notifiee* n) const {
        Customer* me = const_cast<Customer*>(this);
        me->notifiee_ = n;
    }

  private:
    WeakPtr<Location> destination_;
    PackageUnit shipmentSize_;
    ShipmentCount transferRate_;
    ShipmentCount shipmentsReceived_;
    Hour totalLatency_;
    USD totalCost_;
    Customer(const Customer& o);
};

class Port : public Location {
    friend class ShippingNetwork;
  public:
    ~Port() { if (notifiee_) notifiee_->onDel(this); }
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
        virtual void onDel(Port *p) {}
      protected:
        Fwk::WeakPtr<Port> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Port::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Port(const string name) : Location(name) {
        locationType_ = port_;
    }
    Ptr<Port::Notifiee> notifiee_;
    void notifieeIs(Port::Notifiee* n) const {
        Port* me = const_cast<Port*>(this);
        me->notifiee_ = n;
    }

  private:
    Port(const Port& o);
};

class Terminal : public Location {
    friend class ShippingNetwork;
  public:
    ~Terminal() { if (notifiee_) notifiee_->onDel(this); }
    Segment::TransportationMode transportationMode() const { return transportationMode_; }
    virtual void segmentIs(const WeakPtr<Segment> segment);
    virtual void segmentIs(const unsigned int index, WeakPtr<Segment> segment);
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
        virtual void onDel(Terminal *p) {}
      protected:
        Fwk::WeakPtr<Terminal> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Terminal::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Terminal(const string name, const Segment::TransportationMode transportationMode) : Location(name), transportationMode_(transportationMode) {
        locationType_ = terminal_;
    }
    Ptr<Terminal::Notifiee> notifiee_;
    void notifieeIs(Terminal::Notifiee* n) const {
        Terminal* me = const_cast<Terminal*>(this);
        me->notifiee_ = n;
    }

  private:
    Segment::TransportationMode transportationMode_;
    Terminal(const Terminal& o);
};

class Fleet : public Fwk::NamedInterface {
    friend class ShippingNetwork;
  public:
    ~Fleet() { if (notifiee_) notifiee_->onDel(this); }
    USD cost() const { return cost_; }
    void costIs(const USD cost) { if (cost_ == cost) return; cost_ = cost; if (notifiee_) notifiee_->onCost(); }
    USD scheduledCost(const unsigned int index) const { return scheduledCosts_[index]; }
    void scheduledCostIs(const unsigned int index, USD cost) { scheduledCosts_[index] = cost; }
    PackageUnit capacity() const { return capacity_; }
    void capacityIs(const PackageUnit capacity) { if (capacity_ == capacity) return; capacity_ = capacity; if (notifiee_) notifiee_->onCapacity(); }
    PackageUnit scheduledCapacity(const unsigned int index) const { return scheduledCapacitys_[index]; }
    void scheduledCapacityIs(const unsigned int index, PackageUnit capacity) { scheduledCapacitys_[index] = capacity; }
    Segment::TransportationMode transportationMode() const { return transportationMode_; }
    void transportationModeIs(const Segment::TransportationMode transportationMode) { if (transportationMode_ == transportationMode) return; transportationMode_ = transportationMode; if (notifiee_) notifiee_->onTransportationMode(); }
    Mile speed() const { return speed_; }
    void speedIs(const Mile speed) { if (speed_ == speed) return; speed_ = speed; if (notifiee_) notifiee_->onSpeed(); }
    Mile scheduledSpeed(const unsigned int index) const { return scheduledSpeeds_[index]; }
    void scheduledSpeedIs(const unsigned int index, Mile speed) { scheduledSpeeds_[index] = speed; }
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
        virtual void onCost() {}
        virtual void onCapacity() {}
        virtual void onTransportationMode() {}
        virtual void onSpeed() {}
        virtual void onDel(Fleet *p) {}
      protected:
        Fwk::WeakPtr<Fleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<Fleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Fleet(Fwk::String name) : NamedInterface(name) {
        for (unsigned int i = 0; i < 24; i++) {
            scheduledCosts_[i] = 1;
            scheduledSpeeds_[i] = 1;
            scheduledCapacitys_[i] = 100;
        }
    }
    Ptr<Fleet::Notifiee> notifiee_;
    void notifieeIs(Fleet::Notifiee* n) const {
        Fleet* me = const_cast<Fleet*>(this);
        me->notifiee_ = n;
    }

  private:
    USD cost_;
    USD scheduledCosts_[24];
    Mile speed_;
    Mile scheduledSpeeds_[24];
    PackageUnit capacity_;
    PackageUnit scheduledCapacitys_[24];
    Segment::TransportationMode transportationMode_;
    Fleet(const Fleet& o);
};

class TruckFleet : public Fleet {
    friend class ShippingNetwork;
  public:
    ~TruckFleet() { if (notifiee_) notifiee_->onDel(this); }
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
        virtual void onDel(TruckFleet *p) {}
      protected:
        Fwk::WeakPtr<TruckFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<TruckFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    TruckFleet(Fwk::String name) : Fleet(name) {}
    Ptr<TruckFleet::Notifiee> notifiee_;
    void notifieeIs(TruckFleet::Notifiee* n) const {
        TruckFleet* me = const_cast<TruckFleet*>(this);
        me->notifiee_ = n;
    }

  private:
    TruckFleet(const TruckFleet& o);
};

class BoatFleet : public Fleet {
    friend class ShippingNetwork;
  public:
    ~BoatFleet() { if (notifiee_) notifiee_->onDel(this); }
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
        virtual void onDel(BoatFleet *p) {}
      protected:
        Fwk::WeakPtr<BoatFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<BoatFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    BoatFleet(Fwk::String name) : Fleet(name) {}
    Ptr<BoatFleet::Notifiee> notifiee_;
    void notifieeIs(BoatFleet::Notifiee* n) const {
        BoatFleet* me = const_cast<BoatFleet*>(this);
        me->notifiee_ = n;
    }

  private:
    BoatFleet(const BoatFleet& o);
};

class PlaneFleet : public Fleet {
    friend class ShippingNetwork;
  public:
    ~PlaneFleet() { if (notifiee_) notifiee_->onDel(this); }
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
        virtual void onDel(PlaneFleet *p) {}
      protected:
        Fwk::WeakPtr<PlaneFleet> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<PlaneFleet::Notifiee> notifiee() const { return notifiee_; }
  protected:
    PlaneFleet(Fwk::String name) : Fleet(name) {}
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
