#ifndef __USERENGINE_IMPL_HPP__
#define __USERENGINE_IMPL_HPP__

template<int N>
class UserEngine;

template<int N>
struct impl<UserEngine<N>> {

  impl() :
    n_(N)
    {
      for (size_t i = 0; i < 2; ++i) {
          auto val = (i == 0) ? std::make_pair(std::numeric_limits<int>::min(), 0) :
                     std::make_pair(std::numeric_limits<int>::max(), 0);
          uQuote_[i].fill(val);
      }
      for (size_t i=0; i<2; ++i)
          quote_publishers_[i] = std::array<QuotePublisher, N>();
    }

  int n_;

  using atomic_price = std::atomic<int>;
  using atomic_size  = std::atomic<size_t>;

  std::array<std::array<Price_Size_Pair, N>, 2> uQuote_;
  std::array<std::array<QuotePublisher, N>, 2> quote_publishers_;
};

using UQSub = std::shared_ptr<UserQuoteSubscriber>;
using UBSub = std::shared_ptr<UserBookSubscriber>;

template<int N>
void
UserEngine<N>::init_subscribers_() {

  const size_t nl = p_->n_; // Number of legs
  
  // User quotes associated with legs. 
  // No implied considerations
  for(size_t i=0; i<nl; ++i) {
      std::shared_ptr<UserQuoteSubscriber> UQSbid = std::make_shared<UserQuoteSubscriber>();
      std::shared_ptr<UserBookSubscriber>  UBSbid = std::make_shared<UserBookSubscriber>(&(p_->uQuote_[0][i]));
      UQSbid->attach_bid(UBSbid);
      (p_->quote_publishers_)[0][i].attach_bid(UQSbid);

      std::shared_ptr<UserQuoteSubscriber> UQSask = std::make_shared<UserQuoteSubscriber>();
      std::shared_ptr<UserBookSubscriber>  UBSask = std::make_shared<UserBookSubscriber>(&(p_->uQuote_[1][i]));
      UQSask->attach_ask(UBSask);
      (p_->quote_publishers_)[1][i].attach_ask(UQSask);
  }
}

template<int N>
inline void
UserEngine<N>::publish_bid(const SecPair& mkt, const QuotePublishEvent& pe) {
  if (mkt.leg1() < 0)
    (p_->quote_publishers_)[0][mkt.leg0()].nudge_bid(pe);
}

template<int N>
inline void
UserEngine<N>::publish_ask(const SecPair& mkt, const QuotePublishEvent &pe) {
  if (mkt.leg1() < 0)
    (p_->quote_publishers_)[1][mkt.leg0()].nudge_ask(pe);
}

template<int N>
void
UserEngine<N>::write_curve_(const std::array<std::array<std::pair<int, size_t>, N>, 2>& quote) const
  {
    int l = 0;
    std::for_each((quote[1]).begin(), (quote[1]).end(),
		  [&l](auto a){
		    std::string s("leg_");
		    s+=std::to_string(l++);
		    std::cout << std::setw(16) << s << " "; });
    std::cout << std::endl;
    
    l = 0;
    std::for_each((quote[1]).begin(), (quote[1]).end(),
		  [&l](auto a){std::cout << std::setw(15) << "================ "; });
    std::cout << std::endl;
    
    std::for_each(quote[1].begin(), quote[1].end(),
		  [](auto a) {
		    std::string s = std::to_string(a.first) + " x (" + std::to_string(a.second) + ")";
		    std::cout << std::setw(16) << s << " ";
		  });
    std::cout << std::endl;
    
    std::for_each(quote[0].begin(), quote[0].end(),
		  [](auto a) {
		    std::string s = std::to_string(a.first) + " x (" + std::to_string(a.second) + ")";
		    std::cout << std::setw(16) << s << " ";
		  });
    std::cout << std::endl;
    
    std::for_each((quote[1]).begin(), (quote[1]).end(),
		  [&l](auto a){std::cout << std::setw(15) << "================ "; });
    std::cout << std::endl;
  }


template<int N>
void
UserEngine<N>::write_user_curve() const
{
    std::cout << " : ===========\n";
    std::cout << " : USER PRICES\n";
    std::cout << " : ===========\n";
    write_curve_(p_->uQuote_);
}

template<int N>
void
UserEngine<N>::init_() {
  init_subscribers_();
}

#endif
