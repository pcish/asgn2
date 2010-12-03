#ifndef ENTITY_REACTOR_H
#define ENTITY_REACTOR_H

#include "entities.h"
#include "ShippingNetwork.h"
#include "ActivityImpl.h"

namespace Shipping {
class ShippingNetwork;

class ShipmentReactor : public Shipment::Notifiee {
  public:
    static Hour retryTime;
    virtual void onCurrentLocation() {
        cerr << "on current location called" << endl;
        if (notifier_->currentLocation() == notifier_->destination()) {
            notifier_->shippingNetwork()->deliverShipment(notifier_);
        } else {
            forwardShipment();
        }
        cerr << "on current location end" << endl;
    }
    static Fwk::Ptr<ShipmentReactor> shipmentReactorNew() {
        Fwk::Ptr<ShipmentReactor> n = new ShipmentReactor();
        return n;
    }
    void changeLocation(Fwk::Ptr<Location> nextLocation) {
        if (notifier_->currentLocation() == nextLocation) forwardShipment();
        else {
            previousSegment_->usedCapacityDec();
            notifier_->currentLocationIs(nextLocation);
        }
    }

  private:
    ShipmentReactor() { previousSegment_ = NULL; }
    void forwardShipment() {
        cerr << "forward shipment called" << endl;
        Fwk::Ptr<Path> nextHop = notifier_->shippingNetwork()->nextHop(notifier_);
        WeakPtr<Segment> nextSegment = nextHop->segment(0);
        cerr << nextSegment->name() << endl;
        if (nextSegment == NULL || nextHop->location(1) == NULL) {
            throw Fwk::InternalException("nextHop return invalid path");
        }
        cerr << "capacity = " << nextSegment->availableCapacity().value() << endl;
        cerr << nextHop->location(1)->name() << endl;
        if (nextSegment->availableCapacity() > 0) {
            Fwk::Ptr<Fleet> fleet;
            if (nextSegment->transportationMode() == Segment::truck()) {
                fleet = notifier_->shippingNetwork()->truckFleet_;
            } else if (nextSegment->transportationMode() == Segment::plane()) {
                fleet = notifier_->shippingNetwork()->planeFleet_;
            } else if (nextSegment->transportationMode() == Segment::boat()) {
                fleet = notifier_->shippingNetwork()->boatFleet_;
            }
            Hour transitTime =
                ceil((double) notifier_->load().value() / (double) fleet->capacity().value()) *
                (nextSegment->length().value() / fleet->speed().value());
            //cerr << "ceil(" << notifier_->load().value() << " / " << fleet->capacity().value() << ")" << endl;
            //cerr << "* ("<< nextSegment->length().value() << " / " << fleet->speed().value() << ")" << endl;
            cerr << "transit time = " << transitTime.value() << endl;

            Activity::Manager::Ptr manager = activityManagerInstance();
            if (activity_ == NULL) {
                try {
                    activity_ = manager->activityNew(notifier_->name());
                } catch(Fwk::Exception& e) {
                    cerr << "attempting to new activity with name=" << notifier_->name() << "returned: ";
                    cout << e.what() << endl;
                }
            }
            activityNotifiee_ = new ActivityNotifiee(
                activity_.ptr(), this, nextHop->location(1));
            activity_->nextTimeIs(manager->now().value() + transitTime.value());
            cerr << "ETA: " << manager->now().value() + transitTime.value() << endl;
            activity_->lastNotifieeIs(activityNotifiee_.ptr());
            manager->lastActivityIs(activity_.ptr());

            previousSegment_ = nextSegment.ptr();
            nextSegment->usedCapacityInc();
            nextSegment->shipmentsReceivedInc();
            notifier_->costInc(fleet->cost().value() * nextSegment->difficulty().value() * nextSegment->length().value());
            notifier_->transitTimeInc(transitTime);
        } else {
            nextSegment->shipmentsRefusedInc();
            notifier_->transitTimeInc(retryTime);
            Activity::Manager::Ptr manager = activityManagerInstance();
            if(activity_ == NULL) activity_ = manager->activityNew(notifier_->name());
            activityNotifiee_ = new ActivityNotifiee(
                activity_.ptr(), this, notifier_->currentLocation());
            activity_->nextTimeIs(manager->now().value() + retryTime.value());
            activity_->lastNotifieeIs(activityNotifiee_.ptr());
            manager->lastActivityIs(activity_.ptr());
        }
        cerr << "forward shipment return" << endl;
    }
    class ActivityNotifiee : public Activity::Activity::Notifiee {
      public:
        ActivityNotifiee(Activity::Activity *activity, ShipmentReactor *_parent, Fwk::Ptr<Location> nextLocation) :
            Activity::Activity::Notifiee(activity), activity_(activity),
            parent_(_parent), nextLocation_(nextLocation) {}
        virtual void onStatus() {
            Activity::Activity::Status status = activity_->status();
            if (status == Activity::Activity::executing) {
                parent_->changeLocation(nextLocation_);
            }
        }
        virtual void onNextTime() {}
      private:
        Activity::Activity *activity_;
        ShipmentReactor *parent_;
        Fwk::Ptr<Location> nextLocation_;
    };
    Activity::Activity::Ptr activity_;
    Fwk::Ptr<ActivityNotifiee> activityNotifiee_;
    Fwk::Ptr<Segment> previousSegment_;
};
Hour ShipmentReactor::retryTime = Hour(1.0);

class SegmentReactor : public Segment::Notifiee {
  public:
    virtual void onDel(Segment *p) {
        int delta = 0;
        if (notifier_->expediteSupport() == Segment::available()) {
            delta = 1;
        }
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->statistics()->truckSegments_--;
            notifier_->shippingNetwork()->statistics()->truckSegmentsExpediteAvailable_ -= delta;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->statistics()->planeSegments_--;
            notifier_->shippingNetwork()->statistics()->planeSegmentsExpediteAvailable_ -= delta;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->statistics()->boatSegments_--;
            notifier_->shippingNetwork()->statistics()->boatSegmentsExpediteAvailable_ -= delta;
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
            notifier_->shippingNetwork()->statistics()->truckSegmentsExpediteAvailable_ += delta;
        } else if (notifier_->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->statistics()->planeSegmentsExpediteAvailable_ += delta;
        } else if (notifier_->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->statistics()->boatSegmentsExpediteAvailable_ += delta;
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

class CustomerReactor : public Customer::Notifiee {
  public:
    virtual void onDel(Customer *p) {
        notifier_->shippingNetwork()->statistics()->customers_--;
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
                manager->lastActivityIs(activity_.ptr());
            }
        }
    }
    void shipmentNew() {
        Fwk::Ptr<Shipment> shipment = notifier_->shippingNetwork()->shipmentNew();
        shipment->sourceIs(notifier_.ptr());
        shipment->destinationIs(dynamic_cast<Customer*>(notifier_->destination().ptr()));
        shipment->loadIs(notifier_->shipmentSize());
        shipment->currentLocationIs(notifier_.ptr());

        Activity::Manager::Ptr manager = activityManagerInstance();
        activity_->nextTimeIs(manager->now().value() + 24);
        manager->lastActivityIs(activity_.ptr());
        cerr << "shipment at " << notifier_->name() << " started" << endl;
    }
    CustomerReactor() : destSet(false), shipmentSizeSet(false), transferRateSet(false), started(false) {}
    Activity::Activity::Ptr activity_;
    Fwk::Ptr<ActivityNotifiee> activityNotifiee_;
    bool destSet, shipmentSizeSet, transferRateSet, started;
};

class PortReactor : public Port::Notifiee {
  public:
    virtual void onDel(Port *p) {
        notifier_->shippingNetwork()->statistics()->ports_--;
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
            notifier_->shippingNetwork()->statistics()->truckTerminals_--;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->statistics()->planeTerminals_--;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->statistics()->boatTerminals_--;
        }
    }
    static Fwk::Ptr<TerminalReactor> terminalReactorNew() {
        Fwk::Ptr<TerminalReactor> n = new TerminalReactor();
        return n;
    }
};

}

#endif
