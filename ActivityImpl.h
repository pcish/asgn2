#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__

#include <map>
#include <string>
#include <queue>

#include "Activity.h"
#include "Queue.h"

using namespace std;

Fwk::Ptr<Activity::Manager> activityManagerInstance();
void activityManagerInstanceIs(Fwk::Ptr<Activity::Manager>);

namespace ActivityImpl {
    class ManagerImpl;
    //Comparison class for activities
    class ActivityComp : public binary_function<Activity::Ptr, Activity::Ptr, bool> {
        public:
            ActivityComp() {}

            bool operator()(Activity::Ptr a, Activity::Ptr b) const {
                return (a->nextTime() > b->nextTime());
            }
    };

    class ActivityImpl : public Activity {
        protected:
            ActivityImpl(const string& name, ManagerImpl* manager)
                : Activity(name), status_(free), nextTime_(0.0), notifiee_(NULL), manager_(manager) {}
            ManagerImpl* manager() const { return manager_; }

            virtual Status status() const { return status_; }
            virtual void statusIs(Status s) {
                status_ = s;
                if (notifiee_ != NULL) {
                    notifiee_->onStatus();
                }
            }

            virtual Hour nextTime() const { return nextTime_; }
            virtual void nextTimeIs(Hour t) {
                nextTime_ = t;
                if (notifiee_ != NULL) {
                    notifiee_->onNextTime();
                }
            }

            virtual Notifiee::Ptr notifiee() const { return notifiee_; }

            virtual void lastNotifieeIs(Notifiee* n) {
                ActivityImpl* me = const_cast<ActivityImpl*>(this);
                me->notifiee_ = n;
            }
            virtual ~ActivityImpl() {}


        private:
            friend class ManagerImpl;
            Status status_;
            Hour nextTime_;
            Fwk::Ptr<Notifiee> notifiee_;
            ManagerImpl*  manager_;
    };

    class ManagerImpl : public Activity::Manager {

        public:
            typedef Fwk::Ptr<ManagerImpl> Ptr;


            virtual Activity::Ptr activityNew(const string& name);
            virtual Activity::Ptr activity(const string& name) const;
            virtual void activityDel(const string& name);

            virtual Hour now() const { return now_; }
            virtual void nowIs(Hour time);

            static Fwk::Ptr<Activity::Manager> activityManagerInstance();
            static void activityManagerInstanceIs(const Fwk::Ptr<Activity::Manager> _instance);

            virtual void lastActivityIs(Activity::Ptr activity);
            virtual void timeSteppingIs(const TimeStepping _timeStepping) { timeStepping_ = _timeStepping; }
            virtual TimeStepping timeStepping() { return timeStepping_; }

        protected:
            ManagerImpl() : now_(0), timeStepping_(virtualtime_) {}

            //Data members
            priority_queue<Activity::Ptr, vector<Activity::Ptr>, ActivityComp> scheduledActivities_;
            map<string, Activity::Ptr> activities_; //pool of all activities
            Hour now_;
            TimeStepping timeStepping_;
            ~ManagerImpl() { activityInstance_ = NULL; }

            static Activity::Manager::Ptr activityInstance_;

    };

}

#endif /* __ACTIVITY_IMPL_H__ */

