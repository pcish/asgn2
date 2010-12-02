#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>
#include <deque>

#include "PtrInterface.h"
#include "Ptr.h"

#include "Nominal.h"

typedef Nominal<class QueueSize_, unsigned int> QueueSize;

/* ===========================================================================
 * A pretty typical Q which defines enQ and deQ, and a queue size.
 * The default Q size is 10
 * ==========================================================================*/
class Queue : public Fwk::PtrInterface<Queue>
{
public:
  typedef Fwk::Ptr<Queue> Ptr;
  Queue() : queueSize_(10) {}

  QueueSize queueSize() const { return queueSize_; }
  void queueSizeIs(QueueSize queueSize) { queueSize_ = queueSize; }

  int size() { return deq_.size(); }

  int deQ() {
    if(deq_.empty()) {
      return 0;
    }

    int value = deq_.front();

    deq_.pop_front();
    return value;
  }

  void enQ(int v) {
    if(deq_.size() < queueSize().value()) {
      deq_.push_back(v);
    } else {
      std::cout << "q full " << std::endl;
    }
  }

private:
  QueueSize queueSize_;
  std::deque<int> deq_;
};

#endif
