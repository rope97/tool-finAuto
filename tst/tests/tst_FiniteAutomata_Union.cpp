#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

//EXPECTED RESULTS DONE ON PAPER

TEST_CASE("FiniteAutomata::Union")
{
    SECTION("Union of 2 automatas created out of empty strings, returns one of the given automata")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("$").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("$").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap = {};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Union(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 identical automata - ab, should return the same automata")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("ab").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("ab").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{1, 'b'}] = {2};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Union(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("2 automata - ab+ cc")
    {
        //Arrange
        FiniteAutomata startingAutomata1 = FiniteAutomata("ab+").Minimization();
        FiniteAutomata startingAutomata2 = FiniteAutomata("cc").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {3, 4};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'c'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {3};
        expectedTransitionMap[{2, 'c'}] = {4};
        expectedTransitionMap[{3, 'b'}] = {3};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Union(startingAutomata2)).Minimization();

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
        std::set<unsigned> expectedAcceptStates = {1, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {1};
        expectedTransitionMap[{0, 'c'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {1};
        expectedTransitionMap[{1, 'b'}] = {1};
        expectedTransitionMap[{1, 'c'}] = {3};
        expectedTransitionMap[{2, 'a'}] = {3};

        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Union(startingAutomata2)).Minimization();

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
        std::set<unsigned> expectedAcceptStates = {0, 3, 5, 7, 10, 11};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{0, 'c'}] = {3};
        expectedTransitionMap[{1, 'a'}] = {4};
        expectedTransitionMap[{1, 'b'}] = {5};
        expectedTransitionMap[{2, 'a'}] = {6};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{4, 'a'}] = {4};
        expectedTransitionMap[{4, 'b'}] = {7};
        expectedTransitionMap[{5, 'a'}] = {8};
        expectedTransitionMap[{5, 'b'}] = {2};
        expectedTransitionMap[{5, 'c'}] = {3};
        expectedTransitionMap[{6, 'b'}] = {9};
        expectedTransitionMap[{7, 'a'}] = {4};
        expectedTransitionMap[{7, 'c'}] = {3};
        expectedTransitionMap[{8, 'a'}] = {4};
        expectedTransitionMap[{8, 'b'}] = {10};
        expectedTransitionMap[{9, 'c'}] = {11};
        expectedTransitionMap[{10, 'a'}] = {4};
        expectedTransitionMap[{10, 'c'}] = {11};
        expectedTransitionMap[{11, 'a'}] = {11};
        expectedTransitionMap[{11, 'b'}] = {11};
        expectedTransitionMap[{11, 'c'}] = {11};


        //Act
        FiniteAutomata resultingAutomata = (startingAutomata1.Union(startingAutomata2)).Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }
}
