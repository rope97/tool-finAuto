#include "lib/AutomataNode.hpp"
#include <QPainter>

AutomataGraph::AutomataNode::AutomataNode(AutomataGraph *parentGraph, const QString& name)
    : QGraphicsItem(parentGraph)
    , mGVNode(agnode(parentGraph->mGVGraph, const_cast<char *>(name.toStdString().c_str()), 1))
    , mName(name)
{

}

AutomataGraph::AutomataNodeNormal::AutomataNodeNormal(AutomataGraph *parentGraph, const QString& name)
    : AutomataNode(parentGraph, name)
{

}

QRectF AutomataGraph::AutomataNodeNormal::boundingRect() const
{
    return mPath.boundingRect();
}

void AutomataGraph::AutomataNodeNormal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF br = boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);
    if (mIsActive) {
        painter->setBrush(QColor(0, 255, 255, 120));
        painter->drawEllipse(br);
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(mPath);
    painter->drawText(br, Qt::AlignCenter, mName);
}

void AutomataGraph::AutomataNodeNormal::PathSetup()
{
    // Graphviz coordinates represent the center
    // coordinates of the node, so we have to
    // translate them to the top left coords for Qt.
    qreal dpiAdj = 96.0 / 72.0;
    qreal realWidth = ND_height(mGVNode) * 96;
    qreal realHeight = ND_height(mGVNode) * 96;
    qreal leftX = ND_coord(mGVNode).x * dpiAdj - realWidth / 2.0;
    qreal leftY = ND_coord(mGVNode).y * dpiAdj - realHeight / 2.0;
    mPath.addEllipse(leftX, leftY, realWidth, realHeight);
}

AutomataGraph::AutomataNodeStart::AutomataNodeStart(AutomataGraph *parentGraph, const QString& name)
    : AutomataNode(parentGraph, name)
{
    // A "ghost" node which allows us to have an edge "with no source".
    Agnode_t *ghostSource = agnode(parentGraph->mGVGraph, const_cast<char *>(("hidden_" + name).toStdString().c_str()), 1);
    agsafeset(ghostSource, const_cast<char *>("label"), const_cast<char *>(""), const_cast<char *>(""));
    agsafeset(ghostSource, const_cast<char *>("width"), const_cast<char *>("0"), const_cast<char *>("0"));
    agsafeset(ghostSource, const_cast<char *>("height"), const_cast<char *>("0"), const_cast<char *>("0"));
    mGVStartEdge = agedge(parentGraph->mGVGraph, ghostSource, mGVNode, const_cast<char *>(""), 1);
}

QRectF AutomataGraph::AutomataNodeStart::boundingRect() const
{
    return mPath.boundingRect() | mStartEdgePath.boundingRect();
}

void AutomataGraph::AutomataNodeStart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF br = mPath.boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);
    if (mIsActive) {
        painter->setBrush(QColor(0, 255, 255, 120));
        painter->drawEllipse(br);
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(mStartEdgePath);
    painter->drawPath(mPath);
    painter->drawText(br, Qt::AlignCenter, mName);
}

void AutomataGraph::AutomataNodeStart::PathSetup()
{
    qreal dpiAdj = 96.0 / 72.0;
    qreal realWidth = ND_height(mGVNode) * 96;
    qreal realHeight = ND_height(mGVNode) * 96;
    qreal leftX = ND_coord(mGVNode).x * dpiAdj - realWidth / 2.0;
    qreal leftY = ND_coord(mGVNode).y * dpiAdj - realHeight / 2.0;
    mPath.addEllipse(leftX, leftY, realWidth, realHeight);

    // Add sourceless edge to the path.
    bezier *curveData = ED_spl(mGVStartEdge)->list;

    mStartEdgePath.moveTo(curveData->list[0].x * dpiAdj, curveData->list[0].y * dpiAdj);

    for (int i = 1; i < curveData->size; i += 3) {
        mStartEdgePath.cubicTo(curveData->list[i].x * dpiAdj, curveData->list[i].y * dpiAdj,
                     curveData->list[i + 1].x * dpiAdj, curveData->list[i + 1].y * dpiAdj,
                     curveData->list[i + 2].x * dpiAdj, curveData->list[i + 2].y * dpiAdj);
    }

    if (curveData->eflag) {
        mStartEdgePath.lineTo(curveData->ep.x * dpiAdj, curveData->ep.y * dpiAdj);

        QPolygonF arrowhead;
        QPointF lastLineStart = QPointF(curveData->list[curveData->size - 1].x * dpiAdj,
                                        curveData->list[curveData->size - 1].y * dpiAdj);
        QPointF lastLineEnd = QPointF(curveData->ep.x * dpiAdj, curveData->ep.y * dpiAdj);
        QLineF normal = QLineF(lastLineStart, lastLineEnd).normalVector();

        QPointF arrLen = QPointF(normal.dx() / 2.0, normal.dy() / 2.0);
        QPointF p1 = lastLineStart - arrLen;
        QPointF p2 = lastLineEnd;
        QPointF p3 = lastLineStart + arrLen;
        arrowhead.append(p1);
        arrowhead.append(p2);
        arrowhead.append(p3);
        mStartEdgePath.addPolygon(arrowhead);
    }
}

