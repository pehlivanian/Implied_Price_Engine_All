#ifndef __IMPLIEDBOOKSUBSCRIBER_HPP__
#define __IMPLIEDBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using BookPublishEvent = BookSubscriber::BookPublishEvent;

template<typename T>
class ImpliedBookSubscriber : public BookSubscriber
{
public:
  ImpliedBookSubscriber(T *k ) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    ; //nop
  }

  void update_bid(const BookPublishEvent& e) override
  {
    int p = (e.payload_).first;
    if (p > *knot_)
      *knot_ = p;
  }

  void update_ask(const BookPublishEvent& e) override
  {
    int p = (e.payload_).first;
    if (p < *knot_)
      *knot_ = p;
  }
private:
  T* knot_;
};

#endif
