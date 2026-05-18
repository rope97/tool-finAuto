#include "lib/AutomataEdge.hpp"
#include "lib/AutomataNode.hpp"
#include <QPainter>
#include <QPainterPath>

AutomataGraph::AutomataEdge::AutomataEdge(AutomataGraph *parentGraph, const QString& src, const QString& dst, const QString& label)
    : QGraphicsItem(parentGraph)
    , mGVEdge(agedge(parentGraph->mGVGraph, parentGraph->mNodes[src]->mGVNode, parentGraph->mNodes[dst]->mGVNode, const_cast<char *>(label.toStdString().c_str()), 1))
    , mSrc(src)
    , mDst(dst)
    , mLabel(label)
{
    // Set the edge label graphviz attribute.
    agsafeset(mGVEdge, const_cast<char *>("xlabel"), const_cast<char *>(label.toStdString().c_str()), const_cast<char *>(""));
}

QRectF AutomataGraph::AutomataEdge::boundingRect() const
{
    // We have to the edge's label bounding rectangle
    // into consideration.
    QFont gvFont(ED_xlabel(mGVEdge)->fontname, ED_xlabel(mGVEdge)->fontsize);
    QFontMetrics gvFontMetrics(gvFont);
    QRectF br = gvFontMetrics.boundingRect(mLabel);
    // Translate graphviz's center coordinates into Qt's top left coordinates.
    qreal dpiAdj = 96.0 / 72.0;
    qreal leftX = ED_xlabel(mGVEdge)->pos.x * dpiAdj - ED_xlabel(mGVEdge)->dimen.x / 2.0;
    qreal leftY = ED_xlabel(mGVEdge)->pos.y * dpiAdj + ED_xlabel(mGVEdge)->dimen.y / 2.0;
    br.translate(leftX, leftY);
    return mPath.boundingRect() | br;
}

void AutomataGraph::AutomataEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(mPath);

    QFont gvFont(ED_xlabel(mGVEdge)->fontname, ED_xlabel(mGVEdge)->fontsize);
    painter->setFont(gvFont);
    // Translate graphviz's center coordinates into Qt's top left coordinates.
    qreal dpiAdj = 96.0 / 72.0;
    qreal leftX = ED_xlabel(mGVEdge)->pos.x * dpiAdj - ED_xlabel(mGVEdge)->dimen.x / 2.0;
    qreal leftY = ED_xlabel(mGVEdge)->pos.y * dpiAdj + ED_xlabel(mGVEdge)->dimen.y / 2.0;
    painter->drawText(QPointF(leftX, leftY), mLabel);
}

void AutomataGraph::AutomataEdge::PathSetup() {
    qreal dpiAdj = 96.0 / 72.0;

    bezier *curveData = ED_spl(mGVEdge)->list;

    mPath.moveTo(curveData->list[0].x * dpiAdj, curveData->list[0].y * dpiAdj);

    for (int i = 1; i < curveData->size; i += 3) {
        mPath.cubicTo(curveData->list[i].x * dpiAdj, curveData->list[i].y * dpiAdj,
                     curveData->list[i + 1].x * dpiAdj, curveData->list[i + 1].y * dpiAdj,
                     curveData->list[i + 2].x * dpiAdj, curveData->list[i + 2].y * dpiAdj);
    }

    if (curveData->eflag) {
        mPath.lineTo(curveData->ep.x * dpiAdj, curveData->ep.y * dpiAdj);

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
        mPath.addPolygon(arrowhead);
    }
}
