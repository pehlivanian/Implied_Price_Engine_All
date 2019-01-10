//
// Created by charles on 1/9/19.
//


#include <gtest/gtest.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>


#include "QuotePublisher.hpp"
#include "ImpliedEngine.hpp"

TEST(ImpliedEngine, Analytics) {

    ImpliedEngine<6> *IE_ = new ImpliedEngine<6>();

    IE_->publish_bid( SecPair( 0, -1, 1), QuotePublishEvent(std::make_pair(5501, 22)));

    ASSERT_EQ(1, 1);

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
