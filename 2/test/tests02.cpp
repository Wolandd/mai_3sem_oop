#include <gtest/gtest.h>
#include "../include/Twelve.h"

TEST(test_02, Defalut_constructor)
{
    Twelve num;
    
    EXPECT_EQ(num.to_decimal(), 0);
    
    Twelve other("A");
    Twelve sum = num.add(other);
    EXPECT_EQ(sum.to_decimal(), 10);

}

TEST(test_02, Size)
{
    Twelve num (3, 'A');
    EXPECT_EQ(num.to_decimal(), 1570);

}

TEST(test_02, String)
{
    Twelve num ("1AB");
    EXPECT_EQ(num.to_decimal(), 275);

}

TEST(test_02, list)
{
    Twelve num{'A', 'B', '1'};
    EXPECT_EQ(num.to_decimal(), 1573);

}

TEST(test_02, Plus)
{
    Twelve a("1A");
    Twelve b(2, '2');
    Twelve res = a.add(b);
    EXPECT_EQ(res.to_decimal(), 48);

}

TEST(test_02, Minus)
{
    Twelve a("27");
    Twelve b(2, '2');
    Twelve res = a.minus(b);
    EXPECT_EQ(res.to_decimal(), 5);

}

TEST(test_02, Minus_Error)
{
    Twelve a("5");
    Twelve b("A7");

    EXPECT_THROW(a.minus(b), std::underflow_error);

}
TEST(test_02, Copy) {
    Twelve original("123A");
    Twelve copy = original.clone();
    
    EXPECT_TRUE(original.is_equal(copy));
    EXPECT_EQ(original.to_decimal(), copy.to_decimal());
}

TEST(test_02, Equals) {
    Twelve a("AAA");
    Twelve b(3, 'A');
    Twelve c("ABA");
    
    EXPECT_TRUE(a.is_equal(b));
    EXPECT_FALSE(a.is_equal(c));
}

TEST(test_02, Bigger) {
    Twelve a("A"); 
    Twelve b("B"); 
    
    EXPECT_FALSE(a.is_greater(b));
    EXPECT_TRUE(b.is_greater(a));
}

TEST(test_02, Immutability) {
    Twelve original("123A");
    Twelve a = original.add(Twelve("1"));
    Twelve b = original.minus(Twelve("1"));
    
    // Оригинальный объект не изменился
    EXPECT_EQ(original.to_decimal(), 2062);
    EXPECT_TRUE(original.is_equal(Twelve("123A")));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
