#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include "lexicon.h"

#include <QMainWindow>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoggleWindow(QWidget *parent = nullptr);
    ~BoggleWindow();
private:
    WordListWidget *me;
    WordListWidget *computer;
    Board *board;
    Console *console;

    Lexicon *lex;
    Lexicon *lexMe;
    Lexicon *lexComputer;

    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;
    static const int SIZE = 5;

    inline int index(int i, int j) { return i * SIZE + j; }
    struct Point{int x; int y;Point(int i=0, int j=0 ):x(i),y(j){}};

    QVector<Point> wordLocation;
    QString wordClicked;
    bool *visited;

    bool GameStarted = false;
    bool GameOver = false;

public slots:
    void dealWithInput(QString lastline);
    void humanTurn(QString word);
    bool checkVaild(QString word);
    bool checkVaildHelper(QString word,int index,int x, int y, bool *visited);
    void computerTurn();
    void searchHelper(QString word,int x, int y, bool *visited);
    void resetVisited();
    void receiveLocation(int* location);
};

#endif // BOGGLEWINDOW_H
