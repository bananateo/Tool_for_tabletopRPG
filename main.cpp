#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QDockWidget>

#include "grid/gridscene.h"
#include "ui/panel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GridScene scene;

    QMainWindow window;

    auto *view = new QGraphicsView(&scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    window.setCentralWidget(view);

    auto *panel = new Panel;
    auto *dock = new QDockWidget("Tools", &window);
    dock->setWidget(panel);
    window.addDockWidget(Qt::LeftDockWidgetArea, dock);

    window.resize(1100, 600);
    window.show();

    return app.exec();
}