#ifndef ROUTING_RANDOM_WALK_H
#define ROUTING_RANDOM_WALK_H

#include "stdlib.h"
#include "time.h"
#include "routing.h"

class RandomWalk : public RoutingAlgorithm<RandomWalk> {
  public:
    virtual Ptr<Path> nextHop(const WeakPtr<Shipment> shipment) {
        if (shipment->currentLocation()->segments() <= 0) return NULL;
        Ptr<Path> path = Path::pathNew();
        path->locationIs(shipment->currentLocation());
        WeakPtr<Segment> seg = NULL;
        while (seg == NULL || seg->returnSegment() == NULL) {
            seg = shipment->currentLocation()->segment(rand() % shipment->currentLocation()->segments());
        }
        path->segmentIs(seg);
        path->locationIs(seg->returnSegment()->source());
        return path;
    }
    RandomWalk() { srand(time(NULL)); }
};

#endif
