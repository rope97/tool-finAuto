#include <catch2/catch.hpp>
#include "lib/RegexDriver.hpp"

TEST_CASE("RegexDriver::Parse - input: Empty String  - return: if (string is valid regex) then unique_ptr<ast> else nullptr")
{

    SECTION("Empty string - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"(((())))\") - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((())))";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"$\") - just epsilon - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "$";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"(((($))))\") - just epsilon - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((($))))";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"$$$$$$\") - just epsilons - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "$$$$$$";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"$*$+$?$|$\") - epsilons with operators - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "$*$+$?$|$";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }
}

TEST_CASE("RegexDriver::Parse - input: Strings with Brackets  - return: if (string is valid regex) then unique_ptr<ast> else nullptr")
{
    SECTION("String: (\"z(a(b(c(d)e)f)g)m\") - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "z(a(b(c(d)e)f)g)m";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"(((()))))\") - unmatched brackets - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((()))))";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"((a|b)c?d*)a)\") - unmatched brackets - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "((a|b)c?d*)a)";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\")a?l?e?k?s?a?(\") - unmatched brackets - not valid regex")
    {
        RegexDriver rd;
        std::string regex = ")a?l?e?k?s?a?(";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }


    SECTION("String: (\"(((a|b)*)(c?d+))?\") - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((a|b)*)(c?d+))?";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"(((a|b)*)(c?d+))?\") - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((a|b)*)(c?d+))?";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }
}

TEST_CASE("RegexDriver::Parse - input: Operators - return: if (string is valid regex) then unique_ptr<ast> else nullptr")
{
    SECTION("String: (\"|a*\") - binary operator not matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "|a*";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"|a|\") - binary operator not matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "|a|";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"a|b|c|d\") - binary operator matched - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "a|b|c|d";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"*a\") - unary operator not matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "*a";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"?+ab\") - unary operator not matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "?+ab";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"((a?)+)*\") - unary operator matched - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "((a?)+)*";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"a?+*\") - unary operator matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "a?+*";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }

    SECTION("String: (\"(a?+*|b?+*)*\") - unary operator matched - is valid regex")
    {
        RegexDriver rd;
        std::string regex = "(((a?)+)*|((b?)+)*)*";

        auto astPtr = rd.Parse(regex);

        REQUIRE_FALSE(astPtr == nullptr);
    }

    SECTION("String: (\"(a?+*||b?+*)*\") - binary operator not matched - not valid regex")
    {
        RegexDriver rd;
        std::string regex = "(a?+*|b?+*)*";

        auto astPtr = rd.Parse(regex);

        REQUIRE(astPtr == nullptr);
    }
}
