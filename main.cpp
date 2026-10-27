#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>

#include "grid/gridscene.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GridScene scene;
    

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setDragMode(QGraphicsView::ScrollHandDrag); // влачене за местене
    view.resize(900, 600);
    view.show();

    return app.exec();
}