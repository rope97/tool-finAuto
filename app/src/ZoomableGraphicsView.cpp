#include "lib/ZoomableGraphicsView.hpp"
#include <QWheelEvent>

ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

void ZoomableGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal scaleFactor = 1.1;
    qreal maxScaleFactor = 1.5;
    qreal minScaleFactor = 0.35;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    if (event->angleDelta().y() > 0) {
        qreal newTotalScaleFactor = mTotalScaleFactor * scaleFactor;
        if (newTotalScaleFactor < maxScaleFactor) {
            scale(scaleFactor, scaleFactor);
            mTotalScaleFactor = newTotalScaleFactor;
        }
    }
    else if (event->angleDelta().y() < 0) {
        qreal newTotalScaleFactor = mTotalScaleFactor / scaleFactor;
        if (newTotalScaleFactor > minScaleFactor) {
            scale(1 / scaleFactor, 1 / scaleFactor);
            mTotalScaleFactor = newTotalScaleFactor;
        }
    }
}

void ZoomableGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        this->setDragMode(QGraphicsView::RubberBandDrag);
    QGraphicsView::keyPressEvent(event);
}

void ZoomableGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    QGraphicsView::keyReleaseEvent(event);
}
