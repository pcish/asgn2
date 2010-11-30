
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
    if (index < 0 || index >= segments_.size()) {
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

void Segment::sourceIs(const Ptr<Location> source) { 
    if (source_ == source) return; 
    if (source->locationType() == Location::terminal()) {
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

} /* end namespace */

