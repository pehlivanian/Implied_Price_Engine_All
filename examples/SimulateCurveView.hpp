#ifndef __SIMULATECURVEVIEW_HPP__
#define __SIMULATECURVEVIEW_HPP__

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>

#include "Client.hpp"
#include "QuoteServer.hpp"
#include "ImpliedServer.hpp"
#include "threadpool.hpp"


#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))

template<int N>
struct publish_wrapper {
    public:
        SecPair sec_pair_;
        QuotePublishEvent p_;
        ImpliedEngine<N> *IE_;

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
                IE_(IE) {}

};

template<int N>
struct bid_publisher_wrapper : public publish_wrapper<N> {
    bid_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) :
            publish_wrapper<N>(s, p, IE) {}
    void publish(SecPair sec_pair, const QuotePublishEvent&) { this->IE_->publish_bid(sec_pair, this->p_); }
};

template<int N>
struct ask_publisher_wrapper : public publish_wrapper<N> {
    ask_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) :
            publish_wrapper<N>(s, p, IE) {}
    void publish(SecPair sec_pair, const QuotePublishEvent&) { this->IE_->publish_ask(sec_pair, this->p_); }
};



#endif
