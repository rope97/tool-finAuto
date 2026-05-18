#include "lib/FiniteAutomata.hpp"
#include "lib/RegexDriver.hpp"

#include <queue>
#include <set>

FiniteAutomata::FiniteAutomata(const std::string& regex)
{
    RegexDriver driver;
    std::unique_ptr<RegexAST> ast = driver.Parse(regex);
    unsigned startState = 0;
    auto [transitionMap, endState] = ast->Compile(startState);
    mAcceptStates.insert(endState);
    mStartStates.insert(startState);
    mTransitionMap = transitionMap;
}

FiniteAutomata::FiniteAutomata()
    : mTransitionMap()
    , mStartStates()
    , mAcceptStates()
{ }

FiniteAutomata::FiniteAutomata(const std::map<std::pair<unsigned, char>, std::set<unsigned>>& transitionMap,
                               const std::set<unsigned>& startStates, const std::set<unsigned>& acceptStates)
    : mTransitionMap(transitionMap)
    , mStartStates(startStates)
    , mAcceptStates(acceptStates)
{ }

std::set<unsigned> FiniteAutomata::StartStates() const
{
    return mStartStates;
}

std::set<unsigned> FiniteAutomata::AcceptStates() const
{
    return mAcceptStates;
}

std::map<std::pair<unsigned, char>, std::set<unsigned>> FiniteAutomata::TransitionMap() const
{
    return mTransitionMap;
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomata& automata)
{
    out << automata.mStartStates.size() << " ";
    for (unsigned state : automata.mStartStates)
        out << state << " ";

    out << automata.mAcceptStates.size() << " ";
    for (unsigned state : automata.mAcceptStates)
        out << state << " ";

    unsigned tmSize=0;
    for (auto& [k, v] : automata.mTransitionMap)
        tmSize += v.size();

    out << tmSize << " ";
    for (auto& [k, v] : automata.mTransitionMap) {
        for (unsigned state : v)
            out << k.first << " " << k.second << " " << state << " ";
    }

    return out;
}

std::istream& operator>>(std::istream& in, FiniteAutomata& automata)
{
    unsigned counter1, counter2, counter3;
    unsigned state;
    char symbol;
    unsigned state2;

    in >> counter1;
    for (unsigned i = 0; i < counter1; i++) {
        in >> state;
        automata.mStartStates.insert(state);
    }

    in >> counter2;
    for (unsigned i = 0; i < counter2; i++) {
        in >> state;
        automata.mAcceptStates.insert(state);
    }

    in >> counter3;
    for (unsigned i = 0; i < counter3; i++) {
        in >> state >> symbol >> state2;
        automata.mTransitionMap[{state, symbol}].insert(state2);
    }

    return in;
} 

std::set<char> FiniteAutomata::Alphabet() const
{
    std::set<char> alphabet;
    for (auto& [k, _] : mTransitionMap) {
        char symbol = k.second;
        alphabet.insert(symbol);
    }
    return alphabet;
}

// Performs BFS to construct the epsilon closure of passed state.
std::set<unsigned> FiniteAutomata::EpsilonClosure(unsigned state) const
{
    std::set<unsigned> epsilonClosure = {state};

    std::queue<unsigned> stateQueue;
    stateQueue.push(state);

    while (!stateQueue.empty()) {
        unsigned currentState = stateQueue.front();
        stateQueue.pop();

        auto it = mTransitionMap.find({currentState, RegexDriver::epsilon});
        if (it == mTransitionMap.end())
            continue;
        auto adjacentByEps = it->second;

        for (unsigned state : adjacentByEps) {
            // If this state hasn't been visited yet...
            if (epsilonClosure.find(state) == epsilonClosure.end()) {
                epsilonClosure.insert(state);
                stateQueue.push(state);
            }
        }
    }

    return epsilonClosure;
}

