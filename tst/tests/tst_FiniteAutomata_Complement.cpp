#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

//EXPECTED RESULTS DONE ON PAPER

TEST_CASE("FiniteAutomata::Complement")
{
    SECTION("For empty string, function returns automaton that accepts no strings")
    {
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("$").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap = {};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Simple regex - ab")
    {
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("ab").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0, 1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {3};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {2};
        expectedTransitionMap[{3, 'b'}] = {2};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Regex with + - (ab)+c+")
    {
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("(ab)+c+").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0, 1, 2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{0, 'c'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {3};
        expectedTransitionMap[{1, 'c'}] = {2};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{2, 'c'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {1};
        expectedTransitionMap[{3, 'b'}] = {2};
        expectedTransitionMap[{3, 'c'}] = {4};
        expectedTransitionMap[{4, 'a'}] = {2};
        expectedTransitionMap[{4, 'b'}] = {2};
        expectedTransitionMap[{4, 'c'}] = {4};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Regex with ? - (ab)?c?"){
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("(ab)?c?").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{0, 'c'}] = {3};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {4};
        expectedTransitionMap[{1, 'c'}] = {2};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{2, 'c'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {2};
        expectedTransitionMap[{3, 'b'}] = {2};
        expectedTransitionMap[{3, 'c'}] = {2};
        expectedTransitionMap[{4, 'a'}] = {2};
        expectedTransitionMap[{4, 'b'}] = {2};
        expectedTransitionMap[{4, 'c'}] = {3};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Regex with * - (ab)*c*"){
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("(ab)*c*").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{0, 'c'}] = {3};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {0};
        expectedTransitionMap[{1, 'c'}] = {2};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{2, 'c'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {2};
        expectedTransitionMap[{3, 'b'}] = {2};
        expectedTransitionMap[{3, 'c'}] = {3};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Regex with | - (ab)|c"){
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("(ab)|c").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0, 1, 2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{0, 'c'}] = {3};
        expectedTransitionMap[{1, 'a'}] = {2};
        expectedTransitionMap[{1, 'b'}] = {3};
        expectedTransitionMap[{1, 'c'}] = {2};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{2, 'c'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {2};
        expectedTransitionMap[{3, 'b'}] = {2};
        expectedTransitionMap[{3, 'c'}] = {2};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("Complex regex - a((a*b|b*a)+b)*"){
        //Arrange
        FiniteAutomata startingAutomata = FiniteAutomata("a((a*b|b*a)+b)*").Minimization();
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0, 2, 3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0, 'a'}] = {1};
        expectedTransitionMap[{0, 'b'}] = {2};
        expectedTransitionMap[{1, 'a'}] = {3};
        expectedTransitionMap[{1, 'b'}] = {3};
        expectedTransitionMap[{2, 'a'}] = {2};
        expectedTransitionMap[{2, 'b'}] = {2};
        expectedTransitionMap[{3, 'a'}] = {3};
        expectedTransitionMap[{3, 'b'}] = {4};
        expectedTransitionMap[{4, 'a'}] = {3};
        expectedTransitionMap[{4, 'b'}] = {4};

        //Act
        FiniteAutomata resultingAutomata = startingAutomata.Complement().Minimization();

        //Assert
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }
}
