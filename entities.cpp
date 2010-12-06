
#include <string>

#include "entities.h"
#include "entityReactor.h"
#include "ActivityImpl.h"

namespace Shipping {

void Location::segmentIs(const WeakPtr<Segment> seg) {
    if (seg == NULL) {
        throw Fwk::RangeException ("Can't add NULL segment: "+seg->name() );
//        cerr << "Can't add NULL segment" << endl;
        //return;
    }
    segments_.push_back(seg.ptr());
    if (notifiee_) notifiee_->onSegment();
}

void Location::segmentIs(const unsigned int index, WeakPtr<Segment> seg) {
    if (index < 0 || index >= segments_.size()) {
        throw Fwk::RangeException ("Index out of bound: " + index);
//       cerr <<  "Index out of bound exception" << endl;
        //return;
    }
    if (seg == NULL) {
        segments_.erase(segments_.begin() + index);
    }
    else {
        segments_.insert(segments_.begin() + index, seg.ptr());
    }
}

WeakPtr<Segment> Location::segment(const unsigned int index) const {
    if (index < 0 || index >= segments_.size()) {
        //accessor should be exception-free
//        throw Fwk::RangeException
//         cerr << "Segment # out of bound exception" << endl;

         return NULL;
    }
    return segments_[index];
}

void Terminal::segmentIs(const WeakPtr<Segment> seg) {
    if (seg == NULL) {
        throw Fwk::RangeException ("Can't add NULL segment: "+seg->name() );
        //cerr << "Can't add NULL segment" << endl;
        //return;
    }
    else if (seg->transportationMode() != transportationMode_) {
        throw Fwk::RangeException ("Terminal must connect two segments with the same transportation mode");
        //cerr << "Terminal must connect two segments with the same transportation mode" << endl;
        //return;
    }
    Location::segmentIs(seg);
}

void Terminal::segmentIs(const unsigned int index, WeakPtr<Segment> seg) {
    if (index < 0 || index >= segments()) {
        throw Fwk::RangeException ("Index out of bound: " + index);
        //cerr <<  "Index out of bound exception" << endl;
        //return;
    }
    if (seg == NULL) {
        Location::segmentIs(index, seg);
    }
    else {
        if (seg->transportationMode() != transportationMode_) {
            throw Fwk::RangeException ("Terminal must connect two segments with the same transportation mode");
            //cerr << "Terminal must connect two segments with the same transportation mode" << endl;
            //return;
        }
        Location::segmentIs(index, seg);
    }
}

void Segment::sourceIs(const Ptr<Location> source) {
    if (source_ == source) return;
    if (source != NULL && source->locationType() == Location::terminal()) {
        Location * src = const_cast<Location*>(source.ptr());
        Terminal * term = (Terminal*) src;
        if (term->transportationMode() != transportationMode_) {
          return;
        }
    }
    source_ = source;
    if (notifiee_) notifiee_->onSource();
}

void Segment::returnSegmentIs(const Ptr<Segment> returnSegment) {
    if (returnSegment_ == returnSegment) return;
    if (returnSegment != NULL && returnSegment->transportationMode() != transportationMode_) return;
    returnSegment_ = returnSegment;
    if (notifiee_) notifiee_->onReturnSegment();
}

Fleet::Fleet(Fwk::String name) : NamedInterface(name), cost_(1), speed_(1), capacity_(100) {
    for (unsigned int i = 0; i < 24; i++) {
        scheduledCosts_[i].valid = false;
        scheduledSpeeds_[i].valid = false;
        scheduledCapacitys_[i].valid = false;
    }
    now_ = 0;
}

USD Fleet::cost() const {
    if (scheduledCosts_[((int) now_.value()) % 24].valid) {
        return scheduledCosts_[((int) now_.value()) % 24].value;
    } else {
        return cost_;
    }
}

USD Fleet::scheduledCost(const unsigned int index) const { return scheduledCosts_[index].value; }

void Fleet::scheduledCostIs(const unsigned int index, USD cost) {
    scheduledCosts_[index].valid = true;
    scheduledCosts_[index].value = cost;
}

PackageUnit Fleet::capacity() const {
    if (scheduledCapacitys_[((int) now_.value()) % 24].valid) {
        return scheduledCapacitys_[((int) now_.value()) % 24].value;
    } else {
        return capacity_;
    }
}

PackageUnit Fleet::scheduledCapacity(const unsigned int index) const { return scheduledCapacitys_[index].value; }

void Fleet::scheduledCapacityIs(const unsigned int index, PackageUnit capacity) {
    scheduledCapacitys_[index].valid = true;
    scheduledCapacitys_[index].value = capacity;
}

Mile Fleet::speed() const {
    if (scheduledSpeeds_[((int) now_.value()) % 24].valid) {
        return scheduledSpeeds_[((int) now_.value()) % 24].value;
    } else {
        return speed_;
    }
}

Mile Fleet::scheduledSpeed(const unsigned int index) const { return scheduledSpeeds_[index].value; }

void Fleet::scheduledSpeedIs(const unsigned int index, Mile speed) {
    scheduledSpeeds_[index].valid = true;
    scheduledSpeeds_[index].value = speed;
}

Clock::Clock() : NamedInterface("clock") {
    Activity::Manager::Ptr manager = activityManagerInstance();
    if(heartbeatActivity_ == NULL) heartbeatActivity_ = manager->activityNew("ShippingNetwork.onHeartbeatActivity");
    heartbeatActivity_->nextTimeIs(manager->now().value() + 1);
    heartbeatActivity_->lastNotifieeIs(new HeartbeatActivityNotifiee(heartbeatActivity_.ptr(), this));
    manager->lastActivityIs(heartbeatActivity_.ptr());
}

} /* end namespace */

