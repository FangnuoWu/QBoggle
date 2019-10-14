#include "Cube.h"


#include <QHBoxLayout>

Cube::Cube(QWidget *parent) : QWidget(parent)
{
    location = new int[2];
    label = new QLabel();
    label->setText("");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(20);
    label->setFont(font);

    this->setFixedSize(75, 75);
    this->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    setLayout(layout);
}

void Cube::setLetter(QString l)
{
    label->setText(l);
}

void Cube::highlight()
{
    label->setStyleSheet("background-color: yellow");
}

void Cube::cancelHighlight()
{
    label->setStyleSheet("background-color: white");
}

void Cube::mousePressEvent(QMouseEvent *)
{
    label->setStyleSheet("background-color: yellow");
    emit sendLocation(location);
}

