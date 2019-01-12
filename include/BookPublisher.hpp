#ifndef __BOOKPUBLISHER_HPP__
#define __BOOKPUBLISHER_HPP__

#include <list>
#include <memory>
#include <functional>

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"

using Price_Size_Pair  = std::pair<int, size_t>;
using BookPublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;


class BookPublisher : public Publisher<Price_Size_Pair>
{
public:

  BookPublisher() {}
protected:
  inline void notify(const BookPublishEvent&)     override;
  inline void notify_bid(const BookPublishEvent&) override;
  inline void notify_ask(const BookPublishEvent&) override;

};

inline void notify_(const BookPublishEvent& e, const typename Publisher<Price_Size_Pair>::Subscribers& s) {
  std::for_each(s.begin(), s.end(), [&e](auto s_) { s_->update(e); } );
}

inline void
BookPublisher::notify(const BookPublishEvent& e)
{
  notify_( e, subscribers_);
}

inline void
BookPublisher::notify_bid(const BookPublishEvent& e)
{
  notify_(e, bid_subscribers_);
}

inline void
BookPublisher::notify_ask(const BookPublishEvent& e)
{
  notify_(e, ask_subscribers_);
}

#endif
