#ifndef ENGINE_MNG_H
#define ENGINE_MNG_H
#include "entities.h"
#include "Ptr.h"
#include "PtrInterface.h"
#include <vector>
#include <set>

namespace Shipping {
class EngineReactor;
class SegmentReactor;
class CustomerReactor;
class PortReactor;
class TerminalReactor;
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
    Ptr<Path> clone();
private:
    Path() {}
    Path(Path&) {}
    vector<Ptr<Location> > location_;
    vector<WeakPtr<Segment> > segment_;
    USD cost_;
    Mile distance_;
    Segment::ExpediteSupport expedite_;
    Hour hour_;

};
class ShippingNetwork : public Fwk::PtrInterface<ShippingNetwork> {
    friend class EngineReactor;
    friend class SegmentReactor;
    friend class CustomerReactor;
    friend class PortReactor;
    friend class TerminalReactor;
  public:
    ~ShippingNetwork() {}
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
    int truckTerminals() const { return truckTerminals_; }
    int planeTerminals() const { return planeTerminals_; }
    int boatTerminals() const { return boatTerminals_; }

    void truckFleetIs(const Ptr<TruckFleet> _truckFleet) { truckFleet_ = _truckFleet; }
    void planeFleetIs(const Ptr<PlaneFleet> _planeFleet) { planeFleet_ = _planeFleet; }
    void boatFleetIs(const Ptr<BoatFleet> _boatFleet) { boatFleet_ = _boatFleet; }
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
    int ports() const { return ports_; }
    Segment::ExpediteSupport expedite() const { return expedite_; }
    void expediteIs(const Segment::ExpediteSupport expedite) { if (expedite_ == expedite) return; expedite_ = expedite; isConnAttributeChange = true; }
    enum Routing {
        dijkstra__, bfs__
    };
    static inline Routing dijkstra() { return dijkstra__; }
    static inline Routing bfs(){ return bfs__; }
    Routing routing() const { return routing_; }
    void routingIs(const Routing _routing) { routing_ = _routing; }
    Ptr<Shipment> shipmentNew(const string name);

    ShippingNetwork() : customers_(0), ports_(0), truckTerminals_(0), planeTerminals_(0), boatTerminals_(0),
                        truckSegments_(0), planeSegments_(0), boatSegments_(0),
                        truckSegmentsExpediteAvailable_(0), planeSegmentsExpediteAvailable_(0), boatSegmentsExpediteAvailable_(0),
                        isConnAttributeChange(false), expedite_(Segment::allAvailabilities()), routing_(bfs__){}

  protected:
    void customersInc() { customers_++; }
  private:
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
    list<Ptr<Location> > visitedNodes;
};

class EngineManager : public Fwk::PtrInterface<EngineManager> {
  public:
    EngineManager();
    Ptr<ShippingNetwork> shippingNetwork() const { return network_; }
    Ptr<Customer> customerNew(const string name);
    Ptr<PlaneFleet> planeFleetNew() {
        Ptr<PlaneFleet> m = new PlaneFleet();
        if (notifiee_) notifiee_->onPlaneFleetNew(m);
        return m;
    }
    Ptr<TruckFleet> truckFleetNew() {
        Ptr<TruckFleet> m = new TruckFleet();
        if (notifiee_) notifiee_->onTruckFleetNew(m);
        return m;
    }
    Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode);
    Ptr<BoatFleet> boatFleetNew() {
        Ptr<BoatFleet> m = new BoatFleet();
        if (notifiee_) notifiee_->onBoatFleetNew(m);
        return m;
    }
    Ptr<Segment> segmentNew(const Segment::TransportationMode transportationMode, const string name);
    Ptr<Port> portNew(const string name);
    void customerDel(Ptr<Customer> o);
    void terminalDel(Ptr<Terminal> o);
    void portDel(Ptr<Port> o);
    void segmentDel(Ptr<Segment> o);
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
      public:
        virtual void notifierIs(Fwk::Ptr<EngineManager> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<EngineManager::Notifiee> notifieeNew() {
            Fwk::Ptr<EngineManager::Notifiee> n = new Notifiee();
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
        Fwk::Ptr<EngineManager> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<EngineManager::Notifiee> notifiee() const { return notifiee_; }

  protected:
    Ptr<ShippingNetwork> network_;
    Ptr<EngineManager::Notifiee> notifiee_;
    void notifieeIs(EngineManager::Notifiee* n) {
        //EngineManager* me = const_cast<EngineManager*>(this);
        //me->notifiee_ = n;
        notifiee_ = n;
    }
    void locationDel(Ptr<Location> o);
};



}
#endif
