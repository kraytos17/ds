#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "../src/string/my_string.hpp"

class MyStringTest : public testing::Test {
protected:
    String defaultStr;
    String emptyStr;
    String singleCharStr{"A"};
    String smallStr{"Hello"};
    String ssoStr{"1234567890123456789012345678901"};
    String largeStr{
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"};
};

TEST_F(MyStringTest, DefaultConstructor) {
    EXPECT_EQ(defaultStr.size(), 0);
    EXPECT_EQ(std::string_view(defaultStr.data(), defaultStr.size()), "");
}

TEST_F(MyStringTest, ConstructorWithEmptyString) {
    String str("");
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(std::string_view(str.data(), str.size()), "");
}

TEST_F(MyStringTest, ConstructorWithSingleCharacter) {
    EXPECT_EQ(singleCharStr.size(), 1);
    EXPECT_EQ(std::string_view(singleCharStr.data(), singleCharStr.size()), "A");
}

TEST_F(MyStringTest, ConstructorWithSmallString) {
    EXPECT_EQ(smallStr.size(), 5);
    EXPECT_EQ(std::string_view(smallStr.data(), smallStr.size()), "Hello");
}

TEST_F(MyStringTest, ConstructorWithSSOString) {
    EXPECT_EQ(ssoStr.size(), 31);
    EXPECT_EQ(std::string_view(ssoStr.data(), ssoStr.size()), "1234567890123456789012345678901");
}

TEST_F(MyStringTest, ConstructorWithLargeString) {
    EXPECT_EQ(largeStr.size(), 100);
    EXPECT_EQ(std::string_view(largeStr.data(), largeStr.size()), std::string(100, 'A'));
}

TEST_F(MyStringTest, CopyConstructor) {
    String copy = smallStr;
    EXPECT_EQ(copy.size(), smallStr.size());
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Hello");
}

TEST_F(MyStringTest, MoveConstructor) {
    String moved = std::move(smallStr);
    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Hello");
}

TEST_F(MyStringTest, CopyAssignment) {
    String copy;
    copy = smallStr;
    EXPECT_EQ(copy.size(), smallStr.size());
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Hello");
}

TEST_F(MyStringTest, MoveAssignment) {
    String moved;
    moved = std::move(smallStr);
    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Hello");
}

TEST_F(MyStringTest, Concatenation) {
    String str1("Hello, ");
    String str2("World!");
    String result = str1 + str2;

    EXPECT_EQ(result.size(), 13);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "Hello, World!");
}

TEST_F(MyStringTest, ConcatenationWithEmptyString) {
    String str1("Hello");
    String str2("");
    String result = str1 + str2;

    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "Hello");
}

TEST_F(MyStringTest, ConcatenationEmptyStrings) {
    String str1("");
    String str2("");
    String result = str1 + str2;

    EXPECT_EQ(result.size(), 0);
    EXPECT_EQ(std::string_view(result.data(), result.size()), "");
}

TEST_F(MyStringTest, OstreamOperator) {
    std::ostringstream os;
    os << smallStr;
    EXPECT_EQ(os.str(), "Hello");
}

TEST_F(MyStringTest, ComparisonOperator) {
    String str1("Apple");
    String str2("Banana");
    String str3("Apple");

    EXPECT_LT(str1, str2);
    EXPECT_EQ(str1, str3);
    EXPECT_GT(str2, str1);
}

TEST_F(MyStringTest, ComparisonWithEmptyString) {
    String str1("Apple");
    String str2("");
    EXPECT_GT(str1, str2);
    EXPECT_NE(str1, str2);
    EXPECT_LT(str2, str1);
}

TEST_F(MyStringTest, MoveSemantics) {
    String original("Move this!");
    String moved = std::move(original);

    EXPECT_EQ(std::string_view(moved.data(), moved.size()), "Move this!");
    EXPECT_EQ(original.size(), 0);
}

TEST_F(MyStringTest, CopySemantics) {
    String original("Copy this!");
    String copy = original;

    EXPECT_EQ(copy.size(), 10);
    EXPECT_EQ(std::string_view(copy.data(), copy.size()), "Copy this!");
    EXPECT_EQ(original.size(), 10);
}

TEST_F(MyStringTest, LongString) {
    std::string longStr(1000, 'A');
    String str(longStr);

    EXPECT_EQ(str.size(), 1000);
    EXPECT_EQ(std::string_view(str.data(), str.size()), longStr);
}

TEST_F(MyStringTest, ExactSSOEdgeCase) {
    const char* exactSSO = "1234567890123456789012345678901";
    String str(exactSSO);
    EXPECT_EQ(str.size(), 31);
    EXPECT_EQ(std::string_view(str.data(), str.size()), exactSSO);
}

TEST_F(MyStringTest, LargeStringBeyondSSO) {
    const char* beyondSSO = "1234567890123456789012345678901234567890";
    String str(beyondSSO);
    EXPECT_EQ(str.size(), 40);
    EXPECT_EQ(std::string_view(str.data(), str.size()), beyondSSO);
}
