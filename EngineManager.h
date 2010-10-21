#ifndef ENGINE_MNG_H
#define ENGINE_MNG_H
#include "Engine.h"
#include "Ptr.h"
#include "PtrInterface.h"
namespace Shipping {

class EngineManager {
  public:
     Ptr<Customer> customerNew(const string name){
        Ptr<Customer> m = new Customer(name);
        return m;
    }
     Ptr<PlaneFleet> planeFleetNew(){
        Ptr<PlaneFleet> m = new PlaneFleet();
        return m;
    }
     Ptr<TruckFleet> truckFleetNew(){
        Ptr<TruckFleet> m = new TruckFleet();
        return m;
    }
     Ptr<Terminal> terminalNew(const string name, const Segment::TransportationMode transportationMode){
        Ptr<Terminal> m = new Terminal(name, transportationMode);
        return m;
    }
     Ptr<Location> locationNew(const string name){
        Ptr<Location> m = new Location(name);
        return m;
    }
     Ptr<BoatFleet> boatFleetNew(){
        Ptr<BoatFleet> m = new BoatFleet();
        return m;
    }
     Ptr<Fleet> fleetNew(){
        Ptr<Fleet> m = new Fleet();
        return m;
    }
     Ptr<Segment> segmentNew(const TransportationMode transportationMode, const string name){
        Ptr<Segment> m = new Segment(transportationMode, name);
        return m;
    }
     Ptr<Port> portNew(const string name){
        Ptr<Port> m = new Port(name);
        return m;
    }
};
}
#endif
