#include    "Ptr.h"
#include    "ActivityImpl.h"

namespace ActivityImpl {
    void ActivityImpl::ManagerImpl::nowIs(Time _now) {
        Time prevTime(now_);        
        now_ = _now;
    }
    Fwk::Ptr<ActivityImpl::ManagerImpl> ActivityImpl::ManagerImpl::instance_ = NULL;
    
}
Fwk::Ptr<Activity::Manager> activityManagerInstance() {
    return ActivityImpl::ActivityImpl::ManagerImpl::instance();
}
