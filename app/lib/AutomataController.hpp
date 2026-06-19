#ifndef AUTOMATA_CONTROLLER_HPP
#define AUTOMATA_CONTROLLER_HPP

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <set>
#include <optional>

#include "FiniteAutomata.hpp"
#include "AutomataModel.hpp"

class AutomataController {
public:
    static AutomataController& GetController();
    unsigned CreateTLS(AutomataModel& model, std::string&);
    unsigned CreateNFA(AutomataModel& model, const std::string& regex);
    unsigned CreateDFA(AutomataModel& model, const std::string& regex);
    unsigned CreateMDFA(AutomataModel& model, const std::string& regex);
    unsigned CreateA(AutomataModel& model, const FiniteAutomata& automata);

    unsigned ToDFA(AutomataModel& model, unsigned id);
    unsigned ToMDFA(AutomataModel& model, unsigned id);
    unsigned Reverse(AutomataModel& model, unsigned id);
    unsigned Complete(AutomataModel& model, unsigned id);
    unsigned Complement(AutomataModel& model, unsigned id);

    unsigned Union(AutomataModel& model, const std::vector<unsigned> &l);
    unsigned Intersection(AutomataModel& model, const std::vector<unsigned> &l);
    unsigned Difference(AutomataModel& model, const std::vector<unsigned> &l);
   
    bool CheckIsAccepted(AutomataModel& model, unsigned id, const std::string &s);
    std::optional<std::string> GetRandom(AutomataModel& model, unsigned id);
    std::optional<std::string> GetAccepted(AutomataModel& model, unsigned id);
    std::optional<std::string> GetUnaccepted(AutomataModel& model, unsigned id);
   
    unsigned CloneAutomata(AutomataModel& model, const unsigned id);
    void RemoveAutomata(AutomataModel& model, const unsigned id);

private:
    AutomataController();
    AutomataController(const AutomataController&) = delete;
    AutomataController& operator=(const AutomataController&) = delete;
};

#endif // AUTOMATA_CONTROLLER_HPP
