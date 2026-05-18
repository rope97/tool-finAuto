#ifndef AUTOMATA_MODEL_HPP
#define AUTOMATA_MODEL_HPP

#include <queue>
#include <map>
#include "FiniteAutomata.hpp"

class AutomataModel
{
public:
    AutomataModel();

    unsigned AddAutomata(FiniteAutomata& automata);
    std::optional<FiniteAutomata> RemoveAutomata(unsigned id);
    void RemoveAllAutomata();
    std::optional<FiniteAutomata> GetAutomata(unsigned id);
    std::map<unsigned, FiniteAutomata> GetAllAutomata();

private:
    unsigned mIDCounter = 0;
    std::queue<unsigned> mFreeIDs;
    std::map<unsigned, FiniteAutomata> mAutomatas;
};

#endif // AUTOMATA_MODEL_HPP
