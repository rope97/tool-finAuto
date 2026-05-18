#include "lib/AutomataModel.hpp"
#include <QDebug>

AutomataModel::AutomataModel()
{

}

unsigned AutomataModel::AddAutomata(FiniteAutomata& automata)
{
    unsigned id;
    if (mFreeIDs.empty()) {
        id = mIDCounter;
        mIDCounter++;
    }
    else {
        id = mFreeIDs.front();
        mFreeIDs.pop();
    }
    mAutomatas[id] = automata;

    return id;
}

std::optional<FiniteAutomata> AutomataModel::RemoveAutomata(unsigned id)
{
    if(mAutomatas.find(id)==mAutomatas.end())
    {
        return std::nullopt;
    }
    std::optional<FiniteAutomata> tempAutomata = mAutomatas[id];
    mAutomatas.erase(id);
    mFreeIDs.push(id);
    return tempAutomata;
}

void AutomataModel::RemoveAllAutomata()
{
    mAutomatas.clear();
    mFreeIDs = std::queue<unsigned>();
    mIDCounter = 0;
}

std::optional<FiniteAutomata> AutomataModel::GetAutomata(unsigned id)
{
    if(mAutomatas.find(id)==mAutomatas.end())
    {
        return std::nullopt;
    }
    return mAutomatas[id];
}

std::map<unsigned, FiniteAutomata> AutomataModel::GetAllAutomata()
{
    return mAutomatas;
}
