#include "lib/AutomataDisplay.hpp"
#include "lib/RegexDriver.hpp"
#include <map>
#include <algorithm>
#include <iterator>

AutomataDisplay::AutomataDisplay(QObject *parent)
    : QGraphicsScene(parent)
{

}

// Note: The coordinates passed here are the center coordinates for the graph,
// not the usual top left coordinates.
void AutomataDisplay::AddAutomata(const FiniteAutomata &automata, unsigned id, qreal xCenter = 0, qreal yCenter = 0)
{
    AutomataGraph *testGraph = new AutomataGraph(id);

    auto transitionMap = automata.TransitionMap();
    auto startStates = automata.StartStates();
    auto acceptStates = automata.AcceptStates();

    for (unsigned state : startStates) {
        if (acceptStates.find(state) != acceptStates.end())
            testGraph->AddNode(QString::number(state), AutomataGraph::NodeState::StartAccept);
        else
            testGraph->AddNode(QString::number(state), AutomataGraph::NodeState::Start);
    }

    for (unsigned state : acceptStates)
        testGraph->AddNode(QString::number(state), AutomataGraph::NodeState::Accept);

    std::map<std::pair<unsigned, unsigned>, QString> strictTransitionMap;

    for (auto& [k, v] : transitionMap) {
        for (unsigned state : v) {
            if (k.second == RegexDriver::epsilon)
                strictTransitionMap[{k.first, state}] += "eps ";
            else
                strictTransitionMap[{k.first, state}] += QString(k.second) + " ";
        }
    }

    for (auto& [k, v] : strictTransitionMap)
        testGraph->AddEdge(QString::number(k.first), QString::number(k.second), v);

    testGraph->ApplyLayout();

    QRectF br = testGraph->boundingRect();
    testGraph->setPos(xCenter - br.width() / 2.0,
                      yCenter - br.height() / 2.0);

    this->addItem(testGraph);
}

void AutomataDisplay::AddAutomata(const FiniteAutomata &automata, unsigned id, QPointF center)
{
    this->AddAutomata(automata, id, center.x(), center.y());
}

AutomataGraph* AutomataDisplay::GetAutomataByID(unsigned id)
{
    auto items = this->items();

    for(auto item : items) {
        AutomataGraph *graph = dynamic_cast<AutomataGraph *>(item);
        if(graph && graph->GetID() == id)
            return graph;
    }

    return nullptr;
}

std::vector<AutomataGraph *> AutomataDisplay::GetSelectedAutomata()
{
    std::vector<AutomataGraph *> selectedAutomata;
    QList selectedItems = this->selectedItems();

    for (auto& item : selectedItems) {
        AutomataGraph *graph = dynamic_cast<AutomataGraph *>(item);
        if (graph)
            selectedAutomata.push_back(graph);
    }

    return selectedAutomata;
}

std::vector<AutomataGraph *> AutomataDisplay::GetSimulatedAutomata()
{
    std::vector<AutomataGraph *> simulatedAutomata;
    QList items = this->items();

    for (auto& item : items) {
        AutomataGraph *graph = dynamic_cast<AutomataGraph *>(item);
        if (graph && graph->IsSimulated())
            simulatedAutomata.push_back(graph);
    }

    return simulatedAutomata;
}

std::vector<unsigned> AutomataDisplay::GetSelectedIDs(const std::vector<AutomataGraph *>& selected)
{
    std::vector<unsigned> selectedID;
    std::transform(selected.begin(), selected.end(), std::back_inserter(selectedID),
                   [] (const AutomataGraph *a) {return a->GetID();}
                   );
    return selectedID;
}
