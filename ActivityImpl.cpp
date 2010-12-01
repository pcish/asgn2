#include    "Ptr.h"
#include    "ActivityImpl.h"

namespace ActivityImpl {
    Fwk::Ptr<ActivityImpl::ManagerImpl> ActivityImpl::ManagerImpl::instance_ = NULL;
}
Fwk::Ptr<Activity::Manager> activityManagerInstance() {
    return ActivityImpl::ActivityImpl::ManagerImpl::instance();
}
