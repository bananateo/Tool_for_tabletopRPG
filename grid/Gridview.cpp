#include "gridview.h"
#include <QWheelEvent>
#include <QtGlobal>

GridView::GridView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

void GridView::applyZoom(qreal factor)
{
    const qreal newZoom = qBound(kMinZoom, m_zoom * factor, kMaxZoom);
    factor = newZoom / m_zoom;
    if (qFuzzyCompare(factor, 1.0)) return;
    m_zoom = newZoom;
    scale(factor, factor);
}

void GridView::wheelEvent(QWheelEvent *event)
{
    const int dy = event->angleDelta().y();
    if (dy == 0) { event->ignore(); return; }
    applyZoom(dy > 0 ? kStep : 1.0 / kStep);
    event->accept();
}

void GridView::zoomIn()  { applyZoom(kStep); }
void GridView::zoomOut() { applyZoom(1.0 / kStep); }
void GridView::resetZoom()
{
    resetTransform();
    m_zoom = 1.0;
}