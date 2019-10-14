#include "BoggleWindow.h"

#include <QFile>
#include <QString>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

QString *letterChosen;
static int direction[8][2]={{0,1},{0,-1},{-1,0},{1,0},{1,1},{1,-1},{-1,1},{-1,-1}};

BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320, 740, 260);

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    lex = new Lexicon(qFile);
    lexMe = new Lexicon;
    lexComputer = new Lexicon;

    visited = new bool[SIZE*SIZE];

    console->write("Welcome to the game Boggle!");
    console->write("Do you want to change the Board? Please enter Y/N.\n");

    this->connect(console,SIGNAL(newLineWritten(QString)),this,SLOT(dealWithInput(QString)));

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            connect(board->cubes[index(i, j)],SIGNAL(sendLocation(int*)),this,SLOT(receiveLocation(int*)));
        }
    }
}

BoggleWindow::~BoggleWindow()
{
}

void BoggleWindow::dealWithInput(QString lastLine)
{
    //游戏已结束，用户输入Q或R
    if(GameOver){
        if(lastLine=="R"){
            GameOver = false;
            GameStarted = false;
            me->update();
            computer->update();
            board->reset();
            lexMe = new Lexicon;
            lexComputer = new Lexicon;
            console->write("Welcome to the game Boggle again!");
            console->write("Do you want to change the Board? Enter yes(Y) or no(N).");
            return;
        }
        else if(lastLine=="Q"){
            this->close();
            return;
        }else{
            console->write("Invalid input! Please enter R/Q.");
            return;
        }
    }
    //游戏已开始，处理人和电脑两局
    if(GameStarted){
        if (lastLine==""){
            console->write("Computer finished! Enter R to restart the game, Q to quit the game.");
            GameOver = true;
            computerTurn();
            return;
        }
        else {
            humanTurn(lastLine);
            return;
        }
    }
    //游戏还没开始，处理用户更改cube
    if(!GameStarted){
        if (lastLine == "Y"){
            console->write("Please enters a string of characters, which represent the cubes from left to right, top to bottom.");
            return;
        }
        if (lastLine == "N"){
            GameStarted = true;
            console->write("Now it's human turn. Enter words or click the cube. \n Enters a blank line and computer will start it's work.");
            return;
        }
        else {
            QString vaildLetters;
            for (int i = 0; i < lastLine.size(); ++i) {
                if ((lastLine[i]>="A" && lastLine[i]<="Z")||(lastLine[i]>="a" && lastLine[i]<="z"))
                    vaildLetters.append(lastLine[i]);
            }
            if(vaildLetters.size()<25) console->write("The string is too short. Please enter again.");
            else {
                GameStarted = true;
                board->changeBoard(vaildLetters);
                console->write("Now it's human turn. Please enter words next line. Enters a blank line and computer will start it's work.");
                return;
            }
        }
    }
}

void BoggleWindow::receiveLocation(int* location)
{
    if (wordClicked=="") {
        GameStarted = true;
        wordLocation.clear();
    }
    wordLocation.push_back(Point(location[0],location[1]));
    wordClicked.append(letterChosen[index(location[0],location[1])]);
    int sizeOfWord = wordLocation.size();

    if (!lex->containsPrefix(wordClicked.toStdString())) {
        console->write("This word does not exsit.\n");
        wordClicked="";
        for (int i = 0; i < sizeOfWord; ++i)
            board->cancelHighlightCube(wordLocation[i].x,wordLocation[i].y);
        return;
    }

    if(sizeOfWord>1){
        bool isNeighbour = false;
        for (int i = 0; i < 8; ++i) {
            if (wordLocation[sizeOfWord-2].x + direction[i][0]==wordLocation[sizeOfWord-1].x && wordLocation[sizeOfWord-2].y + direction[i][1]==wordLocation[sizeOfWord-1].y)
                isNeighbour = true;
        }
        if (isNeighbour==false) {
            console->write("It's not vaild.");
            wordClicked="";
            for (int i = 0; i < sizeOfWord; ++i)
                board->cancelHighlightCube(wordLocation[i].x,wordLocation[i].y);
            return;
        }
    }

    if(!lexMe->contains(wordClicked.toStdString()) && lex->contains(wordClicked.toStdString()) && sizeOfWord > 3){
        lexMe->add(wordClicked.toStdString());
        me->addWord(wordClicked);
        me->addScore(wordClicked.size()-3);
        wordClicked="";
        for (int i = 0; i < wordLocation.size(); ++i)
            board->cancelHighlightCube(wordLocation[i].x,wordLocation[i].y);
        return;
    }

}

