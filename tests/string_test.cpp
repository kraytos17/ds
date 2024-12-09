#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "../src/string/my_string.hpp"

TEST(MyStringTest, DefaultConstructor) {
    MyString str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(std::string_view(str.begin(), str.size()), "");
}

TEST(MyStringTest, ConstructorWithSmallString) {
    const char* smallStr = "Hello";
    MyString str(std::span{smallStr, 5});
    EXPECT_EQ(str.size(), 5);
    EXPECT_EQ(std::string_view(str.begin(), str.size()), "Hello");
}

TEST(MyStringTest, ConstructorWithLargeString) {
    std::string largeStr(50, 'A');
    MyString str(std::span{largeStr.data(), largeStr.size()});
    EXPECT_EQ(str.size(), 50);
    EXPECT_EQ(std::string_view(str.begin(), str.size()), largeStr);
}

TEST(MyStringTest, CopyConstructor) {
    MyString original(std::span{"Hello"});
    MyString copy = original;
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(std::string_view(copy.begin(), copy.size()), "Hello");
}

TEST(MyStringTest, MoveConstructor) {
    MyString original(std::span{"Hello"});
    MyString moved = std::move(original);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.begin(), moved.size()), "Hello");
    EXPECT_EQ(original.size(), 0);
}

TEST(MyStringTest, CopyAssignment) {
    MyString original(std::span{"Hello"});
    MyString copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(std::string_view(copy.begin(), copy.size()), "Hello");
}

TEST(MyStringTest, MoveAssignment) {
    MyString original(std::span{"Hello"});
    MyString moved;
    moved = std::move(original);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.begin(), moved.size()), "Hello");
    EXPECT_EQ(original.size(), 0);
}

TEST(MyStringTest, Concatenation) {
    MyString str1(std::span{"Hello, "});
    MyString str2(std::span{"World!"});

    MyString result = str1 + str2;

    EXPECT_EQ(result.size(), 13);
    EXPECT_EQ(std::string_view(result.begin(), result.size()), "Hello, World!");
}

TEST(MyStringTest, OstreamOperator) {
    MyString str(std::span{"Hello, World!"});
    std::ostringstream os;
    os << str;

    EXPECT_EQ(os.str(), "Hello, World!");
}

TEST(MyStringTest, ComparisonOperator) {
    MyString str1(std::span{"Apple"});
    MyString str2(std::span{"Banana"});
    MyString str3(std::span{"Apple"});

    EXPECT_LT(str1, str2);
    EXPECT_EQ(str1, str3);
    EXPECT_GT(str2, str1);
}
