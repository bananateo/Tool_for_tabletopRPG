#include "gridscene.h"
#include "backgrounditem.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QVector>
#include <QLineF>
#include <cmath>

namespace 
{
    constexpr qreal kBackgroundZ = -1000;
    constexpr qreal kGridLinesZ  = -500;
}

class GridLinesItem : public QGraphicsItem
{
public:
    explicit GridLinesItem(GridScene *scene) : m_scene(scene)
    {
        setZValue(kGridLinesZ);
        setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
        setAcceptedMouseButtons(Qt::NoButton);
    }

    QRectF boundingRect() const override { return m_scene->sceneRect(); }

    QPainterPath shape() const override { return QPainterPath(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        const QRectF rect = option->exposedRect;
        const int gs = m_scene->gridSize();

        const qreal left = std::floor(rect.left() / gs) * gs;
        const qreal top  = std::floor(rect.top()  / gs) * gs;

        QVector<QLineF> lines;
        for (qreal x = left; x < rect.right(); x += gs)
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (qreal y = top; y < rect.bottom(); y += gs)
            lines.append(QLineF(rect.left(), y, rect.right(), y));

        painter->setPen(QPen(QColor(55, 55, 55), 0));
        painter->drawLines(lines);
    }

private:
    GridScene *m_scene;
};

GridScene::GridScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(-100000, -100000, 200000, 200000);
    addItem(new GridLinesItem(this));
}

void GridScene::setGridSize(int size)
{
    if (size < 1 || size == m_gridSize)
        return;
    m_gridSize = size;
    update();
}

bool GridScene::setBackgroundImage(const QString &filePath)
{
    QPixmap pixmap(filePath);
    if (pixmap.isNull())
        return false;

    if (m_backgroundItem) 
    {
        removeItem(m_backgroundItem);
        delete m_backgroundItem;
        m_backgroundItem = nullptr;
    }

    m_backgroundItem = new BackgroundItem(pixmap);
    m_backgroundItem->setZValue(kBackgroundZ);

    const qreal maxSide = 1500;
    const qreal biggest = qMax(pixmap.width(), pixmap.height());
    if (biggest > maxSide)
        m_backgroundItem->setScale(maxSide / biggest);

    // Центрираме картината в (0, 0) - това е центърът на изгледа по подразбиране.
    m_backgroundItem->setPos(-pixmap.width() / 2.0, -pixmap.height() / 2.0);

    addItem(m_backgroundItem);
    clearSelection();
    m_backgroundItem->setSelected(true);
    return true;
}

void GridScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QColor(30, 30, 30));
}