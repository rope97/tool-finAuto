#ifndef AUTOMATA_GRAPH_HPP
#define AUTOMATA_GRAPH_HPP

#include <QGraphicsItem>
#include <QSharedPointer>
#include <graphviz/gvc.h>
#include <vector>
#include <set>

// Represents a single finite automata's
// graph representation. It uses the graphviz
// library to provide the positions for node
// and edge placements, but the actual rendering
// is handled with this class.
class AutomataGraph : public QGraphicsItem
{
public:
    enum class NodeState
    {
        Normal,
        Start,
        Accept,
        StartAccept
    };

    AutomataGraph(unsigned id);
    ~AutomataGraph();

    unsigned GetID() const;
    QPointF GetCenterPos() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void AddNode(const QString& name, NodeState state);
    void AddEdge(const QString& src, const QString& dst, const QString& label);

    void ApplyLayout();

    bool IsSimulated() const;

    void StartMatchSimulation(const std::vector<std::set<unsigned>>& simulationSteps);
    void StopMatchSimulation();
    void MatchSimulationForward();
    void MatchSimulationBack();
    void MatchSimulationReset();
    void MatchSimulationSkip();

private:
    // Nodes and edges make no sense without
    // a parent graph, thus they are nested classes.
    class AutomataNode;
    class AutomataNodeNormal;
    class AutomataNodeStart;
    class AutomataNodeAccept;
    class AutomataNodeStartAccept;
    class AutomataEdge;

    static GVC_t *mGVContext;

    Agraph_t *mGVGraph;
    QMap<QString, QSharedPointer<AutomataNode>> mNodes;
    QMap<QPair<QString, QString>, QSharedPointer<AutomataEdge>> mEdges;

    unsigned mID;

    bool mIsSimulationActive = false;
    std::vector<std::set<unsigned>> mSimulationSteps;
    unsigned mCurrentSimulationStep = 0;

    void DisableCurrent();
    void EnableCurrent();
};

#endif // AUTOMATA_GRAPH_HPP
