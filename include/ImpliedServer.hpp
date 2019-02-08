#ifndef __IMPLIEDSERVER_HPP__
#define __IMPLIEDSERVER_HPP__

#include <stdio.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <memory>
#include <regex>
#include <sstream>
#include <functional>
#include <exception>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "threadpool.hpp"
#include "report.hpp"
#include "problem.hpp"

#include "ImpliedEngine.hpp"
#include "ImpliedQuoteSubscriber.hpp"
#include "Client.hpp"
#include "SecPair.hpp"
#include "QuoteSimulator.hpp"
#include "impl.hpp"

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))

using Price_Size_Pair = std::pair<int, size_t>;

struct ImpliedServerException : public std::exception {
    const char *what() const noexcept override {
        return "Invalid Quote";
    }
};

template<int N>
class ImpliedServer;

template<int N>
class ImpliedServer
{
public:
    ImpliedServer() :
            p_(std::make_unique<impl<ImpliedServer<N>>>(true, false, true, 8008)) { init_(); }
    ImpliedServer(bool sim_batch_mode) :
            p_(std::make_unique<impl<ImpliedServer<N>>>(sim_batch_mode, false, true, 8008)) { init_(); }
    ImpliedServer(bool sim_batch_mode, bool sim_realtime_mode) :
            p_(std::make_unique<impl<ImpliedServer<N>>>(sim_batch_mode, sim_realtime_mode, true, 8008)) { init_(); }
    ImpliedServer(bool sim_batch_mode, bool sim_realtime_mode, bool sync_mode) :
      p_(std::make_unique<impl<ImpliedServer<N>>>(sim_batch_mode, sim_realtime_mode, sync_mode, 8008)) { init_(); }
    ImpliedServer(bool sim_batch_mode, bool sim_realtime_mode, int port) :
            p_(std::make_unique<impl<ImpliedServer<N>>>(sim_batch_mode, sim_realtime_mode, true, port)) { init_(); }
    ImpliedServer(bool sim_batch_mode, bool sim_realtime_mode, bool sync_mode, int port) :
            p_(std::make_unique<impl<ImpliedServer<N>>>(sim_batch_mode, sim_realtime_mode, sync_mode, port)) { init_(); }

    // Here come the delegators
    void publish_bid(const SecPair& sp, const QuotePublishEvent& e)  { (p_->IE_)->publish_bid(sp, e); }
    void publish_bid(const SecPair& sp, int pc, size_t sz)           { (p_->IE_)->publish_bid(sp, QUOTE(pc,sz)); }
    void publish_bid(int leg, const QuotePublishEvent& e)            { (p_->IE_)->publish_bid(SecPair(leg, -1, 1), e); }
    void publish_bid(int leg, int pc, size_t sz)                     { (p_->IE_)->publish_bid(SecPair(leg, -1, 1), QUOTE(pc,sz)); }
    void publish_bid(int leg0, int leg1, const QuotePublishEvent& e) { (p_->IE_)->publish_bid(SecPair(leg0, leg1, 1), e); }
    void publish_bid(int leg0, int leg1, int pc, size_t sz)          { (p_->IE_)->publish_bid(SecPair(leg0, leg1, 1), QUOTE(pc,sz)); }
    void publish_ask(const SecPair& sp, const QuotePublishEvent& e)  { (p_->IE_)->publish_ask(sp, e); }
    void publish_ask(const SecPair& sp, int pc, size_t sz)           { (p_->IE_)->publish_ask(sp, QUOTE(pc,sz)); }
    void publish_ask(int leg, const QuotePublishEvent& e)            { (p_->IE_)->publish_ask(SecPair(leg, -1, 1), e); }
    void publish_ask(int leg, int pc, size_t sz)                     { (p_->IE_)->publish_ask(SecPair(leg, -1, 1), QUOTE(pc,sz)); }
    void publish_ask(int leg0, int leg1, const QuotePublishEvent& e) { (p_->IE_)->publish_ask(SecPair(leg0, leg1, 1), e); }
    void publish_ask(int leg0, int leg1, int pc, size_t sz)          { (p_->IE_)->publish_ask(SecPair(leg0, leg1, 1), QUOTE(pc,sz)); }
    char* get_buf() const                          { return (p_->C_)->get_buf(); }
    void write_user_curve() const                  { (p_->IE_)->write_user_curve(); }
    void write_implied_curve() const               { (p_->IE_)->write_implied_curve(); }
    void write_merged_curve() const                { (p_->IE_)->write_merged_curve(); }
    int get_num_legs() const                       { return (p_->IE_)->get_num_legs(); }
    Price_Size_Pair get_bid(int leg) const         { return (p_->IE_)->get_bid(leg);}
    Price_Size_Pair get_user_bid(int leg) const    { return (p_->IE_)->get_user_bid(leg); }
    Price_Size_Pair get_implied_bid(int leg) const { return (p_->IE_)->get_implied_bid(leg); }
    Price_Size_Pair get_ask(int leg) const         { return (p_->IE_)->get_ask(leg); }
    Price_Size_Pair get_user_ask(int leg) const    { return (p_->IE_)->get_user_ask(leg); }
    Price_Size_Pair get_implied_ask(int leg) const { return (p_->IE_)->get_implied_ask(leg); }

    void process_profiled();
    void process();

private:
    void preload_tasks_();
    void process_tasks_();
    void profiled_process_tasks_();
    void quote_handler_(const rapidjson::Document &document);
    void init_();

    threadpool pool_;

    std::unique_ptr<impl<ImpliedServer<N>>> p_;
    std::vector<std::function<void()>> tasks_;   // Task queue used to preload for timing experiments
};

#undef QUOTE
#include "ImpliedServer_impl.hpp"

#endif
