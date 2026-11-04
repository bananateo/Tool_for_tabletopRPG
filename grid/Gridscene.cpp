#include "gridscene.h"

#include <QPainter>
#include <QPen>
#include <QVector>
#include <QLineF>
#include <cmath>

GridScene::GridScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(-100000, -100000, 200000, 200000);
}

void GridScene::setGridSize(int size)
{
    if (size < 1 || size == m_gridSize)
        return;
    m_gridSize = size;
    update();
}

void GridScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QColor(30, 30, 30));

    const qreal left = std::floor(rect.left() / m_gridSize) * m_gridSize;
    const qreal top  = std::floor(rect.top()  / m_gridSize) * m_gridSize;

    QVector<QLineF> lines;

    for (qreal x = left; x < rect.right(); x += m_gridSize) 
    {
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    }
    for (qreal y = top; y < rect.bottom(); y += m_gridSize) 
    {
        lines.append(QLineF(rect.left(), y, rect.right(), y));
    }

    painter->setPen(QPen(QColor(55, 55, 55), 0));
    painter->drawLines(lines);
}