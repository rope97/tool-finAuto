#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"
#include <sstream>
#include <iostream>


TEST_CASE("friend std::ostream& operator<< - user defined operator for FiniteAutomata")
{
    SECTION("Standard automata, no special cases")
    {
        std::set<unsigned> testStartStates = {0};
        std::set<unsigned> testAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'b'}] = {2};
        testTransitionMap[{1,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "1 0 1 2 3 0 a 1 0 b 2 1 a 2 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Standard automata, no special cases")
    {
        std::set<unsigned> testStartStates = {0, 1};
        std::set<unsigned> testAcceptStates = {2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'$'}] = {2};
        testTransitionMap[{1,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "2 0 1 2 2 3 3 0 $ 2 0 a 1 1 a 2 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Standard automata with one key having two values")
    {
        std::set<unsigned> testStartStates = {0, 1, 2};
        std::set<unsigned> testAcceptStates = {2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1, 2};
        testTransitionMap[{1,'a'}] = {2};
        testTransitionMap[{1,'$'}] = {3};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "3 0 1 2 2 2 3 4 0 a 1 0 a 2 1 $ 3 1 a 2 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Automata with no start states")
    {
        std::set<unsigned> testStartStates = {};
        std::set<unsigned> testAcceptStates = {2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'$'}] = {2};
        testTransitionMap[{1,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "0 2 2 3 3 0 $ 2 0 a 1 1 a 2 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Automata with no accept states")
    {
        std::set<unsigned> testStartStates = {0, 1};
        std::set<unsigned> testAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'$'}] = {2};
        testTransitionMap[{1,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "2 0 1 0 3 0 $ 2 0 a 1 1 a 2 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Automata with no transitions")
    {
        std::set<unsigned> testStartStates = {0, 1};
        std::set<unsigned> testAcceptStates = {2 ,3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "2 0 1 2 2 3 0 ";

        //Assert
        REQUIRE(expected == output.str());
    }

    SECTION("Empty automata")
    {
        std::set<unsigned> testStartStates = {};
        std::set<unsigned> testAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::stringstream output;

        output << automata;
        std::string expected = "0 0 0 ";

        //Assert
        REQUIRE(expected == output.str());
    }
}
