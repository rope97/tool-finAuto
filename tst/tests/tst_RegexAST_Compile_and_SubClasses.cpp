#include <catch2/catch.hpp>
#include "lib/RegexAST.hpp"
#include "lib/RegexDriver.hpp"

TEST_CASE("RegexAST::Compile - Thompson's construction building box - Expected Results done on paper")
{
    SECTION("RegexAST::Compile( \"a\" ) - basic regex")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("a");
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        std::pair e0 {startState,'a'};
        expTransitions[e0] = {1};
        unsigned expEndState = 1;

        auto [resTransitions, resEndState] = ast->Compile(startState);

        REQUIRE(resTransitions == expTransitions);
        REQUIRE(resEndState == expEndState);
    }

    SECTION("RegexAST::Compile( \"(a|b)+\" )")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("(a|b)+");
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{0,RegexDriver::epsilon}] = {1};
        expTransitions[{1,RegexDriver::epsilon}] = {2,4};
        expTransitions[{2,'a'}] = {3};
        expTransitions[{4,'b'}] = {5};
        expTransitions[{3,RegexDriver::epsilon}] = {6};
        expTransitions[{5,RegexDriver::epsilon}] = {6};
        expTransitions[{6,RegexDriver::epsilon}] = {1,7};
        unsigned expEndState = 7;

        auto [resMap, resEndState] = ast->Compile(startState);

        REQUIRE(expTransitions == resMap);
        REQUIRE(expEndState == resEndState);
    }

    SECTION("RegexAST::Compile( \"(a|b)+c*\" )")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("(a|b)+c*");
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{0,RegexDriver::epsilon}] = {1};
        expTransitions[{1,RegexDriver::epsilon}] = {2,4};
        expTransitions[{2,'a'}] = {3};
        expTransitions[{3,RegexDriver::epsilon}] = {6};
        expTransitions[{4,'b'}] = {5};
        expTransitions[{5,RegexDriver::epsilon}] = {6};
        expTransitions[{6,RegexDriver::epsilon}] = {7,1};
        expTransitions[{7,RegexDriver::epsilon}] = {8,10};
        expTransitions[{8,'c'}] = {9};
        expTransitions[{9,RegexDriver::epsilon}] = {8,10};
        unsigned expEndState = 10;

        auto [resMap, resEndState] = ast->Compile(startState);

        REQUIRE(expTransitions == resMap);
        REQUIRE(expEndState == resEndState);
    }

    SECTION("RegexAST::Compile( \"(ab|c)*b\" ) - PPJ klk 2019 grp A")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("(ab|c)*b");
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{0,RegexDriver::epsilon}] = {1,8};
        expTransitions[{1,RegexDriver::epsilon}] = {2,5};
        expTransitions[{2,'a'}] = {3};
        expTransitions[{3,'b'}] = {4};
        expTransitions[{4,RegexDriver::epsilon}] = {7};
        expTransitions[{5,'c'}] = {6};
        expTransitions[{6,RegexDriver::epsilon}] = {7};
        expTransitions[{7,RegexDriver::epsilon}] = {8,1};
        expTransitions[{8,'b'}] = {9};
        unsigned expEndState = 9;

        auto [resMap, resEndState] = ast->Compile(startState);

        REQUIRE(expTransitions.size() == resMap.size());
        REQUIRE(expEndState == resEndState);
    }

    SECTION("RegexAST::Compile( \"a?b+abc(a|b|c)*\" ) - with every subclass")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("a?b+abc(a|b|c)*");
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{0,RegexDriver::epsilon}] = {1,3};
        expTransitions[{1,'a'}] = {2};
        expTransitions[{2,RegexDriver::epsilon}] = {3};
        expTransitions[{3,RegexDriver::epsilon}] = {4};
        expTransitions[{4,'b'}] = {5};
        expTransitions[{5,RegexDriver::epsilon}] = {6,4};
        expTransitions[{6,'a'}] = {7};
        expTransitions[{7,'b'}] = {8};
        expTransitions[{8,'c'}] = {9};
        expTransitions[{9,RegexDriver::epsilon}] = {10,20};
        expTransitions[{10,RegexDriver::epsilon}] = {11,17};
        expTransitions[{11,RegexDriver::epsilon}] = {12,14};
        expTransitions[{12,'a'}] = {13};
        expTransitions[{13,RegexDriver::epsilon}] = {16};
        expTransitions[{14,'b'}] = {15};
        expTransitions[{15,RegexDriver::epsilon}] = {16};
        expTransitions[{16,RegexDriver::epsilon}] = {19};
        expTransitions[{17,'c'}] = {18};
        expTransitions[{18,RegexDriver::epsilon}] = {19};
        expTransitions[{19,RegexDriver::epsilon}] = {20,10};
        unsigned expEndState = 20;

        auto [resMap, resEndState] = ast->Compile(startState);

        REQUIRE(expTransitions.size() == resMap.size());
        REQUIRE(expEndState == resEndState);
    }
}

