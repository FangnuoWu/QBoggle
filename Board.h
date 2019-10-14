#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"

#include <QWidget>

extern QString *letterChosen;

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    void reset();
    void changeBoard(QString InputLetters);
    void highlightCube(int x, int y);
    void cancelHighlightCube(int x, int y);
signals:

public:
    Cube **cubes;
private:
    int size;    
    QString *letters;
    int letters_index;
    int vec_index;
    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
};

#endif // BOARD_H
