#include "lib/FiniteAutomata.hpp"

FiniteAutomata CreateTLSAutomata()
{
    std::map<std::pair<unsigned, char>, std::set<unsigned>> transitions;

    // TLS-like handshake states
    unsigned C = 0; // ClientHello
    unsigned S = 1; // ServerHello
    unsigned K = 2; // KeyExchange
    unsigned V = 3; // Verification
    unsigned F = 4; // Finished
    unsigned A = 5; // Error state

    // Normal TLS flow: C -> S -> K -> V -> F
    transitions[{C, 'c'}].insert(S);
    transitions[{S, 's'}].insert(K);
    transitions[{K, 'k'}].insert(V);
    transitions[{V, 'v'}].insert(F);

    // Optional “session resume / shortcut path”
    transitions[{S, 's'}].insert(V);
    transitions[{V, 'v'}].insert(F);

    // Error transitions
    transitions[{C, 'x'}].insert(A);
    transitions[{A, 'a'}].insert(A);

    std::set<unsigned> startStates = {C};
    std::set<unsigned> acceptStates = {F};

    return FiniteAutomata(transitions, startStates, acceptStates);
}
