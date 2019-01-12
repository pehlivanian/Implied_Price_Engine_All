//
// Created by charles on 11/21/18.
//

#include <gtest/gtest.h>

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

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))

/* TEST(ImpliedClientTest, Connect2) {

    pid_t pid;
    int pipefd[2];

    // Form pipe for parent-child communication
    pipe(pipefd);

    // From parent, child processes
    pid = fork();

    if (pid > 0) {
        // Parent
        int port = 50001, port_assigned;

        QuoteServer *QS = new QuoteServer(port);
        port_assigned = QS->get_port();

        write(pipefd[1], &port_assigned, sizeof(port_assigned));
        close(pipefd[1]);

        // Poll on separate thread
        // auto t = std::thread(&QuoteServer::poll, QS, 1);
        // t.join();

        QS->poll(1);

    }
    else {

        // Child
        int port;

        read(pipefd[0], &port, sizeof(port));
        close(pipefd[0]);

        sleep(2);

        ImpliedServer<2> *IS = new ImpliedServer<2>(true, true, port);

        IS->process();

        // IS->write_user_curve();
        // IS->write_implied_curve();
        // IS->write_merged_curve();

        ASSERT_EQ(1, 1);

} */

TEST(ImpliedClientTest, ConnectSynch6) {

    using bid_p = bid_publisher_wrapper<6>;
    using ask_p = ask_publisher_wrapper<6>;

    pid_t pid;
    int pipefd[2];

    // Form pipe for parent-child communication
    pipe(pipefd);

    // From parent, child processes
    pid = fork();

    if (pid > 0) {
        // Parent
        int port = 50001, port_assigned;
        char filename[24];
        std::strcpy(filename, "serveme_6outrights.dat");

        QuoteServer *QS = new QuoteServer(filename, port);
        port_assigned = QS->get_port();

        write(pipefd[1], &port_assigned, sizeof(port_assigned));
        close(pipefd[1]);

        // Poll on separate thread
        // auto t = std::thread(&QuoteServer::poll, QS, 1);
        // t.join();

        QS->poll(1);

    }
    else {

        // Child
        int port;

        read(pipefd[0], &port, sizeof(port));
        close(pipefd[0]);

        sleep(2);

        // ImpliedServer<6> *IS = new ImpliedServer<6>(true, false, port);
        ImpliedServer<6> *IS = new ImpliedServer<6>(true, false, false, port);

        IS->process();

        IS->write_user_curve();
        IS->write_implied_curve();
        IS->write_merged_curve();

#define Q(A, B) std::make_pair((A), (unsigned long)(B))
        ASSERT_EQ( IS->get_user_bid(0), Q(4601, 2));
        ASSERT_EQ( IS->get_user_bid(1), Q(4599, 12));
        ASSERT_EQ( IS->get_user_bid(2), Q(4609, 7));
        ASSERT_EQ( IS->get_user_bid(3), Q(4616, 23));
        ASSERT_EQ( IS->get_user_bid(4), Q(4608, 11));
        ASSERT_EQ( IS->get_user_bid(5), Q(4621, 24));

        ASSERT_EQ( IS->get_user_ask(0), Q(4615, 2));
        ASSERT_EQ( IS->get_user_ask(1), Q(4613, 18));
        ASSERT_EQ( IS->get_user_ask(2), Q(4616, 22));
        ASSERT_EQ( IS->get_user_ask(3), Q(4623, 8));
        ASSERT_EQ( IS->get_user_ask(4), Q(4619, 4));
        ASSERT_EQ( IS->get_user_ask(5), Q(4630, 25));

        ASSERT_EQ( IS->get_implied_bid(0), Q(4603, 15));
        ASSERT_EQ( IS->get_implied_bid(1), Q(4602, 13));
        ASSERT_EQ( IS->get_implied_bid(2), Q(4607, 11));
        ASSERT_EQ( IS->get_implied_bid(3), Q(4614, 7));
        ASSERT_EQ( IS->get_implied_bid(4), Q(4608, 7));
        ASSERT_EQ( IS->get_implied_bid(5), Q(4618, 15));

        ASSERT_EQ( IS->get_implied_ask(0), Q(4613, 12));
        ASSERT_EQ( IS->get_implied_ask(1), Q(4610, 8));
        ASSERT_EQ( IS->get_implied_ask(2), Q(4618, 8));
        ASSERT_EQ( IS->get_implied_ask(3), Q(4625, 11));
        ASSERT_EQ( IS->get_implied_ask(4), Q(4619, 21));
        ASSERT_EQ( IS->get_implied_ask(5), Q(4633, 8));

#undef Q
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
