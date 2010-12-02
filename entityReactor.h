#ifndef ENTITY_REACTOR_H
#define ENTITY_REACTOR_H

#include "entities.h"
#include "Engine.h"
#include "ActivityImpl.h"

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
        if (notifier_->source() != NULL) {
            unsigned int i = 0;
            while (notifier_->source()->segment(i).ptr() != notifier_.ptr() && i < notifier_->source()->segments()) i++;
            notifier_->source()->segmentIs(i, NULL);
        }
        if (notifier_->returnSegment() != NULL) {
            notifier_->returnSegment()->returnSegmentIs(NULL);
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
            unsigned int i = 0;
            while (previousSource_->segment(i).ptr() != notifier_.ptr() && i < previousSource_->segments()) i++;
            previousSource_->segmentIs(i, NULL);
        }
        if (notifier_->source() != NULL) {
            notifier_->source()->segmentIs(notifier_.ptr());
        }
        previousSource_ = notifier_->source().ptr();
    }
    virtual void onReturnSegment() {
        if (previousReturnSegment_ != NULL && previousReturnSegment_->returnSegment() == notifier_) {
            previousReturnSegment_->returnSegmentIs(NULL);
        }
        if (notifier_->returnSegment() != NULL) {
            notifier_->returnSegment()->returnSegmentIs(notifier_.ptr());
        }
        previousReturnSegment_ = notifier_->returnSegment();
    }
    static Fwk::Ptr<SegmentReactor> segmentReactorNew() {
        Fwk::Ptr<SegmentReactor> n = new SegmentReactor();
        return n;
    }
  protected:
    SegmentReactor() {
        previousSource_ = NULL;
        previousReturnSegment_ = NULL;
        handlingReturnSegment_ = false;
    }
    Location *previousSource_;
    WeakPtr<Segment> previousReturnSegment_;
    bool handlingReturnSegment_;
};
class CustomerReactor : public Customer::Notifiee{
  public:
    virtual void onDel(Customer *p) {
        notifier_->shippingNetwork()->customers_--;
    }
    virtual void onDestination() { 
        destSet = true;
        checkAndLaunch();
    }
    virtual void onShipmentSize() {
        shipmentSizeSet = true;
        checkAndLaunch();
    }
    virtual void onTransferRate() {
        transferRateSet = true;
        checkAndLaunch();
    }
    static Fwk::Ptr<CustomerReactor> customerReactorNew() {
        Fwk::Ptr<CustomerReactor> n = new CustomerReactor();
        return n;
    }
  private:
    class ActivityNotifiee : public Activity::Activity::Notifiee {
      public: 
      //Pointer here may need to be refined
        ActivityNotifiee(Activity::Activity *activity, CustomerReactor *_parent) : Activity::Activity::Notifiee(activity), activity_(activity), parent_(_parent){}
        virtual void onStatus() {
            Activity::Activity::Status status = activity_->status();
            if (status == Activity::Activity::executing) { //run
                parent_->shipmentNew();
            }
        }
        virtual void onNextTime() {}
      private:
        Activity::Activity *activity_;
        CustomerReactor *parent_;
    };
    void checkAndLaunch() {
        if (!started) {
            if (destSet && shipmentSizeSet && transferRateSet && notifier_) {
                started = true;
                //lauch the shipment
                Activity::Manager::Ptr manager = activityManagerInstance();
                //name here
                activity_ = manager->activityNew(notifier_->name()); //use what name?
                activity_->nextTimeIs(manager->now());
                //here we need to set the time for activity
                activityNotifiee_ = new ActivityNotifiee(activity_.ptr(), this);
                activity_->lastNotifieeIs(activityNotifiee_.ptr());
            }
        }
    }
    void shipmentNew(){
        //name need to be refined
        
        string name = "shipment_" + notifier_->source() + "_" + notifier_->destination();
        Fwk::Ptr<Shipment> shipment = notifier_->shippingNetwork()->shipmentNew(name);
        shipment->sourceIs(notifier->source() );
        shipment->destinationIs(notifier->source() );
        shipment->loadIs(notifier->transferRate() );
        Activity::Manager::Ptr manager = activityManagerInstance();
        activity_->nextTimeIs(manager->now().value() + 24);
        manager->lastActivityIs(activity_.ptr());
    }
    CustomerReactor() : destSet(false), shipmentSizeSet(false), transferRateSet(false), started(false) {}
    Activity::Activity::Ptr activity_;
    Fwk::Ptr<ActivityNotifiee> activityNotifiee_;
    bool destSet, shipmentSizeSet, transferRateSet, started;
    
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
class ShipmentReactor : public Shipment::Notifiee {
  public:
     virtual void onDestination() {}
     virtual void onSource() {}
     virtual void onCurrentLocation() {}
     virtual void onDel(Shipment *p) {}

};
}

#endif