// Implementation of powerset construction.
FiniteAutomata FiniteAutomata::Determinization() const
{
    std::set<char> alphabet = this->Alphabet();

    // Used for keeping track of subsets which have already
    // been constructed and for mapping them into their new
    // labels (state numbers).
    std::map<std::set<unsigned>, unsigned> constructedSubsets;

    // Members for the soon to be determinized automata.
    std::map<std::pair<unsigned, char>, std::set<unsigned>> transitionMap;
    std::set<unsigned> startStates = {0};
    std::set<unsigned> acceptStates;

    unsigned stateCounter = 0;
    std::set<unsigned> initialSubset;
    for (unsigned state : mStartStates)
        initialSubset.merge(this->EpsilonClosure(state));
    constructedSubsets[initialSubset] = stateCounter;

    std::queue<std::set<unsigned>> subsetQueue;
    subsetQueue.push(initialSubset);

    while (!subsetQueue.empty()) {
        auto currentSubset = subsetQueue.front();
        subsetQueue.pop();

        for (char symbol : alphabet) {
            if (symbol == RegexDriver::epsilon)
                continue;
            // Potential new subset constructed with
            // epsilon closures from currentSubset with respect
            // to some symbol.
            std::set<unsigned> newSubset;
            for (unsigned state : currentSubset) {
                auto it = mTransitionMap.find({state, symbol});
                if (it == mTransitionMap.end())
                    continue;
                auto adjacentBySymbol = it->second;
                for (unsigned adjacentState : adjacentBySymbol)
                    newSubset.merge(this->EpsilonClosure(adjacentState));
            }
            if (newSubset.empty())
                continue;
            auto it = constructedSubsets.find(newSubset);
            if (it == constructedSubsets.end()) {
                constructedSubsets[newSubset] = ++stateCounter;
                subsetQueue.push(newSubset);
            }
            transitionMap[{constructedSubsets[currentSubset], symbol}].insert(constructedSubsets[newSubset]);
        }
    }

    // The accept states of the determinized automata
    // are all of its states which correspond to a subset
    // containing any of the original automata's accept states.
    for (auto& [k, v] : constructedSubsets) {
        for (unsigned state : mAcceptStates) {
            if (k.find(state) != k.end()) {
                acceptStates.insert(v);
                break;
            }
        }
    }

    return FiniteAutomata(transitionMap, startStates, acceptStates);
}

FiniteAutomata FiniteAutomata::Completion() const
{
    FiniteAutomata complete = this->Determinization();

    std::set<char> alphabet = complete.Alphabet();
    std::set<unsigned> allStates = complete.States();

    unsigned errorState = allStates.size();
    bool errorStateAdded = false;

    for (unsigned state : allStates) {
        for (char symbol : alphabet) {
            if (complete.mTransitionMap.find({state, symbol}) == complete.mTransitionMap.end()) {
                complete.mTransitionMap[{state, symbol}].insert(errorState);
                errorStateAdded = true;
            }
        }
    }

    if (errorStateAdded) {
        for (char symbol : alphabet)
            complete.mTransitionMap[{errorState, symbol}].insert(errorState);
    }

    return complete;
}

FiniteAutomata FiniteAutomata::CompletionUnderAlphabet(const std::set<char>& alphabet) const
{
    FiniteAutomata complete = this->Determinization();

    std::set<char> newAlphabet = complete.Alphabet();
    for(char symbol : alphabet){
        newAlphabet.insert(symbol);
    }
    std::set<unsigned> allStates = complete.States();

    unsigned errorState = allStates.size();
    bool errorStateAdded = false;

    for (unsigned state : allStates) {
        for (char symbol : newAlphabet) {
            if (complete.mTransitionMap.find({state, symbol}) == complete.mTransitionMap.end()) {
                complete.mTransitionMap[{state, symbol}].insert(errorState);
                errorStateAdded = true;
            }
        }
    }

    if (errorStateAdded) {
        for (char symbol : newAlphabet)
            complete.mTransitionMap[{errorState, symbol}].insert(errorState);
    }

    return complete;

}

std::set<unsigned> FiniteAutomata::States() const
{
    std::set<unsigned> states;
    for (auto& [k, v] : mTransitionMap) {
        unsigned fromState = k.first;
        states.insert(fromState);
        for (unsigned state : v)
            states.insert(state);
    }
    for (unsigned state : mStartStates)
        states.insert(state);
    for (unsigned state : mAcceptStates)
        states.insert(state);

    return states;
}

// decomposition of keys from transition map
std::map<unsigned, std::set<char>> FiniteAutomata::FromStateToStates() const
{
    std::map<unsigned, std::set<char>> mHowToMove;

    for(auto& [k, v] : mTransitionMap){
        mHowToMove[k.first].insert(k.second);
    }

    return mHowToMove;
}

