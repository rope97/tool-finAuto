#include <catch2/catch.hpp>
#include "lib/FiniteAutomata.hpp"

TEST_CASE("FiniteAutomata::MatchSimulation")
{
    SECTION("Simulation with empty string")
    {
        FiniteAutomata automata = FiniteAutomata("(0|1)*");
        std::vector<std::set<unsigned>> expectedResult = {
            {0, 1, 2, 4, 7}
        };

        auto steps = automata.MatchSimulation("");

        REQUIRE(steps == expectedResult);
    }

    SECTION("Simulation with acceptable string")
    {
        FiniteAutomata automata = FiniteAutomata("(a+b)*c?");
        std::vector<std::set<unsigned>> expectedResult = {
            {0, 1, 2, 6, 7, 9},
            {3, 2, 4},
            {3, 2, 4},
            {5, 1, 2, 6, 7, 9},
            {3, 2, 4},
            {5, 1, 2, 6, 7, 9},
            {8, 9}
        };

        auto steps = automata.MatchSimulation("aababc");

        REQUIRE(steps == expectedResult);
    }

    SECTION("Simulation with unacceptable string - all symbols in alphabet")
    {
        FiniteAutomata automata = FiniteAutomata("(1*01*0)*1*").Minimization();
        std::vector<std::set<unsigned>> expectedResult = {
            {0},
            {0},
            {0},
            {1},
            {1},
            {0},
            {1},
            {1}
        };

        auto steps = automata.MatchSimulation("1101001");

        REQUIRE(steps == expectedResult);
    }

    SECTION("Simulation with unacceptable string - symbols not in alphabet")
    {
        FiniteAutomata automata = FiniteAutomata("a+b*");
        std::vector<std::set<unsigned>> expectedResult = {
            {0, 1},
            {2, 1, 3, 4, 6},
            {2, 1, 3, 4, 6},
            {2, 1, 3, 4, 6},
            {},
            {},
            {}
        };

        auto steps = automata.MatchSimulation("aaacbb");

        REQUIRE(steps == expectedResult);
    }

    SECTION("Simulation on automata with multiple start states")
    {
        FiniteAutomata automata = FiniteAutomata("01|ab").Determinization().Reversal();
        std::vector<std::set<unsigned>> expectedResult = {
            {4, 3},
            {2},
            {0}
        };

        auto steps = automata.MatchSimulation("ba");

        REQUIRE(steps == expectedResult);
    }
}
