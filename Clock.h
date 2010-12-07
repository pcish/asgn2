#ifndef CLOCK_H
#define CLOCK_H

#include <list>
#include "fwk/BaseNotifiee.h"
#include "fwk/NamedInterface.h"
#include "Ptr.h"
#include "WeakPtr.h"
#include "PtrInterface.h"
#include "ActivityImpl.h"
#include "Log.h"
#include "types.h"
#include "entities.h"

using Fwk::Ptr;

namespace Shipping {

class Clock : public Fwk::NamedInterface {
  public:
    typedef Fwk::Ptr<Clock> Ptr;

    Clock();
    Hour now() const { return now_; }
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
        friend class Clock;
      public:
        typedef Fwk::Ptr<Notifiee> Ptr;
        enum Status {
            Active__,
            Deleted__
        };
        virtual void notifierIs(Fwk::Ptr<Clock> notifier) {
            if (notifier_ == notifier) return;
            if (notifier_) notifier->notifieeIs(0);
            notifier_ = notifier;
            notifier_->notifieeIs(this);
        }
        static Fwk::Ptr<Clock::Notifiee> notifieeNew() {
            Fwk::Ptr<Clock::Notifiee> n = new Notifiee();
            return n;
        }
        virtual void onNow() {}
        Status status() const { return status_; }
        void statusIs(const Status _status) { status_ = _status; }

      protected:
        Fwk::WeakPtr<Clock> notifier_;
        Notifiee() : notifier_(0), status_(Active__) {}
        Status status_;
    };

  protected:
    list<Clock::Notifiee::Ptr> notifiee_;
    void notifieeIs(Clock::Notifiee* n) {
        notifiee_.push_back(n);
    }

  private:
    Hour now_;
    class StatusDeletedValue {
      public:
        bool operator() (const Clock::Notifiee::Ptr a) {
            return a->status() == Clock::Notifiee::Deleted__;
        }
    };
    void nowInc() {
        Activity::Manager::Ptr manager = activityManagerInstance();
        now_ = now_.value() + 1;
        LOG_INFO("nowInc", "now = " + STR(now_.value()));
        notifiee_.remove_if(StatusDeletedValue());
        for(list<Clock::Notifiee::Ptr>::iterator i = notifiee_.begin(); i != notifiee_.end(); i++) {
            if ((*i)->status_ == Clock::Notifiee::Deleted__) {
                cout << "Should not appear anymore" << endl;
            }
            else {
                (*i)->onNow();
            }
        }
        heartbeatActivity_->nextTimeIs(manager->now().value() + 1);
        manager->lastActivityIs(heartbeatActivity_.ptr());
    }
    Activity::Activity::Ptr heartbeatActivity_;
    class HeartbeatActivityNotifiee : public Activity::Activity::Notifiee {
      public:
        HeartbeatActivityNotifiee(Activity::Activity *activity, Clock *_parent) :
            Activity::Activity::Notifiee(activity), activity_(activity),
            parent_(_parent) {}
        virtual void onStatus() {
            Activity::Activity::Status status = activity_->status();
            if (status == Activity::Activity::executing) {
                parent_->nowInc();
            }
        }
        virtual void onNextTime() {}

      private:
        Activity::Activity *activity_;
        Clock *parent_;
    };
};

class ClockReactor: public Clock::Notifiee {
  public:
    typedef Fwk::Ptr<ClockReactor> Ptr;
    virtual void onNow() {
        parent_->now_ = notifier_->now();
    }
    static ClockReactor::Ptr clockReactorNew(Fleet* parent) {
        ClockReactor::Ptr n = new ClockReactor();
        n->parent_ = parent;
        return n;
    }
    string name() const { return notifier_->name() + ".ClockReactor"; }
  private:
    Fleet* parent_;
};

}

#endif
