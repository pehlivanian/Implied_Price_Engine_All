#include <gtest/gtest.h>

#include "SecPair.hpp"


TEST(SecPairTest, SumofTwo) {
    ASSERT_EQ(SecPair(1, 3, 1), SecPair(1, 2, 1) + SecPair(2, 3, 1));
    ASSERT_EQ(SecPair(1, 5, 1), SecPair(1, 3, 1) + SecPair(3, 5, 1));
    ASSERT_EQ(SecPair(1, 8, 1), SecPair(1, 2, 1) + SecPair(2, 8, 1));
    ASSERT_EQ(SecPair(1, 3, 1), SecPair(1, 2, 1) + SecPair(3, 2, -1));
}


TEST(SecPairTest, SumofThreeorMore) {
    ASSERT_EQ(SecPair(12,22,1),  SecPair(16,22,1) + SecPair(12,16,1));
    ASSERT_EQ(SecPair(3,6,1),    SecPair(3,4,1) + SecPair(4,5,1) + SecPair(5,6,1));
    ASSERT_EQ(SecPair(4,8,1),    SecPair(4,5,1) + SecPair(6,5,-1) + SecPair(7,6,-1) + SecPair(7,8,1));
    ASSERT_EQ(SecPair(6,10,1),   SecPair(7,10,1) + SecPair(7,6,-1));
}

TEST(SecPairTest, Normalization) {
    ASSERT_EQ( SecPair(1, 4, 1), SecPair(1, 4, 1));
    ASSERT_EQ( SecPair(4, 1, -1), SecPair(1, 4, 1));
    ASSERT_EQ( SecPair(4, 4, 2), SecPair( -1, -1, -1));
    ASSERT_EQ( SecPair( 4, 5, 1), SecPair(5, 4, -1));
    ASSERT_EQ( SecPair( 8, 4, 4), SecPair( 4, 8, -4));
    ASSERT_EQ( SecPair( -1, 3, 1), SecPair( 3, -1, -1));
    ASSERT_EQ( SecPair( -1, 4, -2), SecPair( 4, -5, 2));
    ASSERT_EQ( SecPair( 5, 10, 4), SecPair( 10, 5, -4));
    ASSERT_EQ( SecPair( 33, 44, 0), SecPair( 1, 14, 0));
    ASSERT_EQ( SecPair( 4, -1, -1), SecPair( -3, 4, 1));
}

 TEST(SecPairTest, DiffofTwo) {
    ASSERT_EQ(SecPair(8,9,1),    SecPair(8,9,1) - SecPair(-1,-1,-1));
    ASSERT_EQ(SecPair(-1,-1,1),  SecPair(8,9,1) - SecPair(9,8,-1));
    ASSERT_EQ(SecPair(10,12, 1), SecPair(10,12,1) - SecPair(-1,-1,1));
    ASSERT_EQ(SecPair(12,13,-1), SecPair(11,12,1) - SecPair(11,13,1));
    ASSERT_EQ(SecPair(16, 18, -1), SecPair(14, 16, 1) - SecPair(14, 18, 1));
    ASSERT_EQ(SecPair( 5, 7, 0), SecPair( 7, 10, 2) - SecPair( 7, 10, 2));
}

TEST(SecPairTest, Negation) {
    ASSERT_EQ( SecPair(8, 9, 1) * -1, SecPair( 8, 9, -1));
    ASSERT_EQ( SecPair(8, 9, 1) * -1, SecPair( 9, 8, 1));
    ASSERT_EQ( SecPair(-4, -5, -1) * -1, SecPair( -2, -22, -4));
    ASSERT_EQ( SecPair(5, 4, 1) * -1, SecPair( 4, 5, 1));
}

TEST(SecPairTest, Mult) {
    ASSERT_EQ(SecPair(5,8,1),   SecPair(8,5,1) * -1);
    ASSERT_EQ(SecPair(5,6,1),   SecPair(5,6,-1) * -1);
    ASSERT_EQ(SecPair(-1,-1,1), SecPair(-1,-1,1) * -1);
}

int main(int argc, char **argv) {
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
