#ifndef __USERENGINE_HPP__
#define __USERENGINE_HPP__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <atomic>
#include <limits>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "impl.hpp"
#include "UserQuoteSubscriber.hpp"
#include "UserBookSubscriber.hpp"
#include "QuotePublisher.hpp"

using SERIALIZER_W = boost::lock_guard<boost::shared_mutex>;
using SERIALIZER_R = boost::shared_lock<boost::shared_mutex>;

#define SERIALIZE_WRITES SERIALIZER_W lg(mut_);
#define SERIALIZE_READS  SERIALIZER_R ls(mut_);

using Price_Size_Pair = std::pair<int, size_t>;

template<int N>
class UserEngine {
public:
  UserEngine() : p_(std::make_unique<impl<UserEngine<N>>>()) { init_(); }
  
  inline void publish_bid(const SecPair&, const QuotePublishEvent&);
  inline void publish_ask(const SecPair&, const QuotePublishEvent&);

  void write_user_curve() const;
  void write_user_quote() const;

  int get_num_legs() const                                         { return p_->n_; }
  Price_Size_Pair get_bid(int leg) const                           { return (p_->uQuote_)[0][leg]; }
  Price_Size_Pair get_user_bid(int leg) const                      { return get_bid(leg); }
  Price_Size_Pair get_ask(int leg) const                           { return (p_->uQuote_)[1][leg]; }
  Price_Size_Pair get_user_ask(int leg) const                      { return get_ask(leg); }
  std::vector<std::vector<Price_Size_Pair>> get_quote() const      { return p_->uQuote; }
  std::vector<std::vector<Price_Size_Pair>> get_user_quote() const { return p_->uQuote; }
  
private:
  void init_();
  void init_subscribers_();
  void write_curve_(const std::vector<std::vector<std::pair<int, size_t>>>&) const;
  
  mutable boost::shared_mutex mut_;
  std::unique_ptr<impl<UserEngine<N>>> p_;
};

#include "UserEngine_impl.hpp"

#endif
