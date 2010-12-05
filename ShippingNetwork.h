#ifndef SHIPPING_NETWORK_H
#define SHIPPING_NETWORK_H
#include "entities.h"
#include "Ptr.h"
#include "PtrInterface.h"
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include "ActivityImpl.h"
#include "Log.h"

namespace Shipping {
class EngineReactor;
class SegmentReactor;
class CustomerReactor;
class PortReactor;
class TerminalReactor;
class ShipmentReactor;

class Path : public Fwk::PtrInterface<Path> {
public:
    static Ptr<Path> pathNew() { return new Path(); }
    void locationIs (Ptr<Location> _location) {
        if (_location) location_.push_back(_location);
        else if (location_.size() > 0) location_.pop_back();
    }
    void segmentIs(WeakPtr<Segment> _segment) {
        if (_segment) segment_.push_back(_segment);
        else if (segment_.size() > 0) segment_.pop_back();
    }
    Ptr<Location> location(const unsigned int index) const {
        return (index < location_.size() && index >= 0) ? location_[index]:NULL;
    }
    WeakPtr<Segment> segment(const unsigned int index) const {
        return (index < segment_.size() && index >= 0) ? segment_[index] : NULL;
    }
    unsigned int locations() const { return location_.size(); }
    unsigned int segments() const { return segment_.size(); }
    void hourIs(const Hour _hour) { hour_ = _hour; }
    Hour hour() const { return hour_; }
    void costIs(const USD _cost) { cost_ = _cost; }
    USD cost() const { return cost_; }
    Segment::ExpediteSupport expedite() const { return expedite_; }
    void expediteIs(const Segment::ExpediteSupport _expedite) { expedite_ = _expedite; }
    Mile distance() const { return distance_; }
    void distanceIs(const Mile _distance) { distance_ = _distance; }
    void reversedIs (bool _reversed) {
        if (_reversed != reversed_) {
            reverse(location_.begin(), location_.end());
            reverse(segment_.begin(), segment_.end());
            reversed_ = _reversed;
        }
    }
    bool reversed() const { return reversed_; }
    Ptr<Path> clone();
private:
    Path() : reversed_(false) {}
    Path(Path&) {}
    vector<Ptr<Location> > location_;
    vector<WeakPtr<Segment> > segment_;
    USD cost_;
    Mile distance_;
    Segment::ExpediteSupport expedite_;
    Hour hour_;
    bool reversed_;
};

class ShippingNetwork : public Fwk::PtrInterface<ShippingNetwork> {
    friend class ShippingNetworkReactor;
    friend class ShipmentReactor;
  public:
    class Statistics : public Fwk::NamedInterface {
        friend class ShippingNetworkReactor;
        friend class SegmentReactor;
        friend class CustomerReactor;
        friend class PortReactor;
        friend class TerminalReactor;
      public:
        int customers() const { return customers_; }
        int segments() const { return truckSegments_ + planeSegments_ + boatSegments_; }
        int truckSegments() const {
            if (expedite_ == Segment::available()) return truckSegmentsExpediteAvailable_;
            else return truckSegments_;
        }
        int planeSegments() const {
            if (expedite_ == Segment::available()) return planeSegmentsExpediteAvailable_;
            else return planeSegments_;
        }
        int boatSegments() const {
            if (expedite_ == Segment::available()) return boatSegmentsExpediteAvailable_;
            else return boatSegments_;
        }
        int terminals() const { return truckTerminals_ + planeTerminals_ + boatTerminals_; }
        int ports() const { return ports_; }
        int truckTerminals() const { return truckTerminals_; }
        int planeTerminals() const { return planeTerminals_; }
        int boatTerminals() const { return boatTerminals_; }
        Segment::ExpediteSupport expedite() const { return expedite_; }
        void expediteIs(const Segment::ExpediteSupport expedite) { if (expedite_ == expedite) return; expedite_ = expedite; }
        Statistics(Fwk::String name);