std::string FiniteAutomata::BuildRegex() const
{
    FiniteAutomata automata = this->Minimization().Completion();
    unsigned numOfStates = automata.States().size();
    unsigned newStartState = numOfStates;
    unsigned newAcceptState = newStartState + 1;

    for (unsigned state : automata.mStartStates)
        automata.mTransitionMap[{newStartState, RegexDriver::epsilon}].insert(state);

    for (unsigned state : automata.mAcceptStates)
        automata.mTransitionMap[{state, RegexDriver::epsilon}].insert(newAcceptState);

    automata.mStartStates = {newStartState};
    automata.mAcceptStates = {newAcceptState};

    std::map<std::pair<unsigned, unsigned>, std::string> regexLabelMap;

    // First we convert the transition map into a strict graph.
    for (auto& [k, v] : automata.mTransitionMap) {
        for (unsigned state : v) {
            std::string symbol = std::string(1, k.second);
            auto it = regexLabelMap.find({k.first, state});
            if (it == regexLabelMap.end())
                regexLabelMap[{k.first, state}] = symbol;
            else
                it->second += "|" + symbol;
        }
    }

    auto states = automata.States();
    auto statesCopy = states;

    // We chose state q as the erase state.
    for (unsigned q : states) {
        if (q == newStartState || q == newAcceptState)
            continue;
        statesCopy.erase(q);
        for (unsigned p : statesCopy) {
            for (unsigned r : statesCopy) {
                auto pq = regexLabelMap.find({p, q});
                auto qr = regexLabelMap.find({q, r});
                if (pq == regexLabelMap.end() || qr == regexLabelMap.end())
                    continue;
                auto qq = regexLabelMap.find({q, q});
                auto pr = regexLabelMap.find({p, r});

                std::string pqLabel, qqLabel, qrLabel, prLabel;
                pqLabel = "(" + pq->second + ")";
                if (qq != regexLabelMap.end())
                    qqLabel = "(" + qq->second + ")*";
                qrLabel = qr->second;
                if (pr != regexLabelMap.end())
                    prLabel = "|(" + pr->second + ")";
                regexLabelMap[{p, r}] = pqLabel + qqLabel + qrLabel + prLabel;
            }
        }
    }

    return regexLabelMap[{newStartState, newAcceptState}];
}

FiniteAutomata FiniteAutomata::BinarySetOperation(const FiniteAutomata &other, std::function<bool (bool, bool)> acceptStateCondition) const
{
    std::map<std::pair<unsigned, char>, std::set<unsigned>> transitionMap;
    std::set<unsigned> startStates;
    std::set<unsigned> acceptStates;

    auto alphabetUnion = this->Alphabet();
    alphabetUnion.merge(other.Alphabet());

    FiniteAutomata dfaA = this->CompletionUnderAlphabet(alphabetUnion);
    FiniteAutomata dfaB = other.CompletionUnderAlphabet(alphabetUnion);

    auto statesA = dfaA.States();
    auto statesB = dfaB.States();

    unsigned numOfStatesB = statesB.size();

    for (unsigned stateA : statesA) {
        for (unsigned stateB : statesB) {
            unsigned sourceState = numOfStatesB * stateA + stateB;
            if (dfaA.mStartStates.find(stateA) != dfaA.mStartStates.end() && dfaB.mStartStates.find(stateB) != dfaB.mStartStates.end())
                startStates.insert(sourceState);
            for (char symbol : alphabetUnion) {
                unsigned a = *(dfaA.mTransitionMap[{stateA, symbol}].begin());
                unsigned b = *(dfaB.mTransitionMap[{stateB, symbol}].begin());

                unsigned destinationState = numOfStatesB * a + b;
                transitionMap[{sourceState, symbol}].insert(destinationState);
            }
            if (acceptStateCondition(dfaA.mAcceptStates.find(stateA) != dfaA.mAcceptStates.end(), dfaB.mAcceptStates.find(stateB) != dfaB.mAcceptStates.end()))
                acceptStates.insert(sourceState);
        }
    }

    return FiniteAutomata(transitionMap, startStates, acceptStates);
}

FiniteAutomata FiniteAutomata::Union(const FiniteAutomata& other) const
{
    return BinarySetOperation(other, [](bool a, bool b) {
        return a || b;
    });
}

FiniteAutomata FiniteAutomata::Intersection(const FiniteAutomata& other) const
{
    return BinarySetOperation(other, [](bool a, bool b) {
        return a && b;
    });
}

FiniteAutomata FiniteAutomata::Difference(const FiniteAutomata& other) const
{
    return BinarySetOperation(other, [](bool a, bool b) {
        return a && !b;
    });
}

FiniteAutomata FiniteAutomata::Complement() const
{
    FiniteAutomata complement = Completion();

    auto states = complement.States();

    std::set<unsigned> complementAcceptStates;
    for (unsigned state : states) {
        if (complement.mAcceptStates.find(state) == complement.mAcceptStates.end())
            complementAcceptStates.insert(state);
    }
    complement.mAcceptStates = complementAcceptStates;

    return complement;
}

