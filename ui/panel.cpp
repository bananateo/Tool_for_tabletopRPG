#include "panel.h"

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(200);

    auto *layout = new QVBoxLayout(this);

    auto *addBackgroundButton = new QPushButton(tr("Add background"), this);
    connect(addBackgroundButton, &QPushButton::clicked, this, &Panel::onAddBackgroundClicked);
    layout->addWidget(addBackgroundButton);

    layout->addStretch();
}

void Panel::onAddBackgroundClicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Choose background image"),
        QString());

    if (!path.isEmpty())
        emit backgroundImageSelected(path);
}