
#include <string>

#include "entities.h"
#include "Engine.h"

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
/*void Location::segmentIs (unsigned int index, Ptr<Segment> seg) {
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
*/
Ptr<Segment> Location::segment(const unsigned int index) const {
    if (index-1 < 0 || index-1 >= segments_.size () ){
         cerr << "Segment # out of bound exception" << endl;
         return NULL;
    }
    return segments_[index-1];
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
class SegmentReactor : public Segment::Notifiee {
  public:
    virtual void onDel(Fwk::Ptr<Segment> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckSegments_--;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeSegments_--;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatSegments_--;
        }
    }
    static Fwk::Ptr<SegmentReactor> segmentReactorNew() {
        Fwk::Ptr<SegmentReactor> n = new SegmentReactor();
        return n;
    }
};
class CustomerReactor : public Customer::Notifiee {
  public:
    virtual void onDel(Fwk::Ptr<Customer> p) {
        notifier_->shippingNetwork()->customers_--;
    }
    static Fwk::Ptr<CustomerReactor> CustomerReactorNew() {
        Fwk::Ptr<CustomerReactor> n = new CustomerReactor();
        return n;
    }
};
class PortReactor : public Port::Notifiee {
  public:
    virtual void onDel(Fwk::Ptr<Port> p) {
        notifier_->shippingNetwork()->ports_--;
    }
    static Fwk::Ptr<PortReactor> PortReactorNew() {
        Fwk::Ptr<PortReactor> n = new PortReactor();
        return n;
    }
};
class TerminalReactor : public Terminal::Notifiee {
  public:
    virtual void onDel(Fwk::Ptr<Terminal> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckTerminals_--;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeTerminals_--;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatTerminals_--;
        }
    }
    static Fwk::Ptr<TerminalReactor> TerminalReactorNew() {
        Fwk::Ptr<TerminalReactor> n = new TerminalReactor();
        return n;
    }
};
} /* end namespace */

