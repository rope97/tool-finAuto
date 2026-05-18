#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::Minimization - transform Nondeterministic Finite Automata to Minimal Deterministic Finite Automata")
{
    SECTION("For empty string, function returns automaton that accepts just empty string")
    {
        FiniteAutomata nfa = FiniteAutomata("$");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"a*\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("a*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {0};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"a*b+\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("a*b+");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {0};
        expectedTransitionMap[{0,'b'}] = {1};
        expectedTransitionMap[{1,'b'}] = {1};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a*b)?\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a*b)?");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"((a*b)?|c?)?\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("((a*b)?|c?)?");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{0,'c'}] = {2};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(ab|c)*b\" - to MDFA - PPJ 2019 klk grp A")
    {
        FiniteAutomata nfa = FiniteAutomata("(ab|c)*b");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{0,'c'}] = {0};
        expectedTransitionMap[{1,'b'}] = {0};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a|b|c)*\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a|b|c)*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {0};
        expectedTransitionMap[{0,'b'}] = {0};
        expectedTransitionMap[{0,'c'}] = {0};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"1(0|1)*\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("1(0|1)*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'1'}] = {1};
        expectedTransitionMap[{1,'1'}] = {1};
        expectedTransitionMap[{1,'0'}] = {1};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a?|b?|c?|d?|e?)+\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a?|b?|c?|d?|e?)+");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {0};
        expectedTransitionMap[{0,'b'}] = {0};
        expectedTransitionMap[{0,'c'}] = {0};
        expectedTransitionMap[{0,'d'}] = {0};
        expectedTransitionMap[{0,'e'}] = {0};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a+|(a*b+))+(b|a|c)\" - to MDFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a+|(a*b+))+(b|a|c)");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2,3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {1};
        expectedTransitionMap[{1,'a'}] = {2};
        expectedTransitionMap[{1,'b'}] = {2};
        expectedTransitionMap[{1,'c'}] = {3};
        expectedTransitionMap[{2,'a'}] = {2};
        expectedTransitionMap[{2,'b'}] = {2};
        expectedTransitionMap[{2,'c'}] = {3};

        FiniteAutomata resultingAutomata = nfa.Minimization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

}
