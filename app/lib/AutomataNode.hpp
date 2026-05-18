#ifndef AUTOMATA_NODE_HPP
#define AUTOMATA_NODE_HPP

#include <QGraphicsItem>
#include <QPainterPath>
#include <graphviz/gvc.h>

#include "AutomataGraph.hpp"

// Nested class of 'AutomataGraph' representing
// a single node.
class AutomataGraph::AutomataNode : public QGraphicsItem
{
    // Edges sometimes have to access the contents of their nodes.
    friend class AutomataGraph::AutomataEdge;
    friend class AutomataGraph;
public:
    AutomataNode(AutomataGraph *parentGraph, const QString& name);

protected:
    Agnode_t *mGVNode;
    QString mName;
    QPainterPath mPath;

    // Flag for checking whether node is active
    // in current string match simulation.
    bool mIsActive = false;

private:
    virtual void PathSetup() = 0;
};

class AutomataGraph::AutomataNodeNormal : public AutomataGraph::AutomataNode
{
public:
    AutomataNodeNormal(AutomataGraph *parentGraph, const QString& name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void PathSetup() override;
};

class AutomataGraph::AutomataNodeStart : public AutomataGraph::AutomataNode
{
public:
    AutomataNodeStart(AutomataGraph *parentGraph, const QString& name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void PathSetup() override;

    QPainterPath mStartEdgePath;
    Agedge_t *mGVStartEdge;
};

class AutomataGraph::AutomataNodeAccept : public AutomataGraph::AutomataNode
{
public:
    AutomataNodeAccept(AutomataGraph *parentGraph, const QString& name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void PathSetup() override;
};

// It'd make sense for AutomataNodeStartAccept to inherit from both AutomataNodeStart
// and AutomataNodeAccept, but for drawing and setup efficiency, it won't.
class AutomataGraph::AutomataNodeStartAccept : public AutomataGraph::AutomataNode
{
public:
    AutomataNodeStartAccept(AutomataGraph *parentGraph, const QString& name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void PathSetup() override;

    QPainterPath mStartEdgePath;
    Agedge_t *mGVStartEdge;
};

#endif // AUTOMATA_NODE_HPP
