#ifndef __IMPLIEDQUOTESUBSCRIBER_HPP__
#define __IMPLIEDQUOTESUBSCRIBER_HPP__

#include <vector>
#include <string>
#include <exception>
#include <mutex>

#include "QuoteSubscriber.hpp"
#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"
#include "MarketGraph.hpp"
#include "cat1_visitor.hpp"
#include "cat2_visitor.hpp"

using Price_Size_Pair = std::pair<int, size_t>;
using QuotePublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;
using BookPublishEvent = BookSubscriber<Price_Size_Pair>::BookPublishEvent;

class ImpliedQuoteSubscriber : public QuoteSubscriber<Price_Size_Pair>, public BookPublisher
{
public:

    using graph_visitor = cat2_visitor;

  ImpliedQuoteSubscriber() : v1_(-1), v2_(-1), G_(new MarketGraph()) {}
  ImpliedQuoteSubscriber(int v1, int v2, MarketGraph* G, int leg) :
    v1_(v1),
    v2_(v2),
    G_(G),
    ask_visitor_(new graph_visitor(0)),
    bid_visitor_(new graph_visitor(1)),
    leg_(leg)
  {}
  ~ImpliedQuoteSubscriber() = default;

  void update(const QuotePublishEvent& e ) override;
  void update_bid(const QuotePublishEvent& e) override;
  void update_ask(const QuotePublishEvent& e) override;

private:
  int v1_;
  int v2_;
  MarketGraph *G_;
  graph_visitor* ask_visitor_;
  graph_visitor* bid_visitor_;
  int leg_;  
};

#endif
