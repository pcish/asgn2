#include <queue>

#include "Engine.h"
#include "entityReactor.h"

namespace Shipping {

Ptr<Path> ShippingNetwork::path() const {
    if (!source_)
        return NULL;
    if (!destination_) {
        return explore();
    } else {
        return conn();
    }
}
Ptr<Path> ShippingNetwork::explore() const {
    Ptr<Path> path_;
    queue<Ptr<Location> > bfsQueue;
    Ptr<Location> curLocation;
    USD curCost;
    Mile curDistance;
    Hour curTime;

    bfsQueue.push(source_);    
    while (!bfsQueue.empty() ) {
        curLocation = bfsQueue.front();
        bfsQueue.pop();
        for (SegmentCount i = 0; i < curLocation->segments().value(); ++i) {
            Ptr<Segment> seg = curLocation->segment(i);
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
            if ( !(maxDistance_ > 0 && curDistance + seg->length() > maxDistance_) && 
                 !(maxCost_ > 0 && curCost + segCost > maxCost_) && 
                 !(maxTime_ > 0 && curTime + segTime > maxTime_) &&                 
                 !(expedite_ == Segment::available() && seg->expediteSupport() == Segment::unavailable() ) ) {
                //push location and segment into path
                bfsQueue.push(seg->returnSegment()->source() ); // get its return segment's source                
            }
            else { //reaches the limit
            }
        }
    }
}
Ptr<Path> ShippingNetwork::conn() const {
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
