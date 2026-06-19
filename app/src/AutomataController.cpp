#include "lib/AutomataController.hpp"
#include "lib/TLSAutomata.hpp"

AutomataController::AutomataController()
{

}

AutomataController &AutomataController::GetController()
{
    static AutomataController controller{};
    return controller;
}

/* creating new automata */
unsigned AutomataController::CreateNFA(AutomataModel& model, const std::string& regex)
{
    FiniteAutomata automata = FiniteAutomata(regex);
    return model.AddAutomata(automata);
}

unsigned AutomataController::CreateTLS(AutomataModel& model, std::string&)
{
    FiniteAutomata tls = CreateTLSAutomata();
    return model.AddAutomata(tls);
}


unsigned AutomataController::CreateDFA(AutomataModel& model, const std::string& regex)
{
    FiniteAutomata automata = FiniteAutomata(regex).Determinization();
    return model.AddAutomata(automata);
}

unsigned AutomataController::CreateMDFA(AutomataModel& model, const std::string& regex)
{
    FiniteAutomata automata = FiniteAutomata(regex).Minimization();
    return model.AddAutomata(automata);
}

unsigned AutomataController::CreateA(AutomataModel& model, const FiniteAutomata& automata)
{
    FiniteAutomata aut = FiniteAutomata(automata);
    return model.AddAutomata(aut);
}

/* modify active automata */
unsigned AutomataController::ToDFA(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    automata = automata.Determinization();
    model.RemoveAutomata(id);
    return model.AddAutomata(automata);
}

unsigned AutomataController::ToMDFA(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    automata = automata.Minimization();
    model.RemoveAutomata(id);
    return model.AddAutomata(automata);
}

unsigned AutomataController::Reverse(AutomataModel &model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    automata = automata.Reversal();
    model.RemoveAutomata(id);
    return model.AddAutomata(automata);
}

unsigned AutomataController::Complete(AutomataModel &model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    automata = automata.Completion();
    model.RemoveAutomata(id);
    return model.AddAutomata(automata);
}

unsigned AutomataController::Complement(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    automata = automata.Complement();
    model.RemoveAutomata(id);
    return model.AddAutomata(automata);
}

/* operations */
unsigned AutomataController::Union(AutomataModel& model, const std::vector<unsigned> &l)
{
    if (l.size() < 2)
        return 0;

    FiniteAutomata result = *model.GetAutomata(l[0]);
    model.RemoveAutomata(l[0]);
    for (size_t i = 1; i < l.size(); i++) {
        result = result.Union(*model.GetAutomata(l[i]));
        model.RemoveAutomata(l[i]);
    }

    return model.AddAutomata(result);
}

unsigned AutomataController::Intersection(AutomataModel& model, const std::vector<unsigned> &l)
{
    if (l.size() < 2)
        return 0;

    FiniteAutomata result = *model.GetAutomata(l[0]);
    model.RemoveAutomata(l[0]);
    for (size_t i = 1; i < l.size(); i++) {
        result = result.Intersection(*model.GetAutomata(l[i]));
        model.RemoveAutomata(l[i]);
    }

    return model.AddAutomata(result);
}

unsigned AutomataController::Difference(AutomataModel& model, const std::vector<unsigned> &l)
{
    if (l.size() < 2)
        return 0;

    FiniteAutomata result = *model.GetAutomata(l[0]);
    model.RemoveAutomata(l[0]);
    for (size_t i = 1; i < l.size(); i++) {
        result = result.Difference(*model.GetAutomata(l[i]));
        model.RemoveAutomata(l[i]);
    }

    return model.AddAutomata(result);
}

/* acceptable strings */
bool AutomataController::CheckIsAccepted(AutomataModel& model, unsigned id, const std::string &s)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    return automata.IsAccepted(s);
}

std::optional<std::string> AutomataController::GetRandom(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    return automata.GenerateRandom();
}

std::optional<std::string> AutomataController::GetAccepted(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    return automata.GenerateAccepted();
}

std::optional<std::string> AutomataController::GetUnaccepted(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    return automata.GenerateUnaccepted();
}

unsigned AutomataController::CloneAutomata(AutomataModel& model, unsigned id)
{
    FiniteAutomata automata = *model.GetAutomata(id);
    return model.AddAutomata(automata);
}

void AutomataController::RemoveAutomata(AutomataModel& model, unsigned id)
{
    model.RemoveAutomata(id);
}
