#ifndef AUTOMATA_EDGE_HPP
#define AUTOMATA_EDGE_HPP

#include <QGraphicsItem>
#include <QPainterPath>
#include <graphviz/gvc.h>

#include "AutomataGraph.hpp"

class AutomataGraph::AutomataEdge : public QGraphicsItem
{
    friend class AutomataGraph;
public:
    AutomataEdge(AutomataGraph *parentGraph, const QString& src, const QString& dst, const QString& label);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Agedge_t *mGVEdge;
    QString mSrc, mDst, mLabel;

    QPainterPath mPath;
    void PathSetup();
};

#endif // AUTOMATA_EDGE_HPP
