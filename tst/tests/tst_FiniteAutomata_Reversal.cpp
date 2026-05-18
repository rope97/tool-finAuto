#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::Reversal")
{
    SECTION("Concatination reversal")
    {
        FiniteAutomata automata = FiniteAutomata("abcd");
        FiniteAutomata expected = FiniteAutomata("dcba").Minimization();

        automata = automata.Reversal().Minimization();

        REQUIRE(automata.AcceptStates() == expected.AcceptStates());
        REQUIRE(automata.StartStates() == expected.StartStates());
        REQUIRE(automata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Empty string reversal")
    {
        FiniteAutomata automata = FiniteAutomata("$");
        FiniteAutomata expected = FiniteAutomata("$").Minimization();

        automata = automata.Reversal().Minimization();

        REQUIRE(automata.AcceptStates() == expected.AcceptStates());
        REQUIRE(automata.StartStates() == expected.StartStates());
        REQUIRE(automata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Reversal with alternation")
    {
        FiniteAutomata automata = FiniteAutomata("abc|aa|ca");
        FiniteAutomata expected = FiniteAutomata("cba|aa|ac").Minimization();

        automata = automata.Reversal().Minimization();

        REQUIRE(automata.AcceptStates() == expected.AcceptStates());
        REQUIRE(automata.StartStates() == expected.StartStates());
        REQUIRE(automata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Reversal with Kleene star")
    {
        FiniteAutomata automata = FiniteAutomata("(ab|01)*a?");
        FiniteAutomata expected = FiniteAutomata("a?(ba|10)*").Minimization();

        automata = automata.Reversal().Minimization();

        REQUIRE(automata.AcceptStates() == expected.AcceptStates());
        REQUIRE(automata.StartStates() == expected.StartStates());
        REQUIRE(automata.TransitionMap() == expected.TransitionMap());
    }

    SECTION("Reversal with returning edge")
    {
        FiniteAutomata automata = FiniteAutomata("(a+b)*cd?");
        FiniteAutomata expected = FiniteAutomata("d?c(ba+)*").Minimization();

        automata = automata.Reversal().Minimization();

        REQUIRE(automata.AcceptStates() == expected.AcceptStates());
        REQUIRE(automata.StartStates() == expected.StartStates());
        REQUIRE(automata.TransitionMap() == expected.TransitionMap());
    }
}
