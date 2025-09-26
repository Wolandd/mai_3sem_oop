#include <gtest/gtest.h>
#include "gcd.h"

TEST(GCDTest, BasicPositiveNumbers) {
    EXPECT_EQ(gcd::gcd(12, 18), 6);
    EXPECT_EQ(gcd::gcd(18, 12), 6);
    EXPECT_EQ(gcd::gcd(17, 13), 1);
    EXPECT_EQ(gcd::gcd(100, 25), 25);
}

TEST(GCDTest, ZeroCases) {
    EXPECT_EQ(gcd::gcd(0, 5), 5);
    EXPECT_EQ(gcd::gcd(5, 0), 5);
    EXPECT_EQ(gcd::gcd(0, 0), 0);
}

TEST(GCDTest, NegativeNumbers) {
    EXPECT_EQ(gcd::gcd(-12, 18), 6);
    EXPECT_EQ(gcd::gcd(12, -18), 6);
    EXPECT_EQ(gcd::gcd(-12, -18), 6);
    EXPECT_EQ(gcd::gcd(-17, -13), 1);
}

TEST(GCDTest, SameNumbers) {
    EXPECT_EQ(gcd::gcd(15, 15), 15);
    EXPECT_EQ(gcd::gcd(1, 1), 1);
    EXPECT_EQ(gcd::gcd(0, 0), 0);
    EXPECT_EQ(gcd::gcd(-7, -7), 7);
}

TEST(GCDTest, LargeNumbers) {
    EXPECT_EQ(gcd::gcd(123456, 7890), 6);
    EXPECT_EQ(gcd::gcd(999999, 111111), 111111);
    EXPECT_EQ(gcd::gcd(1071, 462), 21);
}

TEST(GCDTest, PrimeNumbers) {
    EXPECT_EQ(gcd::gcd(17, 19), 1);
    EXPECT_EQ(gcd::gcd(13, 26), 13);
    EXPECT_EQ(gcd::gcd(7, 7), 7);
}