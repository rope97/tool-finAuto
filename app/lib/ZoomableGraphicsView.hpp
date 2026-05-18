#ifndef ZOOMABLE_GRAPHICS_VIEW_HPP
#define ZOOMABLE_GRAPHICS_VIEW_HPP

#include <QGraphicsView>
#include <QWidget>

class ZoomableGraphicsView : public QGraphicsView
{
public:
    ZoomableGraphicsView(QWidget *parent = nullptr);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    qreal mTotalScaleFactor = 1;
};

#endif // ZOOMABLE_GRAPHICS_VIEW_HPP