// A reversed automaton recognizes the reversed strings
// of the original language.
FiniteAutomata FiniteAutomata::Reversal() const
{
    std::map<std::pair<unsigned, char>, std::set<unsigned>> reversedMap;

    // Reverse the transitions.
    for (auto& [k, v] : mTransitionMap) {
        for (unsigned state : v)
            reversedMap[{state, k.second}].insert(k.first);
    }

    // Flip the start and accept states.
    return FiniteAutomata(reversedMap, mAcceptStates, mStartStates);
}

// Implementation of Brzozowski's minimization algorithm.
FiniteAutomata FiniteAutomata::Minimization() const
{
    return this->Reversal().Determinization().Reversal().Determinization();
}

bool FiniteAutomata::IsAccepted(const std::string &s)
{

    auto dfa = this->Determinization();
    unsigned state = *(dfa.StartStates()).begin();
    std::map<std::pair<unsigned int, char>, std::set<unsigned int>> transitionMap = dfa.TransitionMap();
    std::set<unsigned> acceptStates = dfa.AcceptStates();

    for (char c : s) {
        if (transitionMap.find(std::make_pair(state, c)) == transitionMap.end()) {
            return false;
        }
        state = *(transitionMap.find(std::make_pair(state, c))->second.begin());
    }

    for (unsigned as : acceptStates)
        if (as == state)
            return true;
    return false;
}

std::optional<std::string> FiniteAutomata::GenerateRandom()
{
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<size_t> dist{0, 1};

    if (dist(engine)) {
        auto generated = GenerateAccepted();
        if (generated)
            return generated;
        generated = GenerateUnaccepted();
        if (generated)
            return generated;
    }
    else {
        auto generated = GenerateUnaccepted();
        if (generated)
            return generated;
        generated = GenerateAccepted();
        if (generated)
            return generated;
    }

    return std::nullopt;
}

std::optional<std::string> FiniteAutomata::GenerateAccepted()
{
    size_t maxAcceptable = 200;
    unsigned maxStateVisits = 100;

    std::vector<std::string> acceptableStrings;

    FiniteAutomata minimalAutomata = this->Minimization();
    unsigned startState = *(minimalAutomata.StartStates().begin());

    auto alphabet = minimalAutomata.Alphabet();
    auto states = minimalAutomata.States();
    std::map<unsigned, unsigned> stateVisits;
    for (unsigned state : states)
        stateVisits[state] = 0;

    std::queue<std::pair<unsigned, std::string>> traversalQueue;
    traversalQueue.push({startState, ""});

    while (!traversalQueue.empty() && acceptableStrings.size() < maxAcceptable) {
        auto [currentState, currentString] = traversalQueue.front();
        traversalQueue.pop();

        if (minimalAutomata.mAcceptStates.find(currentState) != minimalAutomata.mAcceptStates.end())
                acceptableStrings.push_back(currentString);

        for (char symbol : alphabet) {
            auto it = minimalAutomata.mTransitionMap.find({currentState, symbol});
            if (it != minimalAutomata.mTransitionMap.end()) {
                unsigned newState = *(it->second.begin());
                if (stateVisits[newState] < maxStateVisits) {
                    traversalQueue.push({newState, currentString + symbol});
                    stateVisits[newState]++;
                }
            }
        }
    }

    if (acceptableStrings.size() == 0)
        return std::nullopt;

    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<size_t> dist(0, acceptableStrings.size() - 1);

    return acceptableStrings[dist(engine)];
}

std::optional<std::string> FiniteAutomata::GenerateUnaccepted()
{
    return this->Complement().GenerateAccepted();
}

std::vector<std::set<unsigned>> FiniteAutomata::MatchSimulation(const std::string& str) const
{
    std::set<unsigned> currentStep;
    for (unsigned state : mStartStates)
        currentStep.merge(this->EpsilonClosure(state));
    std::vector<std::set<unsigned>> steps = {currentStep};

    for (char symbol : str) {
        std::set<unsigned> newStep;
        for (unsigned state : currentStep) {
            auto it = mTransitionMap.find({state, symbol});
            if (it != mTransitionMap.end()) {
                for (unsigned transitionedState : it->second)
                    newStep.merge(this->EpsilonClosure(transitionedState));
            }
        }
        steps.push_back(newStep);
        currentStep = newStep;
    }

    return steps;
}
