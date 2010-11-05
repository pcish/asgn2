#ifndef SHIPPING_NETWORK_H
#define SHIPPING_NETWORK_H

#include "Engine.h"
#include "types.h"
#include "EngineManager.h"

namespace Shipping {
class Location;

class ShippingNetwork : public Fwk::PtrInterface<ShippingNetwork> {
  public:
    ~ShippingNetwork(){}
    int customers() const { return customers_; }
    int segments() const { return segments_; }
    Ptr<Location> destination() const { return destination_; }
    void destinationIs(const Ptr<Location> destination) { if (destination_ == destination) return; destination_ = destination; }
    USD maxCost() const { return maxCost_; }
    void maxCostIs(const USD maxCost) { if (maxCost_ == maxCost) return; maxCost_ = maxCost; }
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> source) { if (source_ == source) return; source_ = source; }
    int terminals() const { return terminals_; }
    int path() const { return path_; }
    int maxTime() const { return maxTime_; }
    void maxTimeIs(const int maxTime) { if (maxTime_ == maxTime) return; maxTime_ = maxTime; }
    Mile maxDistance() const { return maxDistance_; }
    void maxDistanceIs(const Mile maxDistance) { if (maxDistance_ == maxDistance) return; maxDistance_ = maxDistance; }
    int ports() const { return ports_; }
    Segment::ExpediteSupport expedite() const { return expedite_; }
    void expediteIs(const Segment::ExpediteSupport expedite) { if (expedite_ == expedite) return; expedite_ = expedite; }
    ShippingNetwork() {
        engineReactor_ = EngineManager::Notifiee::notifieeNew();
    }

  private:
    int customers_;
    int segments_;
    Ptr<Location> destination_;
    USD maxCost_;
    Ptr<Location> source_;
    int terminals_;
    int path_;
    int maxTime_;
    Mile maxDistance_;
    int ports_;
    Segment::ExpediteSupport expedite_;
    ShippingNetwork(const ShippingNetwork& o);

    class EngineReactor : public EngineManager::Notifiee {
      public:
        virtual void onCustomerNew() { customers_++; }
        virtual void onTerminalNew() { terminals_++; }
        virtual void onSegmentNew() { segments_++; }
        virtual void onPortNew() { ports_++; }
    };

    EngineReactor engineReactor_;
};
}

#endif
