//
// Created by charles on 11/21/18.
//

#include <gtest/gtest.h>

#include <string>
#include <regex>
#include <vector>
#include <iterator>
#include <future>

#include "ImpliedEngine.hpp"
#include "threadpool.hpp"

template<int N>
struct publish_wrapper {
public:

    SecPair sec_pair_;
    QuotePublishEvent p_;
    ImpliedEngine<N>* IE_;

    publish_wrapper() = default;

    int operator()() {
        publish(sec_pair_, p_);
        return 0;
    }

    virtual void publish(SecPair, const QuotePublishEvent&) = 0;

protected:
    publish_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) :
            sec_pair_(s),
            p_(p),
            IE_(IE)
    {}
};

template<int N>
struct bid_publisher_wrapper : public publish_wrapper<N> {

    bid_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) :
            publish_wrapper<N>( s, p, IE)
    {}

    void publish(SecPair sec_pair_, const QuotePublishEvent&) { this->IE_->publish_bid(sec_pair_, this->p_); }
};

template<int N>
struct ask_publisher_wrapper : public publish_wrapper<N> {
    ask_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) :
            publish_wrapper<N>( s, p, IE )
    {}

    void publish(SecPair sec_pair_, const QuotePublishEvent&) { this->IE_->publish_ask(sec_pair_, this->p_); }
};

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))

TEST(Publishers, PublishTest) {

    using IE6 = ImpliedEngine<6>;
    using bid_p = bid_publisher_wrapper<6>;
    using ask_p = ask_publisher_wrapper<6>;

    std::vector<std::future<int>> bid_futures(6);
    std::vector<std::future<int>> ask_futures(6);
    std::vector<int> bid_results(6);
    std::vector<int> ask_results(6);

    IE6* IE = new IE6;
    threadpool pool;

    for(size_t i=0; i< 6; ++i) {
        auto leg   = SecPair(i, -1, 1);
        auto quote = QUOTE(4504 + i*100, 2);
        bid_results[i] = pool.submit(bid_p(leg, quote, IE)).get();
    }

    for(size_t i=0; i<6; ++i)  {
        auto leg   = SecPair(i, -1, 1);
        auto quote = QUOTE(4514 + i*100, 3);
        ask_results[i] = pool.submit(ask_p(leg, quote, IE)).get();
    }

    int r1 = pool.submit(ask_p( SecPair(1, -1, 1), QUOTE(4610, 17), IE)).get();
    int r2 = pool.submit(ask_p( SecPair(0, 3,  1), QUOTE(-300, 17), IE)).get();
    int r3 = pool.submit(bid_p( SecPair(1, 3, 1),  QUOTE(-200, 2), IE)).get();

    int r4 = pool.submit(bid_p( SecPair(0, 2, 1),  QUOTE(-200, 17), IE)).get();
    int r5 = pool.submit(bid_p( SecPair(2, 3,  1), QUOTE(-100, 17), IE)).get();
    int r6 = pool.submit(bid_p( SecPair(3, -1, 1), QUOTE(4805, 17), IE)).get();

    // IE->write_user_curve();
    // IE->write_implied_curve();
    // IE->write_merged_curve();

    ASSERT_EQ( IE->get_bid(0).first, 4504);
    ASSERT_EQ( IE->get_ask(0).first, 4510);
    ASSERT_EQ( IE->get_bid(1).first, 4605);
    ASSERT_EQ( IE->get_ask(1).first, 4610);
    ASSERT_EQ( IE->get_bid(2).first, 4705);
    ASSERT_EQ( IE->get_ask(2).first, 4710);

    ASSERT_LE( IE->get_user_bid(0).first, IE->get_bid(0).first);
    ASSERT_GE( IE->get_user_ask(0).first, IE->get_ask(0).first);
    ASSERT_LE( IE->get_user_bid(1).first, IE->get_bid(1).first);
    ASSERT_GE( IE->get_user_ask(1).first, IE->get_ask(1).first);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