      private:
        void customersInc() { customers_++; }
        int customers_;
        int ports_;
        int truckTerminals_;
        int planeTerminals_;
        int boatTerminals_;
        int truckSegments_;
        int planeSegments_;
        int boatSegments_;
        int truckSegmentsExpediteAvailable_;
        int planeSegmentsExpediteAvailable_;
        int boatSegmentsExpediteAvailable_;
        Segment::ExpediteSupport expedite_;
    };
    Ptr<ShippingNetwork::Statistics> statistics() const { return statistics_; }

    ~ShippingNetwork() {}
    Ptr<Segment> segmentNew(const Segment::TransportationMode transportationMode, const string name);
    Ptr<Customer> customerNew(const string name);
    Ptr<Port> portNew(const string name);
    Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode);
    Ptr<PlaneFleet> planeFleetNew(const string name);
    Ptr<TruckFleet> truckFleetNew(const string name);
    Ptr<BoatFleet> boatFleetNew(const string name);
    void customerDel(Ptr<Customer> o);
    void terminalDel(Ptr<Terminal> o);
    void portDel(Ptr<Port> o);
    void segmentDel(Ptr<Segment> o);

    Ptr<TruckFleet> truckFleet() const { return truckFleet_; }
    Ptr<PlaneFleet> planeFleet() const { return planeFleet_; }
    Ptr<BoatFleet> boatFleet() const { return boatFleet_; }
    /*Connectivity*/
    Ptr<Location> destination() const { return destination_; }
    void destinationIs(const Ptr<Location> destination) { if (destination_ == destination) return; destination_ = destination; isConnAttributeChange = true; }
    USD maxCost() const { return maxCost_; }
    void maxCostIs(const USD maxCost) { if (maxCost_ == maxCost) return; maxCost_ = maxCost; isConnAttributeChange = true;}
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> source) { if (source_ == source) return; source_ = source; isConnAttributeChange = true; }

    Ptr<Path> path(unsigned int index);
    unsigned int paths();
    Hour maxTime() const { return maxTime_; }
    void maxTimeIs(const Hour maxTime) { if (maxTime_ == maxTime) return; maxTime_ = maxTime; isConnAttributeChange = true;}
    Mile maxDistance() const { return maxDistance_; }
    void maxDistanceIs(const Mile maxDistance) { if (maxDistance_ == maxDistance) return; maxDistance_ = maxDistance; isConnAttributeChange = true; }

    Segment::ExpediteSupport expedite() const { return expedite_; }
    void expediteIs(const Segment::ExpediteSupport expedite) { if (expedite_ == expedite) return; expedite_ = expedite; isConnAttributeChange = true; }
    enum Routing {
        dijkstra__, bfs__, randomwalk__
    };
    static inline Routing dijkstra() { return dijkstra__; }
    static inline Routing bfs(){ return bfs__; }
    static inline Routing randomwalk() { return randomwalk__; }
    Routing routing() const { return routing_; }
    void routingIs(const Routing _routing) { routing_ = _routing; }
    Ptr<Shipment> shipmentNew();
    Ptr<Path> nextHop(const WeakPtr<Shipment> shipment);

    ShippingNetwork();
    string name() { return "shippingNetwork"; }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<ShippingNetwork> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<ShippingNetwork::Notifiee> notifieeNew() {
            Fwk::Ptr<ShippingNetwork::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onCustomerNew(Fwk::Ptr<Customer> p) {}
        virtual void onPlaneFleetNew(Fwk::Ptr<PlaneFleet> p) {}
        virtual void onTruckFleetNew(Fwk::Ptr<TruckFleet> p) {}
        virtual void onTerminalNew(Fwk::Ptr<Terminal> p) {}
        virtual void onLocationNew(Fwk::Ptr<Location> p) {}
        virtual void onBoatFleetNew(Fwk::Ptr<BoatFleet> p) {}
        virtual void onFleetNew(Fwk::Ptr<Fleet> p) {}
        virtual void onSegmentNew(Fwk::Ptr<Segment> p) {}
        virtual void onPortNew(Fwk::Ptr<Port> p) {}
      protected:
        Fwk::Ptr<ShippingNetwork> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<ShippingNetwork::Notifiee> notifiee() const { return notifiee_; }

  protected:
    Ptr<ShippingNetwork::Notifiee> notifiee_;
    void notifieeIs(ShippingNetwork::Notifiee* n) {
        notifiee_ = n;
    }
    void locationDel(Ptr<Location> o);
    void deliverShipment(WeakPtr<Shipment> shipment);
    void dropShipment(WeakPtr<Shipment> shipment);

  private:
    Ptr<Statistics> statistics_;
    map<string, Ptr<Location> > location_;
    map<string, Ptr<Segment> > segment_;
    bool isConnAttributeChange;

    Ptr<TruckFleet> truckFleet_;
    Ptr<PlaneFleet> planeFleet_;
    Ptr<BoatFleet> boatFleet_;
    Ptr<Location> destination_;
    USD maxCost_;
    Ptr<Location> source_;
    vector<Ptr<Path> > path_;
    Hour maxTime_;
    Mile maxDistance_;
    Segment::ExpediteSupport expedite_;
    Routing routing_;
    vector<Ptr<Shipment> > shipment_;

    ShippingNetwork(const ShippingNetwork& o);
    void computePath();
    void explore(Ptr<Location> curLocation, set<string> visitedNodes, Ptr<Path> curPath);

    class Clock : public Fwk::NamedInterface {
      public:
        Clock();
        Hour now() const { return now_; }
        class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
          public:
            virtual void notifierIs(Fwk::Ptr<Clock> notifier) {
                if (notifier_ == notifier) return;
                if (notifier_) notifier->notifieeIs(0);
                notifier_ = notifier;
                notifier_->notifieeIs(this);
            }
            static Fwk::Ptr<Clock::Notifiee> notifieeNew() {
                Fwk::Ptr<Clock::Notifiee> n = new Notifiee();
                return n;
            }
            virtual void onNow() {}
          protected:
            Fwk::WeakPtr<Clock> notifier_;
            Notifiee() : notifier_(0) {}
        };
      protected:
        vector<Ptr<Clock::Notifiee> > notifiee_;
        void notifieeIs(Clock::Notifiee* n) {
            notifiee_.push_back(n);
        }
      private:
        Hour now_;
        void nowInc() {
            Activity::Manager::Ptr manager = activityManagerInstance();
            now_ = now_.value() + 1;
            LOG_INFO("nowInc", "now = " + STR(now_.value()));
            for(vector<Ptr<Clock::Notifiee> >::iterator i = notifiee_.begin(); i != notifiee_.end(); i++) {
                (*i)->onNow();
            }
            heartbeatActivity_->nextTimeIs(manager->now().value() + 1);
            //heartbeatActivity_->lastNotifieeIs(new HeartbeatActivityNotifiee(heartbeatActivity_.ptr(), this));
            manager->lastActivityIs(heartbeatActivity_.ptr());
        }
        Activity::Activity::Ptr heartbeatActivity_;
        class HeartbeatActivityNotifiee : public Activity::Activity::Notifiee {
          public:
            HeartbeatActivityNotifiee(Activity::Activity *activity, Clock *_parent) :
                Activity::Activity::Notifiee(activity), activity_(activity),
                parent_(_parent) {}
            virtual void onStatus() {
                Activity::Activity::Status status = activity_->status();
                if (status == Activity::Activity::executing) {
                    parent_->nowInc();
                }
            }
            virtual void onNextTime() {}
          private:
            Activity::Activity *activity_;
            Clock *parent_;
        };
    };
    Ptr<Clock> clock_;
};

}
#endif
