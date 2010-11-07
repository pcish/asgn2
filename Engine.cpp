#include "Engine.h"

namespace Shipping {

class EngineReactor : public EngineManager::Notifiee {
  public:
    virtual void onCustomerNew(Fwk::Ptr<Customer> p) {
        notifier_->shippingNetwork()->customersInc();
    }
    virtual void onTerminalNew(Fwk::Ptr<Terminal> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckTerminals_++;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeTerminals_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatTerminals_++;
        }
    }
    virtual void onSegmentNew(Fwk::Ptr<Segment> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckSegments_++;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeSegments_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatSegments_++;
        }
    }
    virtual void onPortNew(Fwk::Ptr<Port> p) {
        notifier_->shippingNetwork()->ports_++;
    }
    static Fwk::Ptr<EngineReactor> engineReactorNew() {
        Fwk::Ptr<EngineReactor> n = new EngineReactor();
        return n;
    }
};

EngineManager::EngineManager() {
    network_ = new ShippingNetwork();
    Fwk::Ptr<EngineReactor> r = EngineReactor::engineReactorNew();
    r->notifierIs(this);
}

}
