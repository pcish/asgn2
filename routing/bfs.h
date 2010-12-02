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
        while (!loc.empty() ){
            Ptr<Location> curLocation = loc.front();
            loc.pop();
            visited.insert(curLocation->name() );
            for (unsigned int i = 0; i < curLocation->segments(); ++i) {
                WeakPtr<Segment> seg = curLocation->segment(i);
                WeakPtr<Segment> returnSeg = seg->returnSegment();
                if (returnSeg) {
                    Ptr<Location> nextLocation = returnSeg->source();
                    if (visited.find(nextLocation->name()) != visited.end() ){
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

#endif
