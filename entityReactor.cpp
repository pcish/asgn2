#include "entityReactor.h"

Hour ShipmentReactor::retryTime = Hour(1.0);

void ShipmentReactor::forwardShipment() {
    Fwk::Ptr<Path> nextHop = notifier_->shippingNetwork()->nextHop(notifier_);
    if (nextHop == NULL) {
        notifier_->shippingNetwork()->dropShipment(notifier_);
        return;
    }
    WeakPtr<Segment> nextSegment = nextHop->segment(0);
    LOG_DEBUG("forwardShipment", nextSegment->name());
    if (nextSegment == NULL || nextHop->location(1) == NULL) {
        throw Fwk::InternalException("nextHop return invalid path");
    }
    LOG_DEBUG("forwardShipment", "nextLocation = " +
              nextHop->location(1)->name());
    LOG_DEBUG("forwardShipment", "capacity = " +
              STR(nextSegment->availableCapacity().value()));
    if (nextSegment->availableCapacity() > 0) {
        Fwk::Ptr<Fleet> fleet;
        if (nextSegment->transportationMode() == Segment::truck()) {
            fleet = notifier_->shippingNetwork()->truckFleet_;
        } else if (nextSegment->transportationMode() == Segment::plane()) {
            fleet = notifier_->shippingNetwork()->planeFleet_;
        } else if (nextSegment->transportationMode() == Segment::boat()) {
            fleet = notifier_->shippingNetwork()->boatFleet_;
        }
        if (fleet == NULL) {
            LOG_ERROR("forwardShipment", "cannot find fleet for seg " +
                      nextSegment->name() +
                      " tmode=" + STR(nextSegment->transportationMode()));
        }
        Hour transitTime = ceil(
            ceil((double) notifier_->load().value() /
            (double) fleet->capacity().value()) *
            (nextSegment->length().value() / fleet->speed().value())
        );
        LOG_DEBUG("forwardShipment", "transit time = ceil(ceil(" +
                  STR(notifier_->load().value()) + " / " +
                  STR(fleet->capacity().value()) + ") * (" +
                  STR(nextSegment->length().value()) + " / " +
                  STR(fleet->speed().value()) + ")) = " +
                  STR(transitTime.value()));

        Activity::Manager::Ptr manager = activityManagerInstance();
        if (activity_ == NULL) {
            try {
                activity_ = manager->activityNew(notifier_->name());
            } catch(Fwk::Exception& e) {
                LOG_INFO("forwardShipment", "attempting to new activity with name = " +
                         notifier_->name()+"returned: "+e.what());
            }
        }
        activityNotifiee_ = new ActivityNotifiee(
            activity_.ptr(), this, nextHop->location(1));
        activity_->nextTimeIs(manager->now().value() + transitTime.value());
        LOG_DEBUG("forwardShipment", "ETA: " +
                  STR(manager->now().value() + transitTime.value()));
        activity_->lastNotifieeIs(activityNotifiee_.ptr());
        manager->lastActivityIs(activity_.ptr());

        previousSegment_ = nextSegment.ptr();
        nextSegment->usedCapacityInc();
        nextSegment->shipmentsReceivedInc();
        notifier_->costInc(fleet->cost().value() *
                           nextSegment->difficulty().value() *
                           nextSegment->length().value());
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
}
