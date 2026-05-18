#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::Completion")
{
    SECTION("Completion of already complete automata - result should be the same as the determinized automata")
    {
        FiniteAutomata automata = FiniteAutomata("(0|1)*");
        FiniteAutomata expected = automata.Determinization();

        FiniteAutomata completeAutomata = automata.Completion();

        REQUIRE(completeAutomata.AcceptStates() == expected.AcceptStates());
        REQUIRE(completeAutomata.StartStates() == expected.StartStates());
        REQUIRE(completeAutomata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Completion of trivial automata")
    {
        FiniteAutomata automata = FiniteAutomata("1");
        std::set<unsigned> expectedAcceptStates {1};
        std::set<unsigned> expectedStartStates {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap {
            {{0, '1'}, {1}},
            {{1, '1'}, {2}},
            {{2, '1'}, {2}}
        };

        FiniteAutomata completeAutomata = automata.Completion();

        REQUIRE(completeAutomata.AcceptStates() == expectedAcceptStates);
        REQUIRE(completeAutomata.StartStates() == expectedStartStates);
        REQUIRE(completeAutomata.TransitionMap() == expectedTransitionMap);
    }

    SECTION("Completion of epsilon automata")
    {
        FiniteAutomata automata = FiniteAutomata("$");
        std::set<unsigned> expectedAcceptStates {0};
        std::set<unsigned> expectedStartStates {0};

        FiniteAutomata completeAutomata = automata.Completion();

        REQUIRE(completeAutomata.AcceptStates() == expectedAcceptStates);
        REQUIRE(completeAutomata.StartStates() == expectedStartStates);
        REQUIRE(completeAutomata.TransitionMap().empty());
    }

    SECTION("Completion of non-accepting automata")
    {
        FiniteAutomata automata = FiniteAutomata("1").Intersection(FiniteAutomata("2")).Minimization();
        std::set<unsigned> expectedStartStates {0};

        FiniteAutomata completeAutomata = automata.Completion();

        REQUIRE(completeAutomata.AcceptStates().empty());
        REQUIRE(completeAutomata.StartStates() == expectedStartStates);
        REQUIRE(completeAutomata.TransitionMap().empty());
    }

    SECTION("Completion of a+b?|c")
    {
        FiniteAutomata automata = FiniteAutomata("a+b?|c");
        std::set<unsigned> expectedAcceptStates {1, 2, 3};
        std::set<unsigned> expectedStartStates {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap {
            {{0, 'a'}, {1}},
            {{0, 'b'}, {4}},
            {{0, 'c'}, {2}},
            {{1, 'a'}, {1}},
            {{1, 'b'}, {3}},
            {{1, 'c'}, {4}},
            {{2, 'a'}, {4}},
            {{2, 'b'}, {4}},
            {{2, 'c'}, {4}},
            {{3, 'a'}, {4}},
            {{3, 'b'}, {4}},
            {{3, 'c'}, {4}},
            {{4, 'a'}, {4}},
            {{4, 'b'}, {4}},
            {{4, 'c'}, {4}}
        };

        FiniteAutomata completeAutomata = automata.Completion();

        REQUIRE(completeAutomata.AcceptStates() == expectedAcceptStates);
        REQUIRE(completeAutomata.StartStates() == expectedStartStates);
        REQUIRE(completeAutomata.TransitionMap() == expectedTransitionMap);
    }
}
