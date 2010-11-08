#ifndef ENTITY_REACTOR_H
#define ENTITY_REACTOR_H

#include "entities.h"
#include "Engine.h"
namespace Shipping {
class ShippingNetwork;
class SegmentReactor : public Segment::Notifiee {
  public:
    virtual void onDel(Segment *p) {
        int delta = 0;
        if (notifier_->expediteSupport() == Segment::available()) {
            delta = 1;
        }
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckSegments_--;
            notifier_->shippingNetwork()->truckSegmentsExpediteAvailable_ -= delta;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeSegments_--;
            notifier_->shippingNetwork()->planeSegmentsExpediteAvailable_ -= delta;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatSegments_--;
            notifier_->shippingNetwork()->boatSegmentsExpediteAvailable_ -= delta;
        }
    }
    virtual void onExpediteSupport() {
        int delta;
        if (notifier_->expediteSupport() == Segment::available()) {
            delta = 1;
        } else if (notifier_->expediteSupport() == Segment::unavailable()) {
            delta = -1;
        }
        if (notifier_->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckSegmentsExpediteAvailable_ += delta;
        } else if (notifier_->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeSegmentsExpediteAvailable_ += delta;
        } else if (notifier_->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatSegmentsExpediteAvailable_ += delta;
        }
    }
    virtual void onSource() {
        if (previousSource_ != NULL) {
            int i = 1;
            while (previousSource_->segment(i).ptr() != notifier_.ptr()) i++;
            previousSource_->segmentIs(i, 0);
        }
        if (notifier_->source() != NULL) {
            notifier_->source()->segmentIs(notifier_);
        }
    }
    static Fwk::Ptr<SegmentReactor> segmentReactorNew() {
        Fwk::Ptr<SegmentReactor> n = new SegmentReactor();
        return n;
    }
  protected:
    SegmentReactor() {
        previousSource_ = NULL;
    }
    Location *previousSource_;
};
class CustomerReactor : public Customer::Notifiee {
  public:
    virtual void onDel(Customer *p) {
        notifier_->shippingNetwork()->customers_--;
    }
    static Fwk::Ptr<CustomerReactor> customerReactorNew() {
        Fwk::Ptr<CustomerReactor> n = new CustomerReactor();
        return n;
    }
};
class PortReactor : public Port::Notifiee {
  public:
    virtual void onDel(Port *p) {
        notifier_->shippingNetwork()->ports_--;
    }
    static Fwk::Ptr<PortReactor> portReactorNew() {
        Fwk::Ptr<PortReactor> n = new PortReactor();
        return n;
    }
};
class TerminalReactor : public Terminal::Notifiee {
  public:
    virtual void onDel(Terminal *p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckTerminals_--;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeTerminals_--;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatTerminals_--;
        }
    }
    static Fwk::Ptr<TerminalReactor> terminalReactorNew() {
        Fwk::Ptr<TerminalReactor> n = new TerminalReactor();
        return n;
    }
};
}

#endif
