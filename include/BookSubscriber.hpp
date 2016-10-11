#ifndef __BOOKSUBSCRIBER_HPP__
#define __BOOKSUBSCRIBER_HPP__

#include "Subscriber.hpp"

using Book_Size_Pair = std::pair<int, size_t>;

class BookSubscriber : public Subscriber<Book_Size_Pair>
{
public:
  using BookPublishEvent = Subscriber::PublishEvent;

  virtual void update(const BookPublishEvent&) = 0;
};

#endif
