
#include <string>

#include "entities.h"

namespace Shipping {

void Location::segmentIs(const WeakPtr<Segment> seg) {
    if (seg == NULL) {
        cerr << "Can't add NULL segment" << endl;
        return;
    }
    segments_.push_back(seg.ptr());
    if (notifiee_) notifiee_->onSegment();
}

void Location::segmentIs(const unsigned int index, WeakPtr<Segment> seg) {
    if (index < 0 || index >= segments_.size() ) {
        cerr <<  "Index out of bound exception" << endl;
        return;
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
         cerr << "Segment # out of bound exception" << endl;
         return NULL;
    }
    return segments_[index];
}

void Terminal::segmentIs(const WeakPtr<Segment> seg) {
    if (seg == NULL) {
        cerr << "Can't add NULL segment" << endl;
        return;
    }
    else if (seg->transportationMode() != transportationMode_) {
        cerr << "Terminal must connect two segments with the same transportation mode" << endl;
        return;
    }
    Location::segmentIs(seg);
}

void Terminal::segmentIs(const unsigned int index, WeakPtr<Segment> seg) {
    if (index < 0 || index >= segments()) {
        cerr <<  "Index out of bound exception" << endl;
        return;
    }
    if (seg == NULL) {
        Location::segmentIs(index, seg);
    }
    else {
        if (seg->transportationMode() != transportationMode_) {
            cerr << "Terminal must connect two segments with the same transportation mode" << endl;
            return;
        }
        Location::segmentIs(index, seg);
    }
}

void Segment::returnSegmentIs(const Ptr<Segment> returnSegment) {
    if (returnSegment_ == returnSegment) return;
    if (returnSegment->transportationMode() != transportationMode_) return;
    returnSegment_ = returnSegment;
    if (notifiee_) notifiee_->onReturnSegment();
}
/*
Ptr<Fleet> Fleet::instance() {
    if (Fleet::instance_ == NULL)
        instance_ = new Fleet ();
    return instance_;

}
Fleet::Fleet () {
    speed_.insert (make_pair(Segment::truck(), Mile(0) ) );
    cost_.insert (make_pair(Segment::truck(), USD(0) ) );
    capacity_.insert (make_pair(Segment::truck(), PackageUnit(0) ) );
    speed_.insert (make_pair(Segment::boat(), Mile(0) ) );
    cost_.insert (make_pair(Segment::boat(), USD(0) ) );
    capacity_.insert (make_pair(Segment::boat(), PackageUnit(0) ) );
    speed_.insert (make_pair(Segment::plane(), Mile(0) ) );
    cost_.insert (make_pair(Segment::plane(), USD(0) ) );
    capacity_.insert (make_pair(Segment::plane(), PackageUnit(0) ) );
}
Ptr<Fleet> Fleet::instance_ = NULL;*/

} /* end namespace */

