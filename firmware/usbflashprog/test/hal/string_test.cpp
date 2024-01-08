// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/hal/string_test.cpp
 * @brief Implementation of Unit Test for String Handling Helper Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <string>
#include "string_test.hpp"
#include "hal/string.hpp"

// ---------------------------------------------------------------------------

TEST_F(StringTest, upper) {
    EXPECT_EQ(StringUtils::upper(""), "");
    EXPECT_EQ(StringUtils::upper("123$ "), "123$ ");
    EXPECT_EQ(StringUtils::upper("abcd123$ "), "ABCD123$ ");
    EXPECT_EQ(StringUtils::upper("ABCD123$ "), "ABCD123$ ");
    EXPECT_EQ(StringUtils::upper("aBcD123$ "), "ABCD123$ ");
}

TEST_F(StringTest, lower) {
    EXPECT_EQ(StringUtils::lower(""), "");
    EXPECT_EQ(StringUtils::lower("123$ "), "123$ ");
    EXPECT_EQ(StringUtils::lower("ABCD123$ "), "abcd123$ ");
    EXPECT_EQ(StringUtils::lower("abcd123$ "), "abcd123$ ");
    EXPECT_EQ(StringUtils::lower("aBcD123$ "), "abcd123$ ");
}

TEST_F(StringTest, trim) {
    EXPECT_EQ(StringUtils::trim(""), "");
    EXPECT_EQ(StringUtils::trim("123$"), "123$");
    EXPECT_EQ(StringUtils::trim(" 123$"), "123$");
    EXPECT_EQ(StringUtils::trim("123$ "), "123$");
    EXPECT_EQ(StringUtils::trim(" 123$ "), "123$");
    EXPECT_EQ(StringUtils::trim("\n\t\r 123$ \n\t\r"), "123$");
    EXPECT_EQ(StringUtils::trim(" 1 23$ "), "1 23$");
    EXPECT_EQ(StringUtils::trim("\n\t\r  \n\t\r"), "");
}

TEST_F(StringTest, split) {
    EXPECT_EQ(StringUtils::split("", ','), StringUtils::TStringVector({}));
    EXPECT_EQ(StringUtils::split(",", ','), StringUtils::TStringVector({}));
    EXPECT_EQ(StringUtils::split("a", ','),
              StringUtils::TStringVector({std::string("a")}));
    EXPECT_EQ(StringUtils::split("a,", ','),
              StringUtils::TStringVector({std::string("a")}));
    EXPECT_EQ(StringUtils::split("a,b", ','),
              StringUtils::TStringVector({std::string("a"), std::string("b")}));
    EXPECT_EQ(StringUtils::split("a,b,", ','),
              StringUtils::TStringVector({std::string("a"), std::string("b")}));
    EXPECT_EQ(
        StringUtils::split("a b,c", ','),
        StringUtils::TStringVector({std::string("a b"), std::string("c")}));
    EXPECT_EQ(
        StringUtils::split("a b,c", ' '),
        StringUtils::TStringVector({std::string("a"), std::string("b,c")}));
    EXPECT_EQ(
        StringUtils::split("a b , c", ','),
        StringUtils::TStringVector({std::string("a b "), std::string(" c")}));
    EXPECT_EQ(
        StringUtils::split("a b , c", ',', true),
        StringUtils::TStringVector({std::string("a b"), std::string("c")}));
}

TEST_F(StringTest, toInt) {
    EXPECT_EQ(StringUtils::toInt(""), 0);
    EXPECT_EQ(StringUtils::toInt("abc"), 0);
    EXPECT_EQ(StringUtils::toInt("abc", 16), 2748);
    EXPECT_EQ(StringUtils::toInt("21"), 21);
    EXPECT_EQ(StringUtils::toInt("-21"), -21);
    EXPECT_EQ(StringUtils::toInt("21.5"), 21);
    EXPECT_EQ(StringUtils::toInt("-21.5"), -21);
    EXPECT_EQ(StringUtils::toInt("21abc"), 21);
    EXPECT_EQ(StringUtils::toInt("9", 8), 0);
    EXPECT_EQ(StringUtils::toInt("44", 8), 36);
    EXPECT_EQ(StringUtils::toInt("1010", 2), 10);
}

