
#include <string>

#include "entities.h"

namespace Shipping {

void Location::segmentIs(const Ptr<Segment> seg) {
    if (seg == NULL) {
        cerr << "Can't add NULL segment" << endl;
        return;
    }
    segments_.push_back (seg);
    if (notifiee_)
        notifiee_->onSegment ();
}
void Location::segmentIs (int index, Ptr<Segment> seg) {
    if (index < 0 || index >= segments_.size() ) {
        cerr <<  "Index out of bound exception" << endl;
        return;
    }
    if (seg == NULL) { //delete segment

    }
    else { //put segment at that position

    }
}
//unsigned int Location::segments () { return segments_.size(); }

Ptr<Segment> Location::segment(const unsigned int index) const {
    if (index < 0 || index >= segments_.size () ){
         cerr << "Segment # out of bound exception" << endl;
         return NULL;
    }
    return segments_[index];
}
/*
void Terminal::segmentIs (Ptr<Segment> seg) {
    if (seg == NULL) {// empty
        cerr << "Can't add NULL segment" << endl;
        return;
    }
    else if (seg->transportationMode() != transMode_ ) {
        cerr << "Terminal must connect two segments with the same transportation mode" << endl;
        return;
    }
    Location::segmentIs (seg);
}
/*void Terminal::segmentIs (unsigned int index, Ptr<Segment> seg) {
    if (seg != NULL && seg->transportationMode() != transMode_ ) {// insert node rather than delete
        cerr << "Terminal must connect two segments with the same transportation mode" << endl;
        return;
    }
    Location::segmentIs (index, seg);
}

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

