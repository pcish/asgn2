#include <iostream>
#include <time.h>

#include "ActivityImpl.h"
#include "Log.h"

Fwk::Ptr<Activity::Manager> activityManagerInstance(){
    return ActivityImpl::ManagerImpl::activityManagerInstance();
}
void activityManagerInstanceIs(Fwk::Ptr<Activity::Manager> _instance) {
    ActivityImpl::ManagerImpl::activityManagerInstanceIs(_instance);
}

namespace ActivityImpl {
    //Definition of static member
    Fwk::Ptr<Activity::Manager> ManagerImpl::activityInstance_ = NULL;

    //Gets the singleton instance of ManagerImpl
    Fwk::Ptr<Activity::Manager> ManagerImpl::activityManagerInstance() {

        if (activityInstance_ == NULL) {
            activityInstance_ = new ManagerImpl();
        }
        return activityInstance_;
    }
    void ManagerImpl::activityManagerInstanceIs(const Fwk::Ptr<Activity::Manager> _instance) {
        activityInstance_ = _instance;
    }

    Activity::Ptr ManagerImpl::activityNew(const string& name) {
        Activity::Ptr activity = activities_[name];

        if (activity != NULL) {
            throw Fwk::NameInUseException ("Activity already exists: " + name);
        }

        activity = new ActivityImpl(name, this);
        activities_[name] = activity;

        return activity;
    }

    Activity::Ptr ManagerImpl::activity(const string& name) const {
        map<string, Activity::Ptr>::const_iterator it = activities_.find(name);

        if(it != activities_.end() ) {
            return (*it).second;
        }

        // dont throw an exception (accessor)
        return NULL;
    }

    void ManagerImpl::activityDel(const string& name) {
        activities_.erase(name);
    }

    void ManagerImpl::lastActivityIs(Activity::Ptr activity) {
        scheduledActivities_.push(activity);
    }

    void ManagerImpl::nowIs(Hour t) {
        //find the most recent activites to run and run them in order
        while (!scheduledActivities_.empty()) {

            //figure out the next activity to run
            Activity::Ptr nextToRun = scheduledActivities_.top();

            if (nextToRun->status() == Activity::deleted) {
                scheduledActivities_.pop();
                activityDel(nextToRun->name());
                continue;
            }
            //if the next Hour is greater than the specified time, break
            //the loop
            if (nextToRun->nextTime() > t) {
                break;
            }

            //calculate amount of Hour to sleep
            Hour diff = Hour(nextToRun->nextTime().value() - now_.value());

            if (timeStepping_ == realtime())
                usleep(( ((int)diff.value()) * 1000000));

            now_ = nextToRun->nextTime();

            //run the minimum Hour activity and remove it from the queue
            scheduledActivities_.pop();

            nextToRun->statusIs(Activity::executing);
            nextToRun->statusIs(Activity::free);

        }

        //syncrhonize the time
        now_ = t;
    }

} //end namespace ActivityImpl
