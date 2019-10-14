#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);
    void highlight();
    void cancelHighlight();   
    void mousePressEvent(QMouseEvent *event);
    void changeLocation(int i,int j){location[0]=i;location[1]=j;}

signals:
    void sendLocation(int *location);

public slots:

private:
    QLabel *label;
    int *location;
};

#endif // CUBE_H
