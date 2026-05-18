#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"
#include <sstream>

TEST_CASE("friend std::istream& operator>> - user defined operator for FiniteAutomata")
{
    SECTION("Standard automata, no special cases")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};

        input << "1 0 1 2 3 0 a 1 0 b 2 1 a 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Standard automata, no special cases")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};

        input << "1 0 2 1 2 3 0 a 1 0 b 2 1 a 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Standard automata, no special cases")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0, 1};
        std::set<unsigned> expectedAcceptStates = {1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{3, '$'}] = {2};

        input << "2 0 1 2 1 2 4 0 a 1 0 b 2 1 a 2 3 $ 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Standard automata, with one key having multiple values")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0, 1};
        std::set<unsigned> expectedAcceptStates = {1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1, 2, 3};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{3, '$'}] = {2};

        input << "2 0 1 2 1 2 6 0 a 1 0 a 2 0 a 3 0 b 2 1 a 2 3 $ 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Automata with no start states")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};

        input << "0 1 2 3 0 a 1 0 b 2 1 a 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Automata with no accept states")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};

        input << "1 0 0 3 0 a 1 0 b 2 1 a 2";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Automata with no transitions")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;

        input << "1 0 1 2 0";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }

    SECTION("Empty automata")
    {
        FiniteAutomata read;
        std::stringstream input;

        std::set<unsigned> expectedStartStates = {};
        std::set<unsigned> expectedAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;

        input << "0 0 0";
        input >> read;

        //Assert
        REQUIRE(expectedAcceptStates == read.AcceptStates());
        REQUIRE(expectedStartStates == read.StartStates());
        REQUIRE(expectedTransitionMap == read.TransitionMap());
    }
}
