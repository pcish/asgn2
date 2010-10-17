
#include <string>

#include "Engine.h"

namespace Shipping {

void Location::segmentIs (Ptr<Segment> seg) {
    if (seg == NULL) {
        cerr << "Can't add NULL segment" << endl;
        return;
    }
    segments_.push_back (seg);
}
void Location::segmentIs (unsigned int index, Ptr<Segment> seg) {    
    if (index < 0 || index >= segments_.size() ) {
        cerr <<  "Index out of bound exception" << endl;
        return;
    }
    if (seg == NULL) { //delete segment
        
    }
    else { //put segment at that position
        
    }
}
unsigned int Location::segments () { return segments_.size(); }

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
void Terminal::segmentIs (unsigned int index, Ptr<Segment> seg) {
    if (seg != NULL && seg->transportationMode() != transMode_ ) {// insert node rather than delete
        cerr << "Terminal must connect two segments with the same transportation mode" << endl;
        return;
    }
    Location::segmentIs (index, seg);
}


/*
static Ptr<Terminal> Engine::terminalNew () {
    return new Engine::Terminal ();
}
*/
} /* end namespace */

