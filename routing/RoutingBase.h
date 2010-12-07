#ifndef ROUTING_BASE_H
#define ROUTING_BASE_H

#include "../Ptr.h"
#include "../PtrInterface.h"

template <class T>
class RoutingAlgorithm : public Fwk::PtrInterface<T> {
  public:
    void shippingNetworkIs(ShippingNetwork* network) { network_ = network; }
    virtual Ptr<Path> nextHop(const WeakPtr<Shipment> shipment) = 0;
    static Ptr<T> instance(ShippingNetwork* network) {
        if (instance_ == NULL) {
            instance_ = new T();
            instance_->shippingNetworkIs(network);
        }
        return instance_;
    }
  protected:
    RoutingAlgorithm() {}
    static Ptr<T> instance_;
  private:
    ShippingNetwork* network_;

    RoutingAlgorithm(const RoutingAlgorithm&);
};
template <class T> Ptr<T> RoutingAlgorithm<T>::instance_ = NULL;

#endif
