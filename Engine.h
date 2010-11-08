#ifndef ENGINE_MNG_H
#define ENGINE_MNG_H
#include "entities.h"
#include "Ptr.h"
#include "PtrInterface.h"

namespace Shipping {
class EngineReactor;
class SegmentReactor;
class CustomerReactor;
class PortReactor;
class TerminalReactor;
class ShippingNetwork : public Fwk::PtrInterface<ShippingNetwork> {
    friend class EngineReactor;
    friend class SegmentReactor;
    friend class CustomerReactor;
    friend class PortReactor;
    friend class TerminalReactor;
  public:
    ~ShippingNetwork(){}
    int customers() const { return customers_; }
    int segments() const { return truckSegments_ + planeSegments_ + boatSegments_; }
    int truckSegments() const {
        if (expedite_ == Segment::available()) return truckSegmentsExpediteAvailable_;
        else return truckTerminals_;
    }
    int planeSegments() const {
        if (expedite_ == Segment::available()) return planeSegmentsExpediteAvailable_;
        else return planeTerminals_;
    }
    int boatSegments() const {
        if (expedite_ == Segment::available()) return boatSegmentsExpediteAvailable_;
        else return boatTerminals_;
    }
    int terminals() const { return truckTerminals_ + planeTerminals_ + boatTerminals_; }
    int truckTerminals() const { return truckTerminals_; }
    int planeTerminals() const { return planeTerminals_; }
    int boatTerminals() const { return boatTerminals_; }
    Ptr<Location> destination() const { return destination_; }
    void destinationIs(const Ptr<Location> destination) { if (destination_ == destination) return; destination_ = destination; }
    USD maxCost() const { return maxCost_; }
    void maxCostIs(const USD maxCost) { if (maxCost_ == maxCost) return; maxCost_ = maxCost; }
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> source) { if (source_ == source) return; source_ = source; }

    int path() const { return path_; }
    int maxTime() const { return maxTime_; }
    void maxTimeIs(const int maxTime) { if (maxTime_ == maxTime) return; maxTime_ = maxTime; }
    Mile maxDistance() const { return maxDistance_; }
    void maxDistanceIs(const Mile maxDistance) { if (maxDistance_ == maxDistance) return; maxDistance_ = maxDistance; }
    int ports() const { return ports_; }
    Segment::ExpediteSupport expedite() const { return expedite_; }
    void expediteIs(const Segment::ExpediteSupport expedite) { if (expedite_ == expedite) return; expedite_ = expedite; }
    ShippingNetwork() : customers_(0), ports_(0), truckTerminals_(0), planeTerminals_(0), boatTerminals_(0), truckSegments_(0), planeSegments_(0), boatSegments_(0),
    truckSegmentsExpediteAvailable_(0), planeSegmentsExpediteAvailable_(0), boatSegmentsExpediteAvailable_(0), expedite_(Segment::allAvailabilities()) {
    }
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
    Ptr<Location> destination_;
    USD maxCost_;
    Ptr<Location> source_;
    int path_;
    int maxTime_;
    Mile maxDistance_;

    Segment::ExpediteSupport expedite_;
    ShippingNetwork(const ShippingNetwork& o);
};

class EngineManager : public Fwk::PtrInterface<EngineManager> {
  public:
    EngineManager();
    Ptr<ShippingNetwork> shippingNetwork() const { return network_; }
    Ptr<Customer> customerNew(const string name);
    Ptr<PlaneFleet> planeFleetNew(){
        Ptr<PlaneFleet> m = new PlaneFleet();
        if (notifiee_) notifiee_->onPlaneFleetNew(m);
        return m;
    }
    Ptr<TruckFleet> truckFleetNew(){
        Ptr<TruckFleet> m = new TruckFleet();
        if (notifiee_) notifiee_->onTruckFleetNew(m);
        return m;
    }
    Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode);
    Ptr<BoatFleet> boatFleetNew(){
        Ptr<BoatFleet> m = new BoatFleet();
        if (notifiee_) notifiee_->onBoatFleetNew(m);
        return m;
    }
    Ptr<Segment> segmentNew(const Segment::TransportationMode transportationMode, const string name);
    Ptr<Port> portNew(const string name);
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
};



}
#endif
