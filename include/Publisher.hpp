#ifndef __PUBLISHER_HPP__
#define __PUBLISHER_HPP__

#include <future>
#include <list>
#include <vector>

#include "Subscriber.hpp"

template<typename T>
class Publisher
{
public:

  using PublishEvent  = typename Subscriber<T>::PublishEvent;
  using SubscriberPtr = std::shared_ptr<Subscriber<T>>;
  using Subscribers   = std::list<SubscriberPtr>;
  using Futures       = std::vector<std::future<int>>;

  Publisher() = default;
  inline void attach(Subscriber<T>* s)             { add_subscriber(s); }
  inline void attach_bid(Subscriber<T>* s)	   { add_bid_subscriber(s); }
  inline void attach_ask(Subscriber<T>* s)	   { add_ask_subscriber(s); }
  inline void attach(SubscriberPtr s)              { add_subscriber(s); }
  inline void attach_bid(SubscriberPtr s)	   { add_bid_subscriber(s); }
  inline void attach_ask(SubscriberPtr s)	   { add_ask_subscriber(s); }
  void nudge(const PublishEvent& e)                { notify(e); }
  void nudge_bid(const PublishEvent& e)		   { notify_bid(e); }
  void nudge_ask(const PublishEvent& e)		   { notify_ask(e); }
  size_t num_subscribers() const                   { return subscribers_.size(); }
  std::list<SubscriberPtr> get_subscribers() const { return subscribers_; }
private:
  virtual void notify(const PublishEvent&)     = 0;
  virtual void notify_bid(const PublishEvent&) = 0;
  virtual void notify_ask(const PublishEvent&) = 0;
  void add_subscriber(Subscriber<T>*);
  void add_bid_subscriber(Subscriber<T>*);
  void add_ask_subscriber(Subscriber<T>*);
  void add_bid_subscriber(SubscriberPtr);
  void add_ask_subscriber(SubscriberPtr);
  void add_subscriber(SubscriberPtr);

protected:  
  Subscribers subscribers_, bid_subscribers_, ask_subscribers_;
  Futures futures_, bid_futures_, ask_futures_;
};

template<typename T>
void 
Publisher<T>::add_subscriber(Subscriber<T>* S)
{
  subscribers_.push_back(SubscriberPtr(S));
  futures_.push_back(std::future<int>());
}

template<typename T>
void
Publisher<T>::add_subscriber(SubscriberPtr S)
{
  subscribers_.push_back(S);
  futures_.push_back(std::future<int>());
}

template<typename T>
void
Publisher<T>::add_bid_subscriber(Subscriber<T>* S)
{
  bid_subscribers_.push_back(SubscriberPtr(S));
  bid_futures_.push_back(std::future<int>());
}

template<typename T>
void
Publisher<T>::add_ask_subscriber(Subscriber<T>* S)
{
  ask_subscribers_.push_back(SubscriberPtr(S));
  ask_futures_.push_back(std::future<int>());
}

template<typename T>
void
Publisher<T>::add_bid_subscriber(SubscriberPtr S)
{
  bid_subscribers_.push_back(S);
  bid_futures_.push_back(std::future<int>());
}

template<typename T>
void
Publisher<T>::add_ask_subscriber(SubscriberPtr S)
{
  ask_subscribers_.push_back(S);
  ask_futures_.push_back(std::future<int>());
}

#endif
