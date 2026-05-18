#include "lib/AutomataGraph.hpp"
#include "lib/AutomataNode.hpp"
#include "lib/AutomataEdge.hpp"
#include <QPainter>

// We only have one context for every graph,
// so it makes sense to make it static.
GVC_t *AutomataGraph::mGVContext = gvContext();

AutomataGraph::AutomataGraph(unsigned id)
    : QGraphicsItem()
    , mGVGraph(agopen(const_cast<char *>(std::to_string(id).c_str()), Agstrictdirected, NULL))
    , mID(id)
{
    agsafeset(mGVGraph, const_cast<char *>("splines"), const_cast<char *>("true"), const_cast<char *>("true"));
    agsafeset(mGVGraph, const_cast<char *>("rankdir"), const_cast<char *>("LR"), const_cast<char *>("LR"));
    agsafeset(mGVGraph, const_cast<char *>("dpi"), const_cast<char *>("96"), const_cast<char *>("96"));

    agattr(mGVGraph, AGNODE, const_cast<char *>("fixedsize"), const_cast<char *>("true"));
    agattr(mGVGraph, AGNODE, const_cast<char *>("width"), const_cast<char *>("0.55"));
    agattr(mGVGraph, AGNODE, const_cast<char *>("height"), const_cast<char *>("0.55"));

    // Set font for every label in the graph.
    agattr(mGVGraph, AGRAPH, const_cast<char *>("fontsize"), const_cast<char *>("12"));
    agattr(mGVGraph, AGNODE, const_cast<char *>("fontsize"), const_cast<char *>("12"));
    agattr(mGVGraph, AGEDGE, const_cast<char *>("fontsize"), const_cast<char *>("12"));
    agattr(mGVGraph, AGRAPH, const_cast<char *>("fontname"), const_cast<char *>("Arial"));
    agattr(mGVGraph, AGNODE, const_cast<char *>("fontname"), const_cast<char *>("Arial"));
    agattr(mGVGraph, AGEDGE, const_cast<char *>("fontname"), const_cast<char *>("Arial"));

    // Qt flags to make the graph draggable across the scene.
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

AutomataGraph::~AutomataGraph()
{
    gvFreeLayout(mGVContext, mGVGraph);
}

unsigned AutomataGraph::GetID() const
{
    return mID;
}

QPointF AutomataGraph::GetCenterPos() const
{
    QRectF br = this->boundingRect();
    return {this->x() + br.width() / 2.0, this->y() + br.height() / 2.0};
}

QRectF AutomataGraph::boundingRect() const
{
    qreal dpiAdj = 96.0 / 72.0;
    return QRectF(GD_bb(mGVGraph).LL.x * dpiAdj, GD_bb(mGVGraph).LL.y * dpiAdj,
                  GD_bb(mGVGraph).UR.x * dpiAdj, GD_bb(mGVGraph).UR.y * dpiAdj);
}

void AutomataGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (isSelected())
        painter->drawRect(boundingRect());
}

void AutomataGraph::AddNode(const QString& name, NodeState state)
{
    // If a node with that name already exists
    // in the graph, do nothing.
    if (mNodes.contains(name))
        return;
    switch (state) {
        case NodeState::Normal:
            mNodes[name] = QSharedPointer<AutomataNodeNormal>(new AutomataNodeNormal(this, name));
            break;
        case NodeState::Start:
            mNodes[name] = QSharedPointer<AutomataNodeStart>(new AutomataNodeStart(this, name));
            break;
        case NodeState::Accept:
            mNodes[name] = QSharedPointer<AutomataNodeAccept>(new AutomataNodeAccept(this, name));
            break;
        case NodeState::StartAccept:
            mNodes[name] = QSharedPointer<AutomataNodeStartAccept>(new AutomataNodeStartAccept(this, name));
            break;
    }
}

void AutomataGraph::AddEdge(const QString& src, const QString& dst, const QString& label)
{
    // If an edge with the same label already exists
    // between the src and dst nodes, do nothing.
    if (mEdges.contains({src, dst}))
        return;
    // In case the src and dst nodes don't exist,
    // create them.
    if (!mNodes.contains(src))
        AddNode(src, NodeState::Normal);
    if (!mNodes.contains(dst))
        AddNode(dst, NodeState::Normal);
    mEdges[{src, dst}] = QSharedPointer<AutomataEdge>(new AutomataEdge(this, src, dst, label));
}

void AutomataGraph::ApplyLayout()
{
    gvFreeLayout(mGVContext, mGVGraph);
    gvLayout(mGVContext, mGVGraph, "dot");

    for (auto it = mEdges.begin(); it != mEdges.end(); it++)
        it.value()->PathSetup();

    for (auto it = mNodes.begin(); it != mNodes.end(); it++)
        it.value()->PathSetup();
}

bool AutomataGraph::IsSimulated() const
{
    return mIsSimulationActive;
}

void AutomataGraph::StartMatchSimulation(const std::vector<std::set<unsigned> > &simulationSteps)
{
    if (mSimulationSteps.size() > 0)
        this->DisableCurrent();
    mIsSimulationActive = true;
    mSimulationSteps = simulationSteps;
    mCurrentSimulationStep = 0;
    this->EnableCurrent();
}

void AutomataGraph::StopMatchSimulation()
{
    if (mSimulationSteps.size() > 0)
        this->DisableCurrent();
    mIsSimulationActive = false;
    mSimulationSteps.clear();
    mCurrentSimulationStep = 0;
}

void AutomataGraph::MatchSimulationForward()
{
    if (mCurrentSimulationStep + 1 < mSimulationSteps.size()) {
        this->DisableCurrent();
        mCurrentSimulationStep++;
        this->EnableCurrent();
    }
}

void AutomataGraph::MatchSimulationBack()
{
    if (mCurrentSimulationStep > 0) {
        this->DisableCurrent();
        mCurrentSimulationStep--;
        this->EnableCurrent();
    }
}

void AutomataGraph::MatchSimulationReset()
{
    this->DisableCurrent();
    mCurrentSimulationStep = 0;
    this->EnableCurrent();
}

void AutomataGraph::MatchSimulationSkip()
{
    this->DisableCurrent();
    mCurrentSimulationStep = mSimulationSteps.size() - 1;
    this->EnableCurrent();
}

void AutomataGraph::DisableCurrent()
{
    for (unsigned state : mSimulationSteps[mCurrentSimulationStep]) {
        mNodes[QString::number(state)]->mIsActive = false;
        mNodes[QString::number(state)]->update();
    }
}

void AutomataGraph::EnableCurrent()
{
    for (unsigned state : mSimulationSteps[mCurrentSimulationStep]) {
        mNodes[QString::number(state)]->mIsActive = true;
        mNodes[QString::number(state)]->update();
    }
}