TEST_F(StringTest, toFloat) {
    EXPECT_EQ(StringUtils::toFloat(""), 0.0f);
    EXPECT_EQ(StringUtils::toFloat("abc"), 0.0f);
    EXPECT_EQ(StringUtils::toFloat("21"), 21.0f);
    EXPECT_EQ(StringUtils::toFloat("-21"), -21.0f);
    EXPECT_EQ(StringUtils::toFloat("21.5"), 21.5f);
    EXPECT_EQ(StringUtils::toFloat("-21.5"), -21.5f);
    EXPECT_EQ(StringUtils::toFloat("21abc"), 21.0f);
    EXPECT_EQ(StringUtils::toFloat("21e2"), 2100.0f);
    EXPECT_EQ(StringUtils::toFloat("21e-2"), 0.21f);
}

TEST_F(StringTest, fromInt) {
    EXPECT_EQ(StringUtils::fromInt(0), "0");
    EXPECT_EQ(StringUtils::fromInt(0, 2), "0");
    EXPECT_EQ(StringUtils::fromInt(0, 16), "0");
    EXPECT_EQ(StringUtils::fromInt(2748, 16), "ABC");
    EXPECT_EQ(StringUtils::fromInt(21), "21");
    EXPECT_EQ(StringUtils::fromInt(-21), "-21");
    EXPECT_EQ(StringUtils::fromInt(9, 8), "11");
    EXPECT_EQ(StringUtils::fromInt(36, 8), "44");
    EXPECT_EQ(StringUtils::fromInt(10, 2), "1010");

    EXPECT_EQ(StringUtils::fromInt(0, 10, 4), "   0");
    EXPECT_EQ(StringUtils::fromInt(0, 2, 4), "   0");
    EXPECT_EQ(StringUtils::fromInt(0, 16, 4), "   0");
    EXPECT_EQ(StringUtils::fromInt(2748, 16, 4), " ABC");
    EXPECT_EQ(StringUtils::fromInt(21, 10, 4), "  21");
    EXPECT_EQ(StringUtils::fromInt(-21, 10, 4), " -21");
    EXPECT_EQ(StringUtils::fromInt(9, 8, 4), "  11");
    EXPECT_EQ(StringUtils::fromInt(36, 8, 4), "  44");
    EXPECT_EQ(StringUtils::fromInt(10, 2, 8), "    1010");

    EXPECT_EQ(StringUtils::fromInt(0, 10, 4, '0'), "0000");
    EXPECT_EQ(StringUtils::fromInt(0, 2, 4, '0'), "0000");
    EXPECT_EQ(StringUtils::fromInt(0, 16, 4, '0'), "0000");
    EXPECT_EQ(StringUtils::fromInt(2748, 16, 4, '0'), "0ABC");
    EXPECT_EQ(StringUtils::fromInt(21, 10, 4, '0'), "0021");
    EXPECT_EQ(StringUtils::fromInt(-21, 10, 4, '0'), "0-21");
    EXPECT_EQ(StringUtils::fromInt(9, 8, 4, '0'), "0011");
    EXPECT_EQ(StringUtils::fromInt(36, 8, 4, '0'), "0044");
    EXPECT_EQ(StringUtils::fromInt(10, 2, 8, '0'), "00001010");

    EXPECT_EQ(StringUtils::fromInt(10, 1), "10");
}

TEST_F(StringTest, fromFloat) {
    EXPECT_EQ(StringUtils::fromFloat(0.0f), "0.000");
    EXPECT_EQ(StringUtils::fromFloat(0.0f, 3), "0.000");
    EXPECT_EQ(StringUtils::fromFloat(0.0f, 1), "0.0");
    EXPECT_EQ(StringUtils::fromFloat(0.2145f), "0.214");
    EXPECT_EQ(StringUtils::fromFloat(0.2145f, 3), "0.214");
    EXPECT_EQ(StringUtils::fromFloat(0.2145f, 2), "0.21");
    EXPECT_EQ(StringUtils::fromFloat(0.2145f, 1), "0.2");

    EXPECT_EQ(StringUtils::fromFloat(-8.2145f), "-8.215");
    EXPECT_EQ(StringUtils::fromFloat(-8.2145f, 3), "-8.215");
    EXPECT_EQ(StringUtils::fromFloat(-8.2145f, 2), "-8.21");
    EXPECT_EQ(StringUtils::fromFloat(-8.2145f, 1), "-8.2");

    EXPECT_EQ(StringUtils::fromFloat(10.2145f), "10.215");
    EXPECT_EQ(StringUtils::fromFloat(10.2145f, 3), "10.215");
    EXPECT_EQ(StringUtils::fromFloat(10.2145f, 2), "10.21");
    EXPECT_EQ(StringUtils::fromFloat(10.2145f, 1), "10.2");
}
