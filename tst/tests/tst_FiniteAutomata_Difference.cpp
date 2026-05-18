#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"
#include <iostream>

//EXPECTED RESULTS DONE ON PAPER

TEST_CASE("FiniteAutomata::Difference")
{
    SECTION("Difference of 2 automatas created out of empty strings, returns automaton that does not accept anything")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("$").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("$").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap = {};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Difference(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 identical automata - ab, returns automaton that does not accept anything")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("ab").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("ab").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Difference(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 automata - ab+ cc")
    {
        FiniteAutomata startingAutomata1 = FiniteAutomata("ab+").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("cc").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{1, 'b'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Difference(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 complex automata - abc|aa|ca (a|b)+c?")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("abc|aa|ca").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("(a|b)+c?").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'c'}] = {1};
        expectedTransitionMap[{1, 'a'}] = {2};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Difference(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 complex automata - (a+b)*c? a?b+abc(a|b|c)*")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("(a+b)*c?").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("a?b+abc(a|b|c)*").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0, 2, 4, 5, 7};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'c'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {3};
        expectedTransitionMap[{1, 'b'}] = {4};
        expectedTransitionMap[{3, 'a'}] = {3};
        expectedTransitionMap[{3, 'b'}] = {5};
        expectedTransitionMap[{4, 'a'}] = {6};
        expectedTransitionMap[{4, 'c'}] = {2};
        expectedTransitionMap[{5, 'a'}] = {3};
        expectedTransitionMap[{5, 'c'}] = {2};
        expectedTransitionMap[{6, 'a'}] = {3};
        expectedTransitionMap[{6, 'b'}] = {7};
        expectedTransitionMap[{7, 'a'}] = {3};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Difference(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }
}
