#include <queue>
#include <set>
#include <algorithm>

#include "Engine.h"
#include "entityReactor.h"
#include "routing/bfs.h"
#include "routing/random_walk.h"

namespace Shipping {

Ptr<Path> Path::clone() {
    Ptr<Path> newPath = Path::pathNew();

    newPath->cost_ = cost_;
    newPath->distance_ = distance_;
    newPath->expedite_ = expedite_;
    newPath->hour_ = hour_;
    newPath->location_ = location_;
    newPath->segment_ = segment_;
    return newPath;
}

void ShippingNetwork::computePath() {
    path_.clear();
    if (source_ == NULL) return;
    if (destination_) { // conn
        expedite_ = Segment::unavailable();
        explore (source_, set<string>(), Path::pathNew());
        expedite_ = Segment::available();
        explore (source_, set<string>(), Path::pathNew());
    }
    else { //explore
        explore (source_, set<string>(), Path::pathNew());
    }
}

unsigned int ShippingNetwork::paths() {
    if (isConnAttributeChange) {
        computePath();
        isConnAttributeChange = false;
    }
    return path_.size();
}

Ptr<Path> ShippingNetwork::path(unsigned int index) {
    if (isConnAttributeChange) {
        computePath();
        isConnAttributeChange = false;
    }
    return (index < path_.size() && index >= 0) ? path_[index] : NULL;
}

void ShippingNetwork::explore(const Ptr<Location> curLocation, set<string> visitedNodes, Ptr<Path> curPath) {
    curPath->locationIs(curLocation);
    if (!destination_ || (curLocation->name() == destination_->name())) {
        if (curPath->locations() > 1) {
            Ptr<Path> newPath = curPath->clone();
            newPath->expediteIs(expedite_);
            path_.push_back(newPath);
        }
    }
    if (destination_ && curLocation->name() == destination_->name() ) {
        if (curPath->locations() > 1) curPath->segmentIs(NULL); // pop the node because it already reached the destination
        curPath->locationIs(NULL); // pop the node because it already reached the destination
        return;
    }

    visitedNodes.insert(curLocation->name());

    for (unsigned int i = 0; i < curLocation->segments(); ++i) {
        WeakPtr<Segment> seg = curLocation->segment(i);
        if (!seg->returnSegment() ) {
//            cout << "Invalid return segment" << endl;
            //do we need to handle exception here?
            continue;
        }
        Ptr<Location> nextLocation = seg->returnSegment()->source();
        USD segCost;
        Hour segTime;

        if (seg->transportationMode() == Segment::truck()) {
            segCost = truckFleet_->cost().value();
            segTime = seg->length().value() / truckFleet_->speed().value();
        } else if (seg->transportationMode() == Segment::plane()) {
            segCost = planeFleet_->cost().value();
            segTime = seg->length().value() / planeFleet_->speed().value();
        } else if (seg->transportationMode() == Segment::boat()) {
            segCost = boatFleet_->cost().value();
            segTime = seg->length().value() / boatFleet_->speed().value();
        }
        segCost = segCost.value() * seg->length().value() * seg->difficulty().value();
        if (expedite_ == Segment::available()) {
            segCost = segCost.value() * 1.5;
            segTime = segTime.value() / 1.3;
        }
        bool visited = false;
        if (visitedNodes.find(nextLocation->name() ) != visitedNodes.end()) {
            visited = true;
        }
        if (!visited &&
            !(maxDistance_ > 0 && curPath->distance() + seg->length() > maxDistance_) &&
            !(maxCost_ > 0 && curPath->cost() + segCost > maxCost_) &&
            !(maxTime_.value() > 0 && curPath->hour().value() + segTime.value() > maxTime_.value()) &&
            !(expedite_ == Segment::available() && seg->expediteSupport() == Segment::unavailable())
           ) {
            curPath->segmentIs(seg);
            curPath->distanceIs(curPath->distance().value() + seg->length().value());
            curPath->costIs(curPath->cost().value() + segCost.value());
            curPath->hourIs(curPath->hour().value() + segTime.value());
            explore (nextLocation, visitedNodes, curPath);
            curPath->distanceIs(curPath->distance().value() - seg->length().value());
            curPath->costIs(curPath->cost().value() - segCost.value());
            curPath->hourIs(curPath->hour().value() - segTime.value());
        }
        else {
        }
    }
    if (curPath->locations() > 1) curPath->segmentIs(NULL);
    curPath->locationIs(NULL);
    visitedNodes.erase(curLocation->name());
}

Ptr<Shipment> ShippingNetwork::shipmentNew() {
    static long shipmentID = 0;
    stringstream name;
    name << "shipment_" << shipmentID;
    shipmentID++;
    Ptr<Shipment> m = new Shipment(name.str());
    m->shippingNetworkIs(this);
    Fwk::Ptr<ShipmentReactor> r = ShipmentReactor::shipmentReactorNew();
    r->notifierIs(m);
    shipment_.push_back(m);
    cerr << "shipment " << m->name() << "created" << endl;
    return m;
}

void ShippingNetwork::deliverShipment(WeakPtr<Shipment> shipment) {
}

Ptr<Path> ShippingNetwork::nextHop(const WeakPtr<Shipment> shipment) {
    Ptr<Bfs> bfs = Bfs::instance(this);
    bfs->nextHop(shipment);
    Ptr<RandomWalk> rw = RandomWalk::instance(this);
    return rw->nextHop(shipment);
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

void EngineManager::locationDel(Ptr<Location> o) {
    for (unsigned int i = 0; i < o->segments(); i++) {
        o->segment(i)->sourceIs(NULL);
    }
}

void EngineManager::customerDel(Ptr<Customer> o) {
    locationDel(o);
}

void EngineManager::terminalDel(Ptr<Terminal> o) {
    locationDel(o);
}

void EngineManager::portDel(Ptr<Port> o) {
    locationDel(o);
}

void EngineManager::segmentDel(Ptr<Segment> o) {
    if (o->returnSegment()) o->returnSegment()->returnSegmentIs(NULL);
    if (o->source()) {
        for (unsigned int i = 0; i < o->source()->segments(); i++) {
            if (o->source()->segment(i) == o) {
                o->source()->segmentIs(i, NULL);
                break;
            }
        }
    }
}

}
