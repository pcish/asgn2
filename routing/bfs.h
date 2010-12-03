#ifndef ROUTING_BFS_H
#define ROUTING_BFS_H

#include <set>
#include <queue>
#include "routing.h"

class Bfs : public RoutingAlgorithm<Bfs> {
  public:
    virtual Ptr<Path> nextHop(const WeakPtr<Shipment> shipment) {
        Ptr<Location> from = shipment->currentLocation();
        Ptr<Location> to = shipment->destination();
        Ptr<Path> path = Path::pathNew();
        set<string> visited;
        queue<Ptr<Location> > loc;
        loc.push(from);

        path->locationIs(from);
        while (!loc.empty()) {
            Ptr<Location> curLocation = loc.front();
            loc.pop();
            visited.insert(curLocation->name() );
            for (unsigned int i = 0; i < curLocation->segments(); ++i) {
                WeakPtr<Segment> seg = curLocation->segment(i);
                WeakPtr<Segment> returnSeg = seg->returnSegment();
                if (returnSeg) {
                    Ptr<Location> nextLocation = returnSeg->source();
                    if (visited.find(nextLocation->name()) == visited.end() ){
                        visited.insert(nextLocation->name());
                        path->locationIs(nextLocation);
                        path->segmentIs(seg.ptr());
                        if (nextLocation->name() == to->name() ){ //found path, return
                            return path;
                        }
                        else {
                            loc.push(nextLocation);
                        }
                    }
                }
                else {
                    //ignore invalid return segment
                }
            }
        }
        return NULL;
    }
};
class Dijkstra : public RoutingAlgorithm<Dijkstra> {
  public:
    virtual Ptr<Path> nextHop(const WeakPtr<Shipment> shipment) {
        Ptr<Location> from = shipment->currentLocation();
        Ptr<Location> to = shipment->destination();
        list<Ptr<DistanceTuple<Mile> > > distanceTuple; //used to store which nodes are waiting to be used in the next round of optimization
        map<string, Ptr<DistanceTuple<Mile> > > lookup;

        Ptr<DistanceTuple<Mile> > t = new DistanceTuple<Mile>(from, NULL, 0);
        distanceTuple.push_back(t);
        lookup[from->name()] = t;
        while (!distanceTuple.empty() ) {
            min_element(distanceTuple.begin(), distanceTuple.end(), DistanceTuple<Mile>::DistanceTupleComp());
            Ptr<DistanceTuple<Mile> > best = distanceTuple.front();
            distanceTuple.pop_front();
            Ptr<Location> curLocation = best->location();
            if (curLocation->name() == to->name()) {
                Ptr<Path> path = Path::pathNew();
                path->locationIs(curLocation);
                while (curLocation->name() != from->name()) {
                    t = lookup[curLocation->name()];
                    path->locationIs(t->location());
                    path->segmentIs(t->segment().ptr());
                    curLocation = t->location();
                }
                path->reversed();
            }
            for (unsigned int i = 0; i < curLocation->segments(); ++i) {
                WeakPtr<Segment> seg = curLocation->segment(i);
                WeakPtr<Segment> returnSeg = seg->returnSegment();
                if (returnSeg) {
                    Ptr<Location> nextLocation = returnSeg->source();
                    if (lookup.find(nextLocation->name() ) == lookup.end() ) {
                        t = new DistanceTuple<Mile>(nextLocation, seg.ptr(), best->distance().value() + seg->length().value());
                        distanceTuple.push_back(t);
                        lookup[nextLocation->name()] = t;
                    }
                    else { //try to update the tuple value
                        t = lookup[nextLocation->name()];
                        if (best->distance().value() + seg->length().value() < t->distance().value() ){
                            t->distanceIs(best->distance().value() + seg->length().value());
                            t->segmentIs(seg.ptr());
                        }
                    }
                }
                else {
                    //ignore invalid return segment
                }
            }
        }
        return NULL;
    }
  private:
    template <typename T> //T must be Ordinal
    class DistanceTuple : public Fwk::PtrInterface<DistanceTuple<T> > {
      public:
        DistanceTuple(Ptr<Location> _location, Ptr<Segment> _segment, T _distance) : location_(_location), segment_(_segment), distance_(_distance) {}
        void locationIs(const Ptr<Location> _location) { location_ = _location; }
        void segmentIs(const Ptr<Segment> _segment) { segment_ = _segment; }
        void distanceIs(const T _distance) { distance_ = _distance; }
        Ptr<Location> location() const { return location_; }
        Ptr<Segment> segment() const { return segment_; }
        T distance() const { return distance_; }
        class DistanceTupleComp {
            public:
                DistanceTupleComp() {}
                bool operator()(Ptr<DistanceTuple> a, Ptr<DistanceTuple> b) const {
                    return (a->distance().value() > b->distance().value());
                }
        };
      private:
        Ptr<Location> location_; //Current Location
        Ptr<Segment> segment_; //Previous Segment
        T distance_;
    };

};
#endif
