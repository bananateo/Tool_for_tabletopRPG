#pragma once
#include <QWidget>

class Panel: public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);

signals:
    // Излъчва се, след като потребителят избере файл с картина.
    void backgroundImageSelected(const QString &filePath);

private slots:
    void onAddBackgroundClicked();
};