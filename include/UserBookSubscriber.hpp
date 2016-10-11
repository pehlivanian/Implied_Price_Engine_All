#ifndef __USERBOOKSUBSCRIBER_HPP__
#define __USERBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using Price_Size_Pair = std::pair<int, size_t>;
using BookPublishEvent = BookSubscriber<Price_Size_Pair>::BookPublishEvent ;

class UserBookSubscriber : public BookSubscriber<Price_Size_Pair>
{
public:
  UserBookSubscriber(Price_Size_Pair* k) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    ; // nop
  }

  void update_bid(const BookPublishEvent& e) override
  {
    if ((e.payload_).first > (*knot_).first)
      *knot_ = e.payload_;
  }

  void update_ask(const BookPublishEvent& e) override
  {
    if ((e.payload_).first < (*knot_).first)
      *knot_ = e.payload_;
  }
  Price_Size_Pair* knot_;
};


#endif
