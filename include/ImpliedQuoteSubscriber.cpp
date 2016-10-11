#include "ImpliedQuoteSubscriber.hpp"

void
ImpliedQuoteSubscriber::update(const QuotePublishEvent& e)
{
  ; // nop
}

void 
ImpliedQuoteSubscriber::update_bid(const QuotePublishEvent& e)
{
    G_->updateEdgeWeight(v1_, v2_, -1 * e.payload_.first, e.payload_.second);
    
    // Ask side first
    // PRICE
    try
        { G_->accept(ask_visitor_); }
    catch(std::exception &e)
        { std::cerr << e.what() << "\n"; throw; }
    G_->update_predecessor(ask_visitor_->get_predecessor(), 1);
    G_->update_distance(ask_visitor_->get_distance(), 1);
    G_->update_size(ask_visitor_->get_size(), 1);
    auto dist_ask = G_->get_distance(1);

    // SIZE
    auto size_ask = G_->get_size(1);
    notify_ask(BookPublishEvent(std::make_pair(dist_ask[1], size_ask[1])));

    // Bid side second
        // PRICE
  try
    {
        G_->accept(bid_visitor_);
    }
  catch(std::exception& e)
    {
      std::cerr << e.what() << "\n"; throw;
    }
  G_->update_predecessor(bid_visitor_->get_predecessor(), 0);
  G_->update_distance(bid_visitor_->get_distance(), 0);
    G_->update_size(bid_visitor_->get_size(), 0);
  auto dist_bid = G_->get_distance(0);


    // SIZE
    auto size_bid = G_->get_size(0);
    notify_bid(BookPublishEvent(std::make_pair(-1 * dist_bid[0], size_bid[0])));
}

void
ImpliedQuoteSubscriber::update_ask(const QuotePublishEvent& e)
{
  G_->updateEdgeWeight(v1_, v2_, e.payload_.first, e.payload_.second);
  
  // Ask side first
    // PRICE
  try
  {
      G_->accept(ask_visitor_);
  }
  catch(std::exception &e)
    {
        std::cerr << e.what() << "\n"; throw;
    }
  G_->update_predecessor(ask_visitor_->get_predecessor(), 1);
  G_->update_distance(ask_visitor_->get_distance(), 1);
    G_->update_size(ask_visitor_->get_size(), 1);
  auto dist_ask = G_->get_distance(1);

    // SIZE
    auto size_ask = G_->get_size(1);
    notify_ask(BookPublishEvent(std::make_pair(dist_ask[1], size_ask[1])));

  // Bid side second
    // PRICE
  try { G_->accept(bid_visitor_); }
  catch(std::exception& e) { std::cerr << e.what() << "\n"; throw; }
  G_->update_predecessor(bid_visitor_->get_predecessor(), 0);
  G_->update_distance(bid_visitor_->get_distance(), 0);
    G_->update_size(bid_visitor_->get_size(), 0);
  auto dist_bid = G_->get_distance(0);

    // SIZE
    auto size_bid = G_->get_size(0);
    notify_bid(BookPublishEvent(std::make_pair(-1 * dist_bid[0], size_bid[0])));
}


