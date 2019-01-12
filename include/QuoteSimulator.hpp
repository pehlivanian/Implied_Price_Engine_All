//
// Created by charles on 10/13/16.
//

#ifndef __QUOTESIMULATOR_HPP__
#define __QUOTESIMULATOR_HPP__

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <utility>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "SecPair.hpp"
#include "UserEngine.hpp"


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
            // Only needed to aid in sensible quote generation
	    UE_(new UserEngine<N>()),
            quotes_(num_quotes)
    {}
    void process() { create_quotes_(); }
    void dump();
    void dump_to_file(std::ofstream&);
  
  void attach( std::function<void(const rapidjson::Document &)>);
private:
    void create_quotes_();

    int num_quotes_;
    UserEngine<N> *UE_;
    std::vector<quote> quotes_;
};

// Do this with rapidjson eventually
std::ostream& operator<<(std::ostream& out, const quote& q)
{
    std::string type_str = "\"bid\"";
    if (q.t_ == quote::QUOTE_TYPE::Ask)
        type_str = "\"ask\"";
    std::string inst = "\"Spread_" + std::to_string((q.sp_).leg0()) + std::string("_") + std::to_string((q.sp_).leg1()) + "\"";
    if ((q.sp_).isLeg())
        inst = "\"Leg_" + std::to_string((q.sp_).leg0()) + "\"";

    out << "{ \"Inst\" : ";
    out << inst << ", ";
    out << type_str << " : "
        << q.p_ << ", \"size\" : "
        << q.s_;
    out << " }\n";
    return out;
}

#include "QuoteSimulator_impl.hpp"

#endif
