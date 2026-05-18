#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::BuildRegex")
{
    SECTION("Trivial automata")
    {
        FiniteAutomata automata = FiniteAutomata("abcd").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }

    SECTION("Automata with alternation")
    {
        FiniteAutomata automata = FiniteAutomata("0|1").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }

    SECTION("Automata with loop")
    {
        FiniteAutomata automata = FiniteAutomata("01+0?").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }

    SECTION("Automata with returning edge")
    {
        FiniteAutomata automata = FiniteAutomata("(a+b)*cd?").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }

    SECTION("Automata generated with epsilon regex")
    {
        FiniteAutomata automata = FiniteAutomata("$").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }

    SECTION("Automata generating an empty sub-regex")
    {
        FiniteAutomata automata = FiniteAutomata("0+|1?").Minimization();

        auto regex = automata.BuildRegex();
        FiniteAutomata resultingAutomata = FiniteAutomata(regex).Minimization();

        REQUIRE(automata.StartStates() == resultingAutomata.StartStates());
        REQUIRE(automata.AcceptStates() == resultingAutomata.AcceptStates());
        REQUIRE(automata.TransitionMap() == resultingAutomata.TransitionMap());
    }
}
