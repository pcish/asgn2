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
unsigned int ShippingNetwork::paths() {
    if (isConnAttributeChange) {
        if (source_ == NULL) return 0;
        list<Ptr<Location> > visitedNodes;
        explore (source_, visitedNodes, Path::pathNew() );
        isConnAttributeChange = false;
    }
    return path_.size();
}
Ptr<Path> ShippingNetwork::path(unsigned int index) {
    if (isConnAttributeChange) {
        if (source_ == NULL) return NULL;
        list<Ptr<Location> > visitedNodes;
        explore (source_, visitedNodes, Path::pathNew() );
        isConnAttributeChange = false;
    }
    return (index < path_.size() && index >= 0)?path_[index]:NULL;
}
void ShippingNetwork::explore(const Ptr<Location> curLocation, list<Ptr<Location> > visitedNodes, Ptr<Path> curPath) {
    USD curCost;
    Mile curDistance;
    Hour curTime;
    bool isLast = true;

//    cerr << "Now at location: " << curLocation->name() << endl;
    visitedNodes.push_back(curLocation);
    for (unsigned int i = 0; i < curLocation->segments(); ++i) {
        Ptr<Segment> seg = curLocation->segment(i);
        Ptr<Location> nextLocation = seg->returnSegment()->source();
        USD segCost;
        Hour segTime;
        //Computes time and cost here
        if (seg->transportationMode() == Segment::truck() ) {
            segCost = truckFleet_->cost().value() * seg->length().value();
            segTime = seg->length().value() / truckFleet_->speed().value();
        } else if (seg->transportationMode() == Segment::plane() ) {
            segCost = planeFleet_->cost().value() * seg->length().value();
            segTime = seg->length().value() / planeFleet_->speed().value();
        } else if (seg->transportationMode() == Segment::boat() ) {
            segCost = boatFleet_->cost().value() * seg->length().value();
            segTime = seg->length().value() / boatFleet_->speed().value();
        }
        if (expedite_ == Segment::available() ) {
            segCost = segCost.value() * 1.5;
            segTime = segTime.value() / 1.3;
        }
        //cerr << "Try Segment: " << seg->name() << " Destination: " << seg->returnSegment()->name() << "...";
        bool visited = false;
        /* implement search algorithm (should be using like hash map or set)*/
        for (list<Ptr<Location> >::iterator scannerPtr = visitedNodes.begin(); scannerPtr != visitedNodes.end(); scannerPtr ++) {
        //    cerr << "Comparing " << (*scannerPtr)->name() << " and " << nextLocation->name() << "...";
            if ((*scannerPtr).ptr() == nextLocation.ptr() ) {
                visited = true;
            }
        }
        if ( !visited && 
             !(maxDistance_ > 0 && curPath->distance() + seg->length() > maxDistance_) &&
             !(maxCost_ > 0 && curPath->cost() + segCost > maxCost_) &&
             !(maxTime_ > 0 && curPath->hour() + segTime > maxTime_) &&
             !(expedite_ == Segment::available() && seg->expediteSupport() == Segment::unavailable() ) ) {
        //    cerr << "successull" << endl;
            isLast = false;
            //visitedNotes.push_back(nextLocation ); // get its return segment's source
            curPath->locationIs(nextLocation);
            curPath->segmentIs(seg);
            explore (nextLocation, visitedNodes, curPath);
        }
        else {
        //    cerr << "fail because of insatisfaction of the condition" << endl;
        }
    }
    if (isLast) {
        Ptr<Path> newPath = curPath->clone();
        path_.push_back(newPath);
    }
    cerr << curPath->locations();
    curPath->locationIs(NULL);
    curPath->segmentIs(NULL);
    visitedNodes.pop_back();
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
    virtual void onPlaneFleetNew(Fwk::Ptr<PlaneFleet> p) {
        notifier_->shippingNetwork()->planeFleetIs(p);
    }
    virtual void onTruckFleetNew(Fwk::Ptr<TruckFleet> p) {
        notifier_->shippingNetwork()->truckFleetIs(p);
    }
    virtual void onBoatFleetNew(Fwk::Ptr<BoatFleet> p) {
        notifier_->shippingNetwork()->boatFleetIs(p);
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
