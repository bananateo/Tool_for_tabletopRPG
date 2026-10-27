#pragma once

#include <QGraphicsScene>

class GridScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GridScene(QObject *parent = nullptr);

    int gridSize() const { return m_gridSize; }
    void setGridSize(int size);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    int m_gridSize = 50;   // разстояние между линиите (в единици на сцената)
};