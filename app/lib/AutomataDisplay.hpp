#ifndef AUTOMATA_DISPLAY_HPP
#define AUTOMATA_DISPLAY_HPP

#include <QGraphicsScene>
#include <vector>
#include "AutomataGraph.hpp"
#include "FiniteAutomata.hpp"

class AutomataDisplay : public QGraphicsScene
{
public:
    explicit AutomataDisplay(QObject *parent = nullptr);

    void AddAutomata(const FiniteAutomata& automata, unsigned id, qreal xCenter, qreal yCenter);
    void AddAutomata(const FiniteAutomata& automata, unsigned id, QPointF center);

    AutomataGraph* GetAutomataByID(unsigned id);
    std::vector<AutomataGraph *> GetSelectedAutomata();
    std::vector<AutomataGraph *> GetSimulatedAutomata();
    std::vector<unsigned> GetSelectedIDs(const std::vector<AutomataGraph *>& selected);
};

#endif // AUTOMATA_DISPLAY_HPP
