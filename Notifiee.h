#ifndef __NOTIFIEE_H__
#define __NOTIFIEE_H__

#include <string>

#include "PtrInterface.h"
#include "Ptr.h"

using namespace std;

namespace Fwk {

 class RootNotifiee : public PtrInterface<RootNotifiee> {
	/* Deliberately empty */
    };

    template<typename Notifier>
	class BaseNotifiee : public RootNotifiee {
	
    public:
    BaseNotifiee(Notifier* n = NULL) : notifier_(n) {
	    if (n != NULL) {
		n->lastNotifieeIs(static_cast<typename Notifier::Notifiee*>(this));
	    }
	}
	
	~BaseNotifiee() {
	    if (notifier_ != NULL) {
		notifier_->lastNotifieeIs(0);
	    }
	}
	
	Ptr<Notifier> notifier() const {
	    return notifier_;
	}
	
	void notifierIs(Ptr<Notifier> n) {
	    if (notifier_ != n) {
		if (notifier_ != NULL) {
		    notifier_->lastNotifieeIs(0);
		}
		notifier_ = n;
		if (n != NULL) {
		    n->lastNotifieeIs(
				      static_cast<typename Notifier::Notifiee*>(this)
				      );
		}
	    }
	}
	
    private:
	Ptr<Notifier> notifier_;
	
    };
 
} //end namespace Fwk   
#endif
    