AutomataGraph::AutomataNodeAccept::AutomataNodeAccept(AutomataGraph *parentGraph, const QString& name)
    : AutomataNode(parentGraph, name)
{

}

QRectF AutomataGraph::AutomataNodeAccept::boundingRect() const
{
    return mPath.boundingRect();
}

void AutomataGraph::AutomataNodeAccept::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF br = boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);
    if (mIsActive) {
        painter->setBrush(QColor(0, 255, 255, 120));
        painter->drawEllipse(br);
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(mPath);
    painter->drawText(br, Qt::AlignCenter, mName);
}

void AutomataGraph::AutomataNodeAccept::PathSetup()
{
    qreal dpiAdj = 96.0 / 72.0;
    qreal realWidth = ND_height(mGVNode) * 96;
    qreal realHeight = ND_height(mGVNode) * 96;
    qreal leftX = ND_coord(mGVNode).x * dpiAdj - realWidth / 2.0;
    qreal leftY = ND_coord(mGVNode).y * dpiAdj - realHeight / 2.0;
    mPath.addEllipse(leftX, leftY, realWidth, realHeight);
    qreal ringSpace = 4;
    mPath.addEllipse(leftX + ringSpace, leftY + ringSpace,
                      realWidth - 2 * ringSpace, realHeight - 2 * ringSpace);
}

AutomataGraph::AutomataNodeStartAccept::AutomataNodeStartAccept(AutomataGraph *parentGraph, const QString& name)
    : AutomataNode(parentGraph, name)
{
    Agnode_t *ghostSource = agnode(parentGraph->mGVGraph, const_cast<char *>(("hidden_" + name).toStdString().c_str()), 1);
    agsafeset(ghostSource, const_cast<char *>("label"), const_cast<char *>(""), const_cast<char *>(""));
    agsafeset(ghostSource, const_cast<char *>("width"), const_cast<char *>("0"), const_cast<char *>("0"));
    agsafeset(ghostSource, const_cast<char *>("height"), const_cast<char *>("0"), const_cast<char *>("0"));
    mGVStartEdge = agedge(parentGraph->mGVGraph, ghostSource, mGVNode, const_cast<char *>(""), 1);
}

QRectF AutomataGraph::AutomataNodeStartAccept::boundingRect() const
{
    return mPath.boundingRect() | mStartEdgePath.boundingRect();
}

void AutomataGraph::AutomataNodeStartAccept::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF br = mPath.boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);
    if (mIsActive) {
        painter->setBrush(QColor(0, 255, 255, 120));
        painter->drawEllipse(br);
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(mStartEdgePath);
    painter->drawPath(mPath);
    painter->drawText(br, Qt::AlignCenter, mName);
}

void AutomataGraph::AutomataNodeStartAccept::PathSetup()
{
    qreal dpiAdj = 96.0 / 72.0;
    qreal realWidth = ND_height(mGVNode) * 96;
    qreal realHeight = ND_height(mGVNode) * 96;
    qreal leftX = ND_coord(mGVNode).x * dpiAdj - realWidth / 2.0;
    qreal leftY = ND_coord(mGVNode).y * dpiAdj - realHeight / 2.0;
    mPath.addEllipse(leftX, leftY, realWidth, realHeight);
    qreal ringSpace = 4;
    mPath.addEllipse(leftX + ringSpace, leftY + ringSpace,
                      realWidth - 2 * ringSpace, realHeight - 2 * ringSpace);

    bezier *curveData = ED_spl(mGVStartEdge)->list;

    mStartEdgePath.moveTo(curveData->list[0].x * dpiAdj, curveData->list[0].y * dpiAdj);

    for (int i = 1; i < curveData->size; i += 3) {
        mStartEdgePath.cubicTo(curveData->list[i].x * dpiAdj, curveData->list[i].y * dpiAdj,
                     curveData->list[i + 1].x * dpiAdj, curveData->list[i + 1].y * dpiAdj,
                     curveData->list[i + 2].x * dpiAdj, curveData->list[i + 2].y * dpiAdj);
    }

    if (curveData->eflag) {
        mStartEdgePath.lineTo(curveData->ep.x * dpiAdj, curveData->ep.y * dpiAdj);

        QPolygonF arrowhead;
        QPointF lastLineStart = QPointF(curveData->list[curveData->size - 1].x * dpiAdj,
                                        curveData->list[curveData->size - 1].y * dpiAdj);
        QPointF lastLineEnd = QPointF(curveData->ep.x * dpiAdj, curveData->ep.y * dpiAdj);
        QLineF normal = QLineF(lastLineStart, lastLineEnd).normalVector();

        QPointF arrLen = QPointF(normal.dx() / 2.0, normal.dy() / 2.0);
        QPointF p1 = lastLineStart - arrLen;
        QPointF p2 = lastLineEnd;
        QPointF p3 = lastLineStart + arrLen;
        arrowhead.append(p1);
        arrowhead.append(p2);
        arrowhead.append(p3);
        mStartEdgePath.addPolygon(arrowhead);
    }
}
