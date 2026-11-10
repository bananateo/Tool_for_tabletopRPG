#include "backgrounditem.h"

#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

BackgroundItem::BackgroundItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_pixmap(pixmap)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setTransformOriginPoint(boundingRect().center());
}

QRectF BackgroundItem::boundingRect() const
{
    const QRectF r(QPointF(0, 0), QSizeF(m_pixmap.size()));
    const qreal ppu = qMax(pixelsPerUnit(), 0.0001);
    const qreal margin = (kRotateOffset + kHandleSize) / ppu;
    return r.adjusted(-margin, -margin, margin, margin);
}

QPainterPath BackgroundItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(QPointF(0, 0), QSizeF(m_pixmap.size())));
    if (isSelected()) 
    {
        const qreal h = kHandleSize / qMax(pixelsPerUnit(), 0.0001);
        const QRectF r(QPointF(0, 0), QSizeF(m_pixmap.size()));
        for (const QPointF &c : {r.topLeft(), r.topRight(), r.bottomLeft(), r.bottomRight()})// Добавяне на дръжки на изображението
            path.addRect(QRectF(c.x() - h, c.y() - h, 2 * h, 2 * h));
        path.addEllipse(rotateHandlePos(), h, h);
    }
    return path;
}

qreal BackgroundItem::pixelsPerUnit() const
{
    if (!scene() || scene()->views().isEmpty())
        return 1.0;
    return scene()->views().first()->transform().m11() * scale();
}

QPointF BackgroundItem::sceneCenter() const
{
    return mapToScene(transformOriginPoint());
}

QPointF BackgroundItem::rotateHandlePos() const
{
    const qreal ppu = qMax(pixelsPerUnit(), 0.0001);
    return QPointF(m_pixmap.width() / 2.0, -kRotateOffset / ppu);
}

QRectF BackgroundItem::handleRect(const QPointF &center) const
{
    const qreal h = kHandleSize / qMax(pixelsPerUnit(), 0.0001);
    return QRectF(center.x() - h / 2, center.y() - h / 2, h, h);
}

BackgroundItem::Mode BackgroundItem::hitTest(const QPointF &p) const
{
    if (isSelected()) 
    {
        const qreal h = kHandleSize / qMax(pixelsPerUnit(), 0.0001);
        const QRectF r(QPointF(0, 0), QSizeF(m_pixmap.size()));

        if (QLineF(p, rotateHandlePos()).length() <= h)
            return Mode::Rotate;

        for (const QPointF &c : {r.topLeft(), r.topRight(), r.bottomLeft(), r.bottomRight()}) {
            if (QRectF(c.x() - h, c.y() - h, 2 * h, 2 * h).contains(p))
                return Mode::Resize;
        }
    }
    return Mode::Move;
}

void BackgroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(QPointF(0, 0), m_pixmap);

    if (!isSelected())
        return;

    const qreal ppu = qMax(pixelsPerUnit(), 0.0001);
    const QRectF r(QPointF(0, 0), QSizeF(m_pixmap.size()));

    QPen pen(QColor(80, 160, 255), 1.5 / ppu);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(r);

    // линия към дръжката за въртене
    const QPointF rot = rotateHandlePos();
    painter->drawLine(QPointF(r.center().x(), 0), rot);

    painter->setBrush(QColor(255, 255, 255));
    for (const QPointF &c : {r.topLeft(), r.topRight(), r.bottomLeft(), r.bottomRight()})
        painter->drawRect(handleRect(c));

    const qreal h = kHandleSize / ppu;
    painter->drawEllipse(rot, h / 2, h / 2);
}

void BackgroundItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    switch (hitTest(event->pos())) 
    {
        case Mode::Rotate: setCursor(Qt::CrossCursor);    break;
        case Mode::Resize: setCursor(Qt::SizeFDiagCursor); break;
        default:           setCursor(Qt::SizeAllCursor);   break;
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void BackgroundItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) 
    {
        event->ignore();
        return;
    }

    m_mode = hitTest(event->pos());

    // Селектираме и ръчно, защото ги местим сами.
    if (!isSelected()) 
    {
        scene()->clearSelection();
        setSelected(true);
    }

    const QPointF c = sceneCenter();
    const QPointF d = event->scenePos() - c;

    m_startScale = scale();
    m_startRotation = rotation();
    m_startDistance = qMax(QLineF(c, event->scenePos()).length(), 1.0);
    m_startAngle = qRadiansToDegrees(std::atan2(d.y(), d.x()));

    event->accept();
}

void BackgroundItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (m_mode) {
    case Mode::Move: {
        const QPointF delta = event->scenePos() - event->lastScenePos();
        setPos(pos() + delta);
        break;
    }
    case Mode::Resize: {
        const qreal dist = QLineF(sceneCenter(), event->scenePos()).length();
        prepareGeometryChange();
        setScale(qBound(0.01, m_startScale * dist / m_startDistance, 100.0));
        break;
    }
    case Mode::Rotate: {
        const QPointF d = event->scenePos() - sceneCenter();
        const qreal angle = qRadiansToDegrees(std::atan2(d.y(), d.x()));
        qreal rot = m_startRotation + (angle - m_startAngle);
        if (event->modifiers() & Qt::ShiftModifier)          // Shift = стъпки по 15°
            rot = std::round(rot / 15.0) * 15.0;
        setRotation(rot);
        break;
    }
    default:
        break;
    }
    event->accept();
}

void BackgroundItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mode = Mode::None;
    event->accept();
}