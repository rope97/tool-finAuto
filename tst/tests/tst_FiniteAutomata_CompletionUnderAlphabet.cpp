#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::CompletionUnderAlphabet")
{
    SECTION("Completion with empty set - result should be the same as with normal completion")
    {
        FiniteAutomata automata = FiniteAutomata("(0|1)*");
        FiniteAutomata expected = automata.Completion();

        FiniteAutomata completeAutomata = automata.CompletionUnderAlphabet({});

        REQUIRE(completeAutomata.AcceptStates() == expected.AcceptStates());
        REQUIRE(completeAutomata.StartStates() == expected.StartStates());
        REQUIRE(completeAutomata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Completion with set already in alphabet - result should be the same as with normal completion")
    {
        FiniteAutomata automata = FiniteAutomata("a+b?|c");
        FiniteAutomata expected = automata.Completion();

        FiniteAutomata completeAutomata = automata.CompletionUnderAlphabet({'a', 'b', 'c'});

        REQUIRE(completeAutomata.AcceptStates() == expected.AcceptStates());
        REQUIRE(completeAutomata.StartStates() == expected.StartStates());
        REQUIRE(completeAutomata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Completion with disjoint set")
    {
        FiniteAutomata automata = FiniteAutomata("0|1");
        std::set<unsigned> expectedAcceptStates {1, 2};
        std::set<unsigned> expectedStartStates {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap {
            {{0, '0'}, {1}},
            {{0, '1'}, {2}},
            {{0, '2'}, {3}},
            {{1, '0'}, {3}},
            {{1, '1'}, {3}},
            {{1, '2'}, {3}},
            {{2, '0'}, {3}},
            {{2, '1'}, {3}},
            {{2, '2'}, {3}},
            {{3, '0'}, {3}},
            {{3, '1'}, {3}},
            {{3, '2'}, {3}},
        };

        FiniteAutomata completeAutomata = automata.CompletionUnderAlphabet({'2'});

        REQUIRE(completeAutomata.AcceptStates() == expectedAcceptStates);
        REQUIRE(completeAutomata.StartStates() == expectedStartStates);
        REQUIRE(completeAutomata.TransitionMap() == expectedTransitionMap);
    }
}
