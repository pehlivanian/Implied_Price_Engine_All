#ifndef __IMPLIEDSERVER_HPP__
#define __IMPLIEDSERVER_HPP__

#include <memory>
#include <regex>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "ImpliedEngine.hpp"
#include "Client.hpp"
#include "SecPair.hpp"
#include "threadpool.hpp"
#include "impl.hpp"

template<int N>
class ImpliedServer;

template<int N>
class ImpliedServer
{
public:

    ImpliedServer() :
            p_(std::make_unique<impl<ImpliedServer<N>>>()) { init_(); }
    void process();

    // Lots of delegators
    char* get_buf() const            { return (p_->C_)->get_buf(); }
    void write_user_curve() const    { (p_->IE_)->write_user_curve(); }
    void write_implied_curve() const { (p_->IE_)->write_implied_curve(); }
    void write_merged_curve() const  { (p_->IE_)->write_merged_curve(); }

private:
    void init_();
    std::unique_ptr<impl<ImpliedServer>> p_;
};

#include "ImpliedServer_impl.hpp"

#endif