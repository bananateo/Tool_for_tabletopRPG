#pragma once

#include <QGraphicsScene>

class BackgroundItem;

class GridScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GridScene(QObject *parent = nullptr);

    int gridSize() const { return m_gridSize; }
    void setGridSize(int size);

public slots:
    // Зарежда картина от файл и я слага ЗАД grid-а. Заменя предишната.
    bool setBackgroundImage(const QString &filePath);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    int m_gridSize = 50;
    BackgroundItem *m_backgroundItem = nullptr;
};