void BoggleWindow::humanTurn(QString word)
{
    for (int var = 0; var < wordLocation.size(); ++var) {
        board->cancelHighlightCube(wordLocation.at(var).x, wordLocation.at(var).y);
    }
    wordLocation.clear();

    if (word.size()<4) {
        console->write("It's not a vaild word. Please enter a word at least four letters long.");
        return;
    }
    if (lexMe->contains(word.toStdString())) {
        console->write("You have already find this word. Please enter again.");
        return;
    }
    if (!lex->contains(word.toStdString())) {
        console->write("This word does not exsit. Please enter again.");
        return;
    }

    if (checkVaild(word)){
        for (int var = 0; var < wordLocation.size(); ++var) {
            board->highlightCube(wordLocation.at(var).x, wordLocation.at(var).y);
        }
        lexMe->add(word.toStdString());
        me->addWord(word);
        me->addScore(word.size()-3);
    }else
        console->write("This word can not be formed on the board. Please enter again.");
}

bool BoggleWindow::checkVaild(QString word)
{
    resetVisited();
    for ( int x = 0; x < SIZE; ++x)
        for (int y =0; y < SIZE; ++y){
            resetVisited();
            if (checkVaildHelper(word,0,x,y,visited)) return true;
        }
    return false;
}

bool BoggleWindow::checkVaildHelper(QString word,int indexOfWord, int x, int y, bool* visited)
{
    if(indexOfWord == word.size()) return true;

    if (x<0||x>4||y<0||y>4) return false;

    if (letterChosen[index(x, y)]=="qu"){
        if (QString(word.at(indexOfWord))!="q") return false;
        indexOfWord++;
        if (QString(word.at(indexOfWord))!="u") return false;
    }else{
        if (QString(word.at(indexOfWord))!=letterChosen[index(x, y)]) return false;
    }
    if (visited[index(x, y)]) return false;

    visited[index(x, y)] = true;
    wordLocation.push_back(Point(x,y));

    if(checkVaildHelper(word,indexOfWord+1,x+1,y,visited) || checkVaildHelper(word,indexOfWord+1,x+1,y+1,visited)
            || checkVaildHelper(word,indexOfWord+1,x-1,y,visited) || checkVaildHelper(word,indexOfWord+1,x-1,y+1,visited)
            || checkVaildHelper(word,indexOfWord+1,x-1,y-1,visited) || checkVaildHelper(word,indexOfWord+1,x+1,y-1,visited)
            || checkVaildHelper(word,indexOfWord+1,x,y+1,visited) || checkVaildHelper(word,indexOfWord+1,x,y-1,visited)) {
        return true;
    }

    visited[index(x, y)] = false;
    wordLocation.removeLast();
    return false;
}

void BoggleWindow::computerTurn()
{
    for (int var = 0; var < wordLocation.size(); ++var) {
        board->cancelHighlightCube(wordLocation.at(var).x, wordLocation.at(var).y);
    }

    QString word;
    visited = new bool[SIZE*SIZE];
    for ( int x = 0; x < SIZE; ++x)
        for (int y =0; y < SIZE; ++y){
            resetVisited();
            searchHelper(word,x,y,visited);
        }

}

void BoggleWindow::searchHelper(QString word, int x, int y, bool* visited)
{
    if (x<0||x>4||y<0||y>4||visited[index(x, y)]) return;

    word.append(letterChosen[index(x, y)]);
    if(!lex->containsPrefix(word.toStdString())){
        word.remove(word.size()-1);
        return;
    }
    visited[index(x, y)] = true;
    if(word.size()>3 && lex->contains(word.toStdString())){
         if(!lexMe->contains(word.toStdString()) && !lexComputer->contains(word.toStdString())){
            lexComputer->add(word.toStdString());
            computer->addWord(word);
            computer->addScore(word.size()-3);
         }
    }
    for (int i = 0; i < 8; ++i) {
        searchHelper(word,x+direction[i][0],y+direction[i][1],visited);
    }
    visited[index(x, y)] = false;
}

void BoggleWindow::resetVisited()
{
    for (int i=0; i<SIZE*SIZE; ++i){
        visited[i] = false;
    }
}
