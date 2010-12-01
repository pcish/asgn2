#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__

#include <list>
#include <map>

#include "fwk/Exception.h"
#include "Activity.h"

using namespace std;

Ptr<Activity::Manager> activityManagerInstance();

namespace ActivityImpl {

class ActivityImpl : public Activity {
public:
    /* Write this class */
    virtual Status status() const { return status_; }
    virtual void statusIs(const Status _status){ status_ = _status; }
    virtual Time nextTime() const {return nextTime_;}
    virtual void nextTimeIs(Time _nextTime){ nextTime_ = _nextTime; }
    virtual Fwk::Ptr<Notifiee> notifiee() const { 
        Fwk::Ptr<Notifiee> front = notifiee_.front();
        return front; 
    }
    virtual void lastNotifieeIs(Notifiee *n) { 
        Fwk::Ptr<Notifiee> nptr(n);
        notifiee_.push_back(nptr); 
    }
    class ManagerImpl;
private:
    ActivityImpl(const string &name) : Activity(name), nextTime_(0) {}
    Status status_;
    Time nextTime_;
    list<Fwk::Ptr<Notifiee> > notifiee_;
};

//Manager should be a singleton
class ActivityImpl::ManagerImpl : public Activity::Manager {
public:
    /* Write this class */
    virtual Activity::Ptr activityNew(const string &name) {
        if (activity_.find(name) != activity_.end() )
            throw Fwk::NameInUseException("Activity already exists: " + name);
        Activity::Ptr act = new ActivityImpl(name);
        activity_[name] = act;
        return act;
    }

    virtual Activity::Ptr activity(const string &name) const {
        map<string, Activity::Ptr>::const_iterator p = activity_.find(name);
        if (p == activity_.end() )
            return NULL;
        return p->second;
    }

    virtual void activityDel(const string &name) { activity_.erase(name); }
    virtual void lastActivityIs(Activity::Ptr act) { activity_.insert(make_pair(act->name(), act) ); }    

    virtual Time now() const { return now_; }
    virtual void nowIs(Time _now) { now_ = _now;} 
    static Fwk::Ptr<ManagerImpl> instance() {
        if (instance_ == NULL)
            instance_ = new ManagerImpl();
        return instance_;
    }

private:
    ManagerImpl() : now_(0) {}
    map<string, Activity::Ptr> activity_;
    Time now_;
    static Fwk::Ptr<ManagerImpl> instance_;
};
Fwk::Ptr<ActivityImpl::ManagerImpl> ActivityImpl::ManagerImpl::instance_ = NULL;

}

Fwk::Ptr<Activity::Manager> activityManagerInstance() {
    return ActivityImpl::ActivityImpl::ManagerImpl::instance();
}

#endif /* __ACTIVITY_IMPL_H__ */

