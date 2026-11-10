#pragma once

#include <QGraphicsItem>
#include <QPixmap>

class BackgroundItem : public QGraphicsItem
{
public:
    explicit BackgroundItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    enum class Mode {None, Move, Resize, Rotate};

    qreal pixelsPerUnit() const;             
    QPointF sceneCenter() const;
    Mode hitTest(const QPointF &localPos) const;
    QRectF handleRect(const QPointF &center) const;
    QPointF rotateHandlePos() const;

    QPixmap m_pixmap;
    Mode m_mode = Mode::None;

    qreal m_startScale = 1.0;
    qreal m_startRotation = 0.0;
    qreal m_startDistance = 1.0;
    qreal m_startAngle = 0.0;

    static constexpr qreal kHandleSize = 12.0;
    static constexpr qreal kRotateOffset = 30.0;
};