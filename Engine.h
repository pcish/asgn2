#ifndef ENGINE_MNG_H
#define ENGINE_MNG_H
#include "entities.h"
#include "ShippingNetwork.h"
#include "Ptr.h"
#include "PtrInterface.h"
namespace Shipping {
class EngineManager : public Fwk::PtrInterface<EngineManager> {
  public:
    EngineManager() {
        network_ = new ShippingNetwork();
    }
     Ptr<Customer> customerNew(const string name){
        Ptr<Customer> m = new Customer(name);
        return m;
    }
     Ptr<PlaneFleet> planeFleetNew(){
        Ptr<PlaneFleet> m = new PlaneFleet();
        return m;
    }
     Ptr<TruckFleet> truckFleetNew(){
        Ptr<TruckFleet> m = new TruckFleet();
        return m;
    }
     Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode){
        Ptr<Terminal> m = new Terminal(name, transportationMode);
        return m;
    }
     Ptr<Location> locationNew(const string name){
        Ptr<Location> m = new Location(name);
        return m;
    }
     Ptr<BoatFleet> boatFleetNew(){
        Ptr<BoatFleet> m = new BoatFleet();
        return m;
    }
     Ptr<Fleet> fleetNew(){
        Ptr<Fleet> m = new Fleet();
        return m;
    }
     Ptr<Segment> segmentNew(const Segment::TransportationMode transportationMode, const string name){
        Ptr<Segment> m = new Segment(transportationMode, name);
        return m;
    }
     Ptr<Port> portNew(const string name){
        Ptr<Port> m = new Port(name);
        return m;
    }
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
        virtual void onCustomerNew() {}
        virtual void onPlaneFleetNew() {}
        virtual void onTruckFleetNew() {}
        virtual void onTerminalNew() {}
        virtual void onLocationNew() {}
        virtual void onBoatFleetNew() {}
        virtual void onFleetNew() {}
        virtual void onSegmentNew() {}
        virtual void onPortNew() {}
      protected:
        Fwk::Ptr<EngineManager> notifier_;
        Notifiee() : notifier_(0) {}
    };
    Ptr<EngineManager::Notifiee> notifiee() const { return notifiee_; }
  protected:
    Ptr<ShippingNetwork> network_;
    Ptr<EngineManager::Notifiee> notifiee_;
    void notifieeIs(EngineManager::Notifiee* n) const {
        EngineManager* me = const_cast<EngineManager*>(this);
        me->notifiee_ = n;
    }
};
}
#endif
