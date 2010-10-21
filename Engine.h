#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>

using namespace std;
#include "Ptr.h"
#include "PtrInterface.h"
//#include "fwk/BaseNotifiee.h"
#include "fwk/NamedInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "types.h"

namespace Shipping {

// Create your rep/engine interface here.
//store name in each entity?
/*
TODO list:
1. Add destructor for every class 
2. Delete segment from Location
*/
/*
template <class T>
class BaseNotifiee {
    public:
        BaseNotifiee(T* _notifier) {  notifier_ = _notifier; }
        void notifierIs (T* _notifier) {
        }
    protected:
        T* notifier_;
};
*/

class Location;

class Segment : public Fwk::PtrInterface<Segment> {
public:
    enum TransportationMode {
        truck_, boat_, plane_
    };
    static inline TransportationMode truck() { return truck_; }
    static inline TransportationMode boat() { return boat_; }
    static inline TransportationMode plane() { return plane_; }

    static Ptr<Segment> SegmentNew (const string& _name, const TransportationMode& _transMode) { return new Segment(_name, _transMode); } 

    string name() const { return name_; }
    TransportationMode const transportationMode() { return transMode_; }
    Ptr<Location> source() const { return source_; }
    void sourceIs(const Ptr<Location> _source) { source_ = _source; }
    Mile length() const { return length_; }
    void lengthIs(const Mile& _length) { length_ = _length; } 
    Ptr<Segment> returnSegment() const { return returnSegment_; }
    void returnSegmentIs(const Ptr<Segment>& _returnSegment) { returnSegment_ = _returnSegment; }
    SegmentDifficultyUnit difficulty() const { return difficulty_; }
    void difficultyIs (const SegmentDifficultyUnit& _difficulty) { difficulty_ = _difficulty; }
    bool expediteSupport() const { return expediteSupport_; }
    void expediteSupportIs (const bool _expediteSupport) { expediteSupport_ = _expediteSupport; }
    
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee {
        public:
            virtual void onExpediteSupport() {}
            virtual void onSegment() {}
            virtual void notifierIs(Segment* _notifier) {
                if (notifier_ == _notifier)
                    return;
                if (notifier_) 
                    notifier_->notifieeIs(0);
                notifier_ = _notifier;
            }
            static Fwk::Ptr<Segment::Notifiee> notifieeNew () {
                Fwk::Ptr<Segment::Notifiee> n = new Notifiee ();
                return n;
            }
        protected:
            Segment* notifier_;
            Notifiee () : notifier_(0) {}
    };
    Ptr<Segment::Notifiee> notifiee() const { return notifiee_; }

protected:
    string name_;
    Ptr<Segment::Notifiee> notifiee_;
    void notifieeIs( Segment::Notifiee* n) const {
        Segment* me = const_cast<Segment*>(this);
        me->notifiee_ = n;
    }
private:
    Segment (const string& _name, const TransportationMode& _transMode) : name_(_name), transMode_(_transMode) {}
    TransportationMode transMode_;
    Ptr<Location> source_;
    Mile length_; 
    Ptr<Segment> returnSegment_;
    SegmentDifficultyUnit difficulty_;
    bool expediteSupport_;
};

class Location : public Fwk::PtrInterface<Location>{
    public:
        //static Ptr<Location> LocationNew (const string _name) { } 
        //declared virtual because subclasses may have additional contraints on the type of segment that connects
        Ptr<Segment> segment (unsigned int index);
        virtual void segmentIs (Ptr<Segment> seg);
        virtual void segmentIs (unsigned int index, Ptr<Segment> seg); // assign segment for nth segment
        //unsigned int segments ();
        string name() { return name_; }
    protected:
        Location (const string& _name) : name_(_name) {}
        vector<Ptr<Segment> > segments_;
        string name_;
};

class Customer : public Location {
    public:
        static Ptr<Customer> CustomerNew(const string& _name) { return new Customer(_name); }
    protected:
        Customer(string _name) : Location(_name) {}
};
class Port : public Location {
    public:
        static Ptr<Port> CustomerNew(const string& _name) { return new Port(_name); }
    protected:
        Port(string _name) : Location(_name) {}
};
class Terminal : public Location {
    public:
        static Ptr<Terminal> TerminalNew(const string& _name, Segment::TransportationMode _transMode) { return new Terminal(_name, _transMode); }
        Segment::TransportationMode transportationMode () { return transMode_; }
        //a terminal's type should be assigned at the beginning and cannot withstand changing after instantiated
        //void transportationModeIs (Segment::TransportationMode _transMode);
        virtual void segmentIs (Ptr<Segment> seg);
        virtual void segmentIs (unsigned int index, Ptr<Segment> seg);
    protected:
        Terminal (string _name, Segment::TransportationMode _transMode) : Location(_name), transMode_ (_transMode) {}
    private:
        Segment::TransportationMode transMode_;
};

class Fleet : public Fwk::PtrInterface<Fleet> {
    public:
        static Ptr<Fleet> instance();

        Mile speed (const Segment::TransportationMode _mode) { return speed_.find(_mode)->second; }
        void speedIs (const Segment::TransportationMode _mode, const Mile& v) { speed_.find(_mode)->second = v; }
        PackageUnit capacity (const Segment::TransportationMode _mode) { return capacity_.find(_mode)->second; }
        void capacityIs (const Segment::TransportationMode _mode, const PackageUnit& v) { capacity_.find(_mode)->second = v; }
        USD cost(const Segment::TransportationMode _mode) { return cost_.find(_mode)->second; }
        void costIs (const Segment::TransportationMode _mode, const USD& v) { cost_.find(_mode)->second = v; }

    private:
    Fleet();
    //Segment::TransportationMode transMode_;
    map<Segment::TransportationMode, Mile> speed_;
    map<Segment::TransportationMode, PackageUnit> capacity_;
    map<Segment::TransportationMode, USD> cost_;
    static Ptr<Fleet> instance_;
};

} /* end namespace */

#endif
