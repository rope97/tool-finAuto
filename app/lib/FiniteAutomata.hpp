#ifndef FINITE_AUTOMATA_HPP
#define FINITE_AUTOMATA_HPP

#include <vector>
#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <random>
#include <optional>
#include <functional>

class FiniteAutomata
{
public:
	FiniteAutomata();
	FiniteAutomata(const std::string& regex);
    FiniteAutomata(const std::map<std::pair<unsigned, char>, std::set<unsigned>>& transitionMap,
                   const std::set<unsigned> &startStates, const std::set<unsigned>& acceptStates);

	//getters
    std::set<unsigned> StartStates() const;
    std::set<unsigned> AcceptStates() const;
    std::map<std::pair<unsigned, char>, std::set<unsigned>> TransitionMap() const;
	
	std::set<char> Alphabet() const;
    std::set<unsigned> States() const;
	std::map<unsigned, std::set<char>> FromStateToStates() const;

    std::string BuildRegex() const;

    FiniteAutomata Determinization() const;
    FiniteAutomata Completion() const;
    FiniteAutomata CompletionUnderAlphabet(const std::set<char>& alphabet) const;
    FiniteAutomata Reversal() const;
    FiniteAutomata Minimization() const;

    FiniteAutomata Union(const FiniteAutomata& other) const;
    FiniteAutomata Intersection(const FiniteAutomata& other) const;
    FiniteAutomata Difference(const FiniteAutomata& other) const;
    FiniteAutomata Complement() const;

	bool IsAccepted(const std::string &s);
    std::optional<std::string> GenerateRandom();
    std::optional<std::string> GenerateAccepted();
    std::optional<std::string> GenerateUnaccepted();

    std::vector<std::set<unsigned>> MatchSimulation(const std::string& str) const;

private: 
	std::map<std::pair<unsigned, char>, std::set<unsigned>> mTransitionMap;
	std::set<unsigned> mStartStates;
	std::set<unsigned> mAcceptStates;
	
    std::set<unsigned> EpsilonClosure(unsigned state) const;
    FiniteAutomata BinarySetOperation(const FiniteAutomata& other, std::function<bool (bool, bool)> acceptStateCondition) const;

    friend std::istream& operator>>(std::istream& in, FiniteAutomata& automata);
    friend std::ostream& operator<<(std::ostream& out, const FiniteAutomata& automata);
};

std::ostream& operator<<(std::ostream& out, const FiniteAutomata& automata);
std::istream& operator>>(std::istream& in, FiniteAutomata& automata);

#endif // FINITE_AUTOMATA_HPP
