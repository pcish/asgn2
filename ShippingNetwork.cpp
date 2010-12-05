#include <queue>
#include <set>
#include <algorithm>

#include "ShippingNetwork.h"
#include "entityReactor.h"
#include "routing/routing.h"
#include "Log.h"

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
    return m;
}

void ShippingNetwork::deliverShipment(WeakPtr<Shipment> shipment) {
    LOG_INFO("deliverShipment", "shipment " + shipment->name() + " received at destination");
    shipment->destination()->shipmentsReceivedInc();
    shipment->destination()->totalLatencyInc(shipment->transitTime());
    shipment->destination()->totalCostInc(shipment->cost());
    for (unsigned int i = 0; i < shipment_.size(); i++) {
        if (shipment_[i].ptr() == shipment.ptr()) {
            shipment_.erase(shipment_.begin() + i);
            break;
        }
    }
}

void ShippingNetwork::dropShipment(WeakPtr<Shipment> shipment) {
    LOG_INFO("dropShipment", "dropping shipment " + shipment->name());
    for (unsigned int i = 0; i < shipment_.size(); i++) {
        if (shipment_[i].ptr() == shipment.ptr()) {
            shipment_.erase(shipment_.begin() + i);
            break;
        }
    }

}

Ptr<Path> ShippingNetwork::nextHop(const WeakPtr<Shipment> shipment) {
    switch (routing_) {
        case bfs__:
            return Bfs::instance(this)->nextHop(shipment);
        break;
        case dijkstra__:
            return Dijkstra::instance(this)->nextHop(shipment);
        break;
        case randomwalk__:
            return RandomWalk::instance(this)->nextHop(shipment);
        break;
        default:
            return NULL;
    }
}

class ShippingNetworkReactor : public ShippingNetwork::Notifiee {
  public:
    virtual void onCustomerNew(Fwk::Ptr<Customer> p) {
        notifier_->statistics()->customersInc();
    }
    virtual void onTerminalNew(Fwk::Ptr<Terminal> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->statistics()->truckTerminals_++;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->statistics()->planeTerminals_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->statistics()->boatTerminals_++;
        }
    }
    virtual void onSegmentNew(Fwk::Ptr<Segment> p) {
        if (p->transportationMode() == Segment::truck()) {
            notifier_->statistics()->truckSegments_++;
        } else if (p->transportationMode() == Segment::plane()) {
            notifier_->statistics()->planeSegments_++;
        } else if (p->transportationMode() == Segment::boat()) {
            notifier_->statistics()->boatSegments_++;
        }
    }
    virtual void onPortNew(Fwk::Ptr<Port> p) {
        notifier_->statistics()->ports_++;
    }
    virtual void onPlaneFleetNew(Fwk::Ptr<PlaneFleet> p) {
        notifier_->planeFleetIs(p);
    }
    virtual void onTruckFleetNew(Fwk::Ptr<TruckFleet> p) {
        notifier_->truckFleetIs(p);
    }
    virtual void onBoatFleetNew(Fwk::Ptr<BoatFleet> p) {
        notifier_->boatFleetIs(p);
    }
    static Fwk::Ptr<ShippingNetworkReactor> ShippingNetworkReactorNew() {
        Fwk::Ptr<ShippingNetworkReactor> n = new ShippingNetworkReactor();
        return n;
    }
};

ShippingNetwork::Statistics::Statistics(Fwk::String name) :
    NamedInterface(name), customers_(0), ports_(0),
    truckTerminals_(0), planeTerminals_(0), boatTerminals_(0),
    truckSegments_(0), planeSegments_(0), boatSegments_(0),
    truckSegmentsExpediteAvailable_(0), planeSegmentsExpediteAvailable_(0),
    boatSegmentsExpediteAvailable_(0), expedite_(Segment::allAvailabilities()) {
}

ShippingNetwork::ShippingNetwork() : isConnAttributeChange(false), expedite_(Segment::allAvailabilities()), routing_(bfs__) {
    Fwk::Ptr<ShippingNetworkReactor> r = ShippingNetworkReactor::ShippingNetworkReactorNew();
    r->notifierIs(this);
    stringstream name;
    name << "shippingNetwork" << " " << "Statistics" << endl;
    statistics_ = new Statistics(name.str());
}

Ptr<Customer> ShippingNetwork::customerNew(const string name) {
    Ptr<Customer> m = new Customer(name);
    m->shippingNetworkIs(this);
    Fwk::Ptr<CustomerReactor> r = CustomerReactor::customerReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onCustomerNew(m);
    location_[m->name()] = m;
    return m;
}

Ptr<Terminal> ShippingNetwork::terminalNew(const string name, const Segment::TransportationMode transportationMode){
    Ptr<Terminal> m = new Terminal(name, transportationMode);
    m->shippingNetworkIs(this);
    Fwk::Ptr<TerminalReactor> r = TerminalReactor::terminalReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onTerminalNew(m);
    location_[m->name()] = m;
    return m;
}

Ptr<Segment> ShippingNetwork::segmentNew(const Segment::TransportationMode transportationMode, const string name){
    Ptr<Segment> m = new Segment(name, transportationMode);
    m->shippingNetworkIs(this);
    Fwk::Ptr<SegmentReactor> r = SegmentReactor::segmentReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onSegmentNew(m);
    segment_[m->name()] = m;
    return m;
}

Ptr<Port> ShippingNetwork::portNew(const string name){
    Ptr<Port> m = new Port(name);
    m->shippingNetworkIs(this);
    Fwk::Ptr<PortReactor> r = PortReactor::portReactorNew();
    r->notifierIs(m);
    if (notifiee_) notifiee_->onPortNew(m);
    location_[m->name()] = m;
    return m;
}

Ptr<PlaneFleet> ShippingNetwork::planeFleetNew(const string name) {
    Ptr<PlaneFleet> m = new PlaneFleet(name);
    if (notifiee_) notifiee_->onPlaneFleetNew(m);
    return m;
}

Ptr<TruckFleet> ShippingNetwork::truckFleetNew(const string name) {
    Ptr<TruckFleet> m = new TruckFleet(name);
    if (notifiee_) notifiee_->onTruckFleetNew(m);
    return m;
}

Ptr<BoatFleet> ShippingNetwork::boatFleetNew(const string name) {
    Ptr<BoatFleet> m = new BoatFleet(name);
    if (notifiee_) notifiee_->onBoatFleetNew(m);
    return m;
}

void ShippingNetwork::locationDel(Ptr<Location> o) {
    for (unsigned int i = 0; i < o->segments(); i++) {
        o->segment(i)->sourceIs(NULL);
    }
    map<string,Ptr<Location> >::iterator t = location_.find(o->name());
    if (t != location_.end()) {
        location_.erase(t);
    }
}

void ShippingNetwork::customerDel(Ptr<Customer> o) {
    locationDel(o);
}

void ShippingNetwork::terminalDel(Ptr<Terminal> o) {
    locationDel(o);
}

void ShippingNetwork::portDel(Ptr<Port> o) {
    locationDel(o);
}

void ShippingNetwork::segmentDel(Ptr<Segment> o) {
    if (o->returnSegment()) o->returnSegment()->returnSegmentIs(NULL);
    if (o->source()) {
        for (unsigned int i = 0; i < o->source()->segments(); i++) {
            if (o->source()->segment(i) == o) {
                o->source()->segmentIs(i, NULL);
                break;
            }
        }
    }
    map<string,Ptr<Segment> >::iterator t = segment_.find(o->name());
    if (t != segment_.end()) {
        segment_.erase(t);
    }
}

}
