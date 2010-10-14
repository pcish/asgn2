#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include "Ptr.h"
#include "PtrInterface.h"

using std::string;
using Fwk::Ptr;

///
/// Instance provides the external interface to entities in
/// our shipping network simulation. For flexibility
/// in future extensions, clients always refer to instances and
/// their attributes by name and attribute values are strings.
///
class Instance : public Fwk::PtrInterface<Instance> {
public:

    ///
    /// Returns the name of the instance.
    ///
    virtual string name() {
        return name_;
    }

    ///
    /// Returns the value of the attribute with the given name.
    /// If an attribute with the given name has not been set explicitly
    /// then the return value is the default value for the attribute,
    /// if any, or the empty string ("") otherwise.
    ///
    virtual string attribute(const string& attributeName) = 0;

    ///
    /// Modify the named attribute to have the value
    /// specified by the given string.
    ///
    /// In some situations, setting an attribute on an instance
    /// may implicitly modify attributes on other instances.
    ///
    virtual void attributeIs(const string& name, const string& v) = 0;

    // See full definition below.
    class Manager;

protected:
    Instance(const string& name) : name_(name) {
        // Nothing else to do.
    }

private:
    string name_;

};

///
/// Instance::Manager provides the interface to create and destroy
/// instances.
///
class Instance::Manager : public Fwk::PtrInterface<Instance::Manager> {
public:
    ///
    /// Returns a newly-created instance with the specified name.
    /// The spec paramater indicates additional information about
    /// the instance, such as its type.
    ///
    /// Names must be unique to a manager, so it is an error to call
    /// instanceNew with a name that is already associated with an instance
    /// through this manager.
    ///
    virtual Ptr<Instance> instanceNew(
        const string& name, const string& spec
    ) = 0;

    ///
    /// Return the instance with the given name or null if there is none.
    ///
    virtual Ptr<Instance> instance(const string& name) = 0;

    ///
    /// Delete the instance with the given name, if there is one.
    ///
    /// Deleting an instance may implicitly modify other instances
    /// associated with the instance.
    ///
    virtual void instanceDel(const string& name) = 0;

};

///
/// Return an instance manager for the shipping network.
///
/// This manager uses the spec parameter on instanceNew to indicate
/// a type of entity to create. The shipping instance manager supports
/// creating the following types:
///
///     "Customer"
///     "Port"
///     "Truck terminal"
///     "Boat terminal"
///     "Plane terminal"
///     "Truck segment"
///     "Boat segment"
///     "Plane segment"
///     "Stats"
///     "Conn"
///     "Fleet"
///
/// The returned instances for Customer, Port, and the terminal types
/// are referred to as location instances, and support attributes
/// of the form "segmentN" where N is a positive integer.  A valid value
/// for one of these attributes is the name of a segment instance or
/// the empty string ("") if the named attribute is not set.
///

/// The segmentN attributes should always begin at 1 and should be
/// read-only. This means that by setting a segment's location by:
/// 
/// segmentRep->attributeIs("source", "loc");
///
/// should implicitly set the segmentN attribute of the location to the
/// greatest unused number. Also, deleting a segment by doing:
///
/// segmentRep->attributeIs("source", "");
///
/// should implicitly remove the corresponding segmentN attribute of
/// location and also shift down the segment numbers greater than N, so
/// that there are no gaps in the sequence.
///
/// The returned instances for segment types support the following attributes:
///
///    "source"
///    "length"
///    "return segment"
///    "difficulty"
///    "expedite support"
///
/// The source attribute contains the name of the instance that connects to
/// the segment instance; that is, the source has a "segmentN" attribute
/// that names the segment instance.
///
/// The length attribute contains a non-negative number specifying the
/// length of this segment, in miles.
///
/// The difficulty attribute is a floating point number from 1.0 to 5.0
/// representing the difficulty of traveling along the segment.
///
/// The expedite support attribute indicates whether the segment supports
/// expedited shipping or not, and should return either "yes" or "no".
///
/// The return segment attribute contains the name of a segment instance
/// specifying where a segment connects. For example, a client may
/// specify that location A is connected to location B as follows:
///
///     Ptr<Instance> loc1 = mgr->instanceNew("A", "Customer");
///     Ptr<Instance> loc2 = mgr->instanceNew("B", "Port");
///     Ptr<Instance> s1 = mgr->instanceNew("A-B", "Truck segment");
///     Ptr<Instance> s2 = mgr->instanceNew("B-A", "Truck segment");
///     s1->attributeIs("source", loc1->name());
///     s2->attributeIs("source", loc2->name());
///     s1->attributeIs("return segment", s2->name());
///
/// Specifying the return segment of a segment implicitly sets the
/// return segment attribute of the referenced segment. In the example above,
/// the last line implicitly sets s2->attribute("return segment") to s1->name()
///
/// The instance manager may implicitly disassociate objects
/// that are deleted. In the example above, instanceDel(loc2->name())
/// may delete s2 and may make s1->attribute("return segment") return
/// the empty string ("").
///
/// The Stats, Conn and Fleet types are special in that the manager
/// only allows one instance of each type to exist at any given time.
///
extern Ptr<Instance::Manager> shippingInstanceManager();

#endif
