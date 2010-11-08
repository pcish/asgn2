#include <queue>
#include <list>

#include "Engine.h"
#include "entityReactor.h"

namespace Shipping {
Ptr<Path> Path::clone() {
//    Ptr<Path> newPath (const_cast<Ptr<Path> >(this) );
    Ptr<Path> newPath = Path::pathNew();
    
    newPath->cost_ = cost_;
    newPath->distance_ = distance_;
    newPath->expedite_ = expedite_;
    newPath->hour_ = hour_;
    newPath->location_ = location_;
    newPath->segment_ = segment_;
    
    return newPath;
}
Ptr<Path> ShippingNetwork::path(unsigned int index) {
    if (isConnAttributeChange) {
        if (source_ == NULL) return NULL;
        explore (source_, list<Ptr<Location> >(), Path::pathNew() );
    }
    return (index < path_.size() && index >= 0)?path_[index]:NULL;
}
Ptr<Path> ShippingNetwork::explore(const Ptr<Location> curLocation, list<Ptr<Location> > visitedNotes, Ptr<Path> curPath) {
    list<Ptr<Location> > visitedNodes;
    USD curCost;
    Mile curDistance;
    Hour curTime;
    bool isLast = true;

    visitedNodes.push_back(curLocation);
    for (unsigned int i = 0; i < curLocation->segments(); ++i) {
        Ptr<Segment> seg = curLocation->segment(i);
        Ptr<Location> nextLocation = seg->returnSegment()->source();
        USD segCost;
        Hour segTime;
        //Computes time and cost here
        if (seg->transportationMode() == Segment::truck() ) {
            segCost = truckFleet->cost().value() * seg->length().value();
            segTime = seg->length().value() / truckFleet->speed().value();
        } else if (seg->transportationMode() == Segment::plane() ) {
            segCost = planeFleet->cost().value() * seg->length().value();
            segTime = seg->length().value() / planeFleet->speed().value();
        } else if (seg->transportationMode() == Segment::boat() ) {
            segCost = boatFleet->cost().value() * seg->length().value();
            segTime = seg->length().value() / boatFleet->speed().value();
        }
        bool visited = false;
        /* implement search algorithm (should be using like hash map or set)*/
        for (list<Ptr<Location> >::iterator scannerPtr = visitedNodes.begin(); scannerPtr != visitedNodes.end(); scannerPtr ++) {
            if ((*scannerPtr).ptr() == nextLocation.ptr() ) {
                visited = true;
            }
        }
        if ( !visited && 
                !(maxDistance_ > 0 && curPath->distance() + seg->length() > maxDistance_) &&
                !(maxCost_ > 0 && curPath->cost() + segCost > maxCost_) &&
                !(maxTime_ > 0 && curPath->hour() + segTime > maxTime_) &&
                !(expedite_ == Segment::available() && seg->expediteSupport() == Segment::unavailable() ) ) {
            isLast = false;
            visitedNotes.push_back(nextLocation ); // get its return segment's source
            curPath->locationIs(nextLocation);
            curPath->segmentIs(seg);
            explore (nextLocation, visitedNotes, curPath);
        }
    }
    if (isLast) {
        Ptr<Path> newPath = curPath->clone();
        path_.push_back(newPath);
    }
    curPath->locationIs(NULL);
    curPath->segmentIs(NULL);
}
class EngineReactor : public EngineManager::Notifiee {
    public:
        virtual void onCustomerNew(Fwk::Ptr<Customer> p) {
            notifier_->shippingNetwork()->customersInc();
        }
        virtual void onTerminalNew(Fwk::Ptr<Terminal> p) {
            if (p->transportationMode() == Segment::truck()) {
                notifier_->shippingNetwork()->truckTerminals_++;
            } else if (p->transportationMode() == Segment::plane()) {
                notifier_->shippingNetwork()->planeTerminals_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatTerminals_++;
        }
    }
    virtual void onSegmentNew(Fwk::Ptr<Segment> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->shippingNetwork()->truckSegments_++;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->shippingNetwork()->planeSegments_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->shippingNetwork()->boatSegments_++;
        }
    }
    virtual void onPortNew(Fwk::Ptr<Port> p) {
        notifier_->shippingNetwork()->ports_++;
    }
    static Fwk::Ptr<EngineReactor> engineReactorNew() {
        Fwk::Ptr<EngineReactor> n = new EngineReactor();
        return n;
    }
};

EngineManager::EngineManager() {
    network_ = new ShippingNetwork();
    Fwk::Ptr<EngineReactor> r = EngineReactor::engineReactorNew();
    r->notifierIs(this);
}

Ptr<Customer> EngineManager::customerNew(const string name) {
    Ptr<Customer> m = new Customer(name);
    m->shippingNetworkIs(network_.ptr());
    Fwk::Ptr<CustomerReactor> r = CustomerReactor::customerReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onCustomerNew(m);
    return m;
}

Ptr<Terminal> EngineManager::terminalNew(const string name, const Segment::TransportationMode transportationMode){
    Ptr<Terminal> m = new Terminal(name, transportationMode);
    m->shippingNetworkIs(network_.ptr());
    Fwk::Ptr<TerminalReactor> r = TerminalReactor::terminalReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onTerminalNew(m);
    return m;
}

Ptr<Segment> EngineManager::segmentNew(const Segment::TransportationMode transportationMode, const string name){
    Ptr<Segment> m = new Segment(name, transportationMode);
    m->shippingNetworkIs(network_.ptr());
    Fwk::Ptr<SegmentReactor> r = SegmentReactor::segmentReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onSegmentNew(m);
    return m;
}

Ptr<Port> EngineManager::portNew(const string name){
    Ptr<Port> m = new Port(name);
    m->shippingNetworkIs(network_.ptr());
    Fwk::Ptr<PortReactor> r = PortReactor::portReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onPortNew(m);
    return m;
}

}
