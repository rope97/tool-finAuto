#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::Alphabet - return all transition symbols of the automata")
{
    SECTION("Standard automata")
    {
        std::set<unsigned> testStartStates = {0};
        std::set<unsigned> testAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'b'}] = {2};
        testTransitionMap[{1,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {'a', 'b'};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }

    SECTION("Standard automata with one key having multiple values")
    {
        std::set<unsigned> testStartStates = {0};
        std::set<unsigned> testAcceptStates = {2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1, 4, 5, 6};
        testTransitionMap[{0,'b'}] = {2};
        testTransitionMap[{1,'$'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {'a', 'b', '$'};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }

    SECTION("Automata with no start states")
    {
        std::set<unsigned> testStartStates = {};
        std::set<unsigned> testAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{1,'a'}] = {2};
        testTransitionMap[{2,'a'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {'a'};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }

    SECTION("Automata with no accept states")
    {
        std::set<unsigned> testStartStates = {0};
        std::set<unsigned> testAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;
        testTransitionMap[{0,'a'}] = {1};
        testTransitionMap[{0,'b'}] = {2};
        testTransitionMap[{1,'c'}] = {2};

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {'a', 'b', 'c'};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }

    SECTION("Automata with no transitions")
    {
        std::set<unsigned> testStartStates = {0};
        std::set<unsigned> testAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }

    SECTION("Empty automata")
    {
        std::set<unsigned> testStartStates = {};
        std::set<unsigned> testAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> testTransitionMap;

        FiniteAutomata automata = FiniteAutomata(testTransitionMap, testStartStates, testAcceptStates);

        std::set<char> expected = {};

        //Assert
        REQUIRE(automata.Alphabet() == expected);
    }
}