TEST_CASE("SubclassesOfRegexAST::Compile - Thompson's construction building box - Expected Results done on paper")
{
    SECTION("AlternationAST::Compile(startState) - regex:\"a|b\" -  AlternationAST(move(ast(a)), move(ast(b))")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast1 = rd.Parse("a");
        std::unique_ptr<RegexAST> ast2 = rd.Parse("b");
        auto altAST = AlternationAST(std::move(ast1), std::move(ast2));
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{1,'a'}] = {2};
        expTransitions[{3,'b'}] = {4};
        expTransitions[{0,RegexDriver::epsilon}] = {1,3};
        expTransitions[{2,RegexDriver::epsilon}] = {5};
        expTransitions[{4,RegexDriver::epsilon}] = {5};
        unsigned expEndState = 5;

        auto [resTransitions, resEndState] = altAST.Compile(startState);

        REQUIRE(resTransitions == expTransitions);
        REQUIRE(resEndState == expEndState);
    }

    SECTION("ZeroOrMoreAST::Compile(startState) - regex:\"a*\" - ZeroOrMoreAST(move(ast(a)))")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("a");
        auto zomAST = ZeroOrMoreAST(std::move(ast));
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{1,'a'}] = {2};
        expTransitions[{0,RegexDriver::epsilon}] = {1,3};
        expTransitions[{2,RegexDriver::epsilon}] = {1,3};
        unsigned expEndState = 3;

        auto [resTransitions, resEndState] = zomAST.Compile(startState);

        REQUIRE(resTransitions == expTransitions);
        REQUIRE(resEndState == expEndState);
    }

    SECTION("OneOrMoreAST::Compile(startState) - regex:\"a+\" - OneOrMoreAST(move(ast(a)))")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("a");
        auto oomAST = OneOrMoreAST(std::move(ast));
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{1,'a'}] = {2};
        expTransitions[{0,RegexDriver::epsilon}] = {1};
        expTransitions[{2,RegexDriver::epsilon}] = {1,3};
        unsigned expEndState = 3;

        auto [resTransitions, resEndState] = oomAST.Compile(startState);

        REQUIRE(resTransitions == expTransitions);
        REQUIRE(resEndState == expEndState);
    }

    SECTION("ZeroOrOneAST::Compile(startState) - regex:\"a?\" - ZeroOrOneAST(move(ast(a)))")
    {
        RegexDriver rd;
        std::unique_ptr<RegexAST> ast = rd.Parse("a");
        auto zooAST = ZeroOrOneAST(std::move(ast));
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
        unsigned startState = 0;
        expTransitions[{1,'a'}] = {2};
        expTransitions[{0,RegexDriver::epsilon}] = {1,3};
        expTransitions[{2,RegexDriver::epsilon}] = {3};
        unsigned expEndState = 3;

        auto [resTransitions, resEndState] = zooAST.Compile(startState);

        REQUIRE(resTransitions == expTransitions);
        REQUIRE(resEndState == expEndState);
    }

    SECTION("Concatenation::Compile(startState) - regex:\"ab\" - Concatenation(move(ast(a)), move(ast(b)))")
    {
            RegexDriver rd;
            std::unique_ptr<RegexAST> ast1 = rd.Parse("a");
            std::unique_ptr<RegexAST> ast2 = rd.Parse("b");
            auto altAST = ConcatenationAST(std::move(ast1), std::move(ast2));
            std::map<std::pair<unsigned, char>, std::set<unsigned>> expTransitions;
            unsigned startState = 0;
            expTransitions[{0,'a'}] = {1};
            expTransitions[{1,'b'}] = {2};
            unsigned expEndState = 2;

            auto [resTransitions, resEndState] = altAST.Compile(startState);

            REQUIRE(resTransitions == expTransitions);
            REQUIRE(resEndState == expEndState);
    }
}



























