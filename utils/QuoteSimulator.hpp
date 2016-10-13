//
// Created by charles on 10/13/16.
//

#ifndef __QUOTESIMULATOR_HPP__
#define __QUOTESIMULATOR_HPP__

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include "SecPair.hpp"
#include "ImpliedServer.hpp"

struct quote
{

    enum QUOTE_TYPE { Bid, Ask, N};

    quote() = default;
    quote(QUOTE_TYPE t, const SecPair& sp, int p, size_t s) :
            t_(t),
            sp_(sp),
            p_(p),
            s_(s) {}

    QUOTE_TYPE t_;
    SecPair sp_;
    int p_;
    size_t s_;
};

template<int N>
class QuoteSimulator
{
public:
    QuoteSimulator(int num_quotes=1000) :
            num_quotes_(num_quotes),
            IS_(ImpliedServer<N>(false)),
            quotes_(num_quotes)
    {}
    void process() { create_quotes_(); }
private:
    void create_quotes_();

    int num_quotes_;
    ImpliedServer<N> IS_;
    std::vector<quote> quotes_;
};

// Do this with rapidjson eventually
std::ostream& operator<<(std::ostream& out, const quote& q)
{
    std::string type_str = "\"bid\"";
    if (q.t_ == quote::QUOTE_TYPE::Ask)
        type_str = "\"ask\"";
    std::string inst = "\"Spread" + std::to_string((q.sp_).leg0()) + std::to_string((q.sp_).leg1()) + "\"";
    if ((q.sp_).isLeg())
        inst = "\"Leg" + std::to_string((q.sp_).leg0()) + "\"";

    out << "{ \"Inst\" : ";
    out << inst << ", ";
    out << type_str << " : "
        << q.p_ << ", \"size\" : "
        << q.s_;
    out << " }\n";
    return out;
}

int move_bid_price(int bid, int ask, int move_type)
{
    int r;
    switch(move_type)
    {
        case (0) :
            r = std::min(bid+1, ask-1);
            break;
        case (1) :
            r = std::min(bid+2, ask-1);
            break;
        case (2) :
            r = std::min(bid+1, ask-1);
            // r = std::min(static_cast<int>(.5*(bid + ask)), ask-1);
            break;
    }
    return r;
}

int move_ask_price(int bid, int ask, int move_type)
{
    int r;
    switch(move_type)
    {
        case (0) :
            r = std::max(ask-1,bid+1);
            break;
        case (1) :
            r = std::max(ask-2,bid+1);
            break;
        case (2) :
            r = std::max(ask-1,bid+1);
            // r = std::max(static_cast<int>(.5*(bid + ask + 2)), bid+1);
            break;
    }
    return r;
}

template<int N>
void
QuoteSimulator<N>::create_quotes_()
{
    int quote_count = 0;
    int num_legs = IS_.get_num_legs();

    std::mt19937 gen;
    gen.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist_leg(0,num_legs-1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_bid_ask(0,1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_inst_type(0,2);
    std::uniform_int_distribution<std::mt19937::result_type> dist_price_move_type(0,2);

    for(size_t i=0; i<num_legs; ++i) {
        quote q = quote(quote::QUOTE_TYPE::Bid, SecPair(i, -1, 1), 4604 + (i * 100), 10);
        quotes_[quote_count++] = q;
        IS_.publish_bid(q.sp_, q.p_, q.s_);
    }
    for(size_t i=0; i<num_legs; ++i) {
        quote q = quote(quote::QUOTE_TYPE::Ask, SecPair(i, -1, 1), 4614 + (i * 100), 10);
        quotes_[quote_count++] = q;
        IS_.publish_ask(q.sp_, q.p_, q.s_);
    }

    #define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
    while(quote_count < num_quotes_)
    {
        int leg_num = dist_leg(gen);
        int bid_ask = dist_bid_ask(gen);
        int inst_type = dist_inst_type(gen);
        int price_move_type = dist_price_move_type(gen);

        int curr_price;
        int curr_bid0 = IS_.get_user_bid(leg_num).first;
        int curr_ask0 = IS_.get_user_ask(leg_num).first;
        quote::QUOTE_TYPE t = (quote::QUOTE_TYPE)(bid_ask);
        if ((inst_type == 0) || (leg_num == num_legs-1)) {
            switch (t) {
                case quote::QUOTE_TYPE::Bid :
                    curr_price = move_bid_price(curr_bid0, curr_ask0, price_move_type);
                    IS_.publish_bid(SecPair(leg_num, -1, 1), QUOTE(curr_price, 11));
                    break;
                case quote::QUOTE_TYPE::Ask :
                    curr_price = move_ask_price(curr_bid0, curr_ask0, price_move_type);
                    IS_.publish_ask(SecPair(leg_num, -1, 1), QUOTE(curr_price, 11));
                    break;
            }
//            std::cerr << "DEBUG: " << SecPair(leg_num, -1, 1) << " : "
//                      << (quote::QUOTE_TYPE(bid_ask)) << " : "
//                      << curr_bid0 << " : " << curr_ask0 << " : "
//                      << move_bid_price << " ::: " << curr_price << "\n";
            quotes_[quote_count++] = quote(t, SecPair(leg_num, -1, 1), curr_price, 11);
        }
        else
        {
            std::uniform_int_distribution<std::mt19937::result_type> dist_leg1(leg_num+1,num_legs-1);
            int leg1_num = dist_leg1(gen);
            int curr_bid1 = IS_.get_user_bid(leg1_num).first;
            int curr_ask1 = IS_.get_user_ask(leg1_num).first;
            switch (t) {
                case quote::QUOTE_TYPE::Bid :
                    curr_price = move_bid_price(curr_bid0-curr_ask1, curr_ask0-curr_bid1, price_move_type);
                    IS_.publish_bid(SecPair(leg_num, leg1_num, 1), QUOTE(curr_price, 11));
                    break;
                case quote::QUOTE_TYPE::Ask :
                    curr_price = move_ask_price(curr_bid0-curr_ask1, curr_ask0-curr_bid1, price_move_type);
                    IS_.publish_ask(SecPair(leg_num, leg1_num, 1), QUOTE(curr_price, 11));
                    break;
            }
//            std::cout << "DEBUG: " << SecPair(leg_num, leg1_num, 1) << " : "
//                      << (quote::QUOTE_TYPE(bid_ask)) << " : "
//                      << curr_bid0-curr_ask1 << " : " << curr_ask0-curr_bid1 << " : "
//                      << curr_price << "\n";
            quotes_[quote_count++] = quote(t, SecPair(leg_num, leg1_num, 1), curr_price, 11);
        }
    }
#undef QUOTE

    std::for_each(quotes_.begin(), quotes_.end(), [](const quote& q){ std::cout << q; });
    std::cout << std::endl;
}


#if 0
// Refactor this with rapidjson eventually
template<int N>
void
QuoteSimulator<N>::serialize_json() const
{
    std::fstream fs;
    fs.open(filename_, "w");

    for(auto& quote : quotes_)
        fs << quote;
}
#endif

#endif
