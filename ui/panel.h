#pragma once
#include <QWidget>

class Panel: public QWidget
{
    Q_OBJECT


    public:
        explicit Panel(QWidget *parent = nullptr);
};