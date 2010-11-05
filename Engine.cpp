#include "Engine.h"

namespace Shipping {

class EngineReactor : public EngineManager::Notifiee {
  public:
    virtual void onCustomerNew() {
        notifier_->shippingNetwork()->customersInc();
    }
    virtual void onTerminalNew() {
        notifier_->shippingNetwork()->terminals_++;
    }
    virtual void onSegmentNew() {
        notifier_->shippingNetwork()->segments_++;
    }
    virtual void onPortNew() {
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
