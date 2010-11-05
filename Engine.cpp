#include "Engine.h"

namespace Shipping {

class EngineReactor : public EngineManager::Notifiee {
  public:
    virtual void onCustomerNew() {
        notifier_->shippingNetwork()->customers_++;
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
};

}
