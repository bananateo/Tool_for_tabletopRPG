#pragma once
#include <QGraphicsView>

class GridView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GridView(QGraphicsScene *scene, QWidget *parent = nullptr);

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    void applyZoom(qreal factor);

    qreal m_zoom = 1.0;
    static constexpr qreal kMinZoom = 0.1;
    static constexpr qreal kMaxZoom = 5.0;
    static constexpr qreal kStep    = 1.15;
};