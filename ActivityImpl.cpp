#include    "Ptr.h"
#include    "types.h"
#include    "ActivityImpl.h"

namespace ActivityImpl {
    void ActivityImpl::statusIs(const Status _status){ 
        if (status_ == _status)
            return; //Do nothing, obeying the idempotent mutator syntax
        status_ = _status; 
        switch (status_) {
            case executing:
            break;
            default:
            break;
        }
    }

    void ActivityImpl::ManagerImpl::nowIs(Time _now) {
        Time prevTime(now_);
        for (; now_.value() + 1.0 <= _now.value(); now_ = now_.value() + 1.0) {
            for (map<string, Activity::Ptr>::iterator actPtr = activity_.begin(); actPtr != activity_.end(); actPtr ++) {
                if (actPtr->second->status() == Activity::ready) {
                    actPtr->second->statusIs(Activity::executing);
                }
            }
        }
        now_ = _now;
    }
    Fwk::Ptr<ActivityImpl::ManagerImpl> ActivityImpl::ManagerImpl::instance_ = NULL;
    
}
Fwk::Ptr<Activity::Manager> activityManagerInstance() {
    return ActivityImpl::ActivityImpl::ManagerImpl::instance();
}
