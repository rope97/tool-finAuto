#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::Determinization - transform Nondeterministic Finite Automata to Deterministic Finite Automata")
{
    SECTION("For empty string, function returns automaton that accepts just empty string")
    {
        FiniteAutomata nfa = FiniteAutomata("$");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"a*\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("a*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,1};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{1,'a'}] = {1};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"a*b+\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("a*b+");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};
        expectedTransitionMap[{2,'b'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a*b)?\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a*b)?");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"((a*b)?|c?)?\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("((a*b)?|c?)?");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,2,3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{0,'c'}] = {3};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(ab|c)*b\" - to DFA - PPJ 2019 klk grp A")
    {
        FiniteAutomata nfa = FiniteAutomata("(ab|c)*b");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {2};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{0,'c'}] = {3};
        expectedTransitionMap[{1,'b'}] = {4};
        expectedTransitionMap[{4,'a'}] = {1};
        expectedTransitionMap[{4,'b'}] = {2};
        expectedTransitionMap[{4,'c'}] = {3};
        expectedTransitionMap[{3,'a'}] = {1};
        expectedTransitionMap[{3,'b'}] = {2};
        expectedTransitionMap[{3,'c'}] = {3};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a|b|c)*\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a|b|c)*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {0,1,2,3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{0,'c'}] = {3};
        expectedTransitionMap[{1,'a'}] = {1};
        expectedTransitionMap[{1,'b'}] = {2};
        expectedTransitionMap[{1,'c'}] = {3};
        expectedTransitionMap[{2,'a'}] = {1};
        expectedTransitionMap[{2,'b'}] = {2};
        expectedTransitionMap[{2,'c'}] = {3};
        expectedTransitionMap[{3,'a'}] = {1};
        expectedTransitionMap[{3,'b'}] = {2};
        expectedTransitionMap[{3,'c'}] = {3};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"1(0|1)*\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("1(0|1)*");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {1,2,3};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'1'}] = {1};
        expectedTransitionMap[{1,'1'}] = {3};
        expectedTransitionMap[{1,'0'}] = {2};
        expectedTransitionMap[{2,'0'}] = {2};
        expectedTransitionMap[{2,'1'}] = {3};
        expectedTransitionMap[{3,'1'}] = {3};
        expectedTransitionMap[{3,'0'}] = {2};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }

    SECTION("NFA constructed from \"(a+|(a*b+))+(b|a|c)\" - to DFA")
    {
        FiniteAutomata nfa = FiniteAutomata("(a+|(a*b+))+(b|a|c)");
        std::set<unsigned> expectedStartStates = {0};
        std::set<unsigned> expectedAcceptStates = {3,4,5};
        std::map<std::pair<unsigned, char>, std::set<unsigned>> expectedTransitionMap;
        expectedTransitionMap[{0,'a'}] = {1};
        expectedTransitionMap[{0,'b'}] = {2};
        expectedTransitionMap[{1,'a'}] = {3};
        expectedTransitionMap[{1,'b'}] = {4};
        expectedTransitionMap[{1,'c'}] = {5};
        expectedTransitionMap[{2,'a'}] = {3};
        expectedTransitionMap[{2,'b'}] = {4};
        expectedTransitionMap[{2,'c'}] = {5};
        expectedTransitionMap[{3,'a'}] = {3};
        expectedTransitionMap[{3,'b'}] = {4};
        expectedTransitionMap[{3,'c'}] = {5};
        expectedTransitionMap[{4,'a'}] = {3};
        expectedTransitionMap[{4,'b'}] = {4};
        expectedTransitionMap[{4,'c'}] = {5};

        FiniteAutomata resultingAutomata = nfa.Determinization();

        REQUIRE(expectedStartStates == resultingAutomata.StartStates());
        REQUIRE(expectedAcceptStates == resultingAutomata.AcceptStates());
        REQUIRE(expectedTransitionMap == resultingAutomata.TransitionMap());
    }
}
