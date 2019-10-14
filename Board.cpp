#include "Board.h"
#include "Cube.h"

#include <QGridLayout>
#include <QTime>

const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

    this->size = size;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];
    letterChosen = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];

    shake();

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            cubes[index(i, j)]->changeLocation(i,j);
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);
        }
    }
    setLayout(layout);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            letters_index = rand()%size;
            QString singleLetter = QString(this->letters[index(i, j)].at(letters_index));
            if(singleLetter =="Q"){
                singleLetter = "Qu";
            }
            letterChosen[index(i, j)] = singleLetter.toLower();
            this->cubes[index(i, j)]->setLetter(singleLetter);
        }
    }
}

Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
}

void Board::changeBoard(QString InputLetters)
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            letterChosen[index(i, j)] = InputLetters[index(i,j)].toLower();
            this->cubes[index(i, j)]->setLetter(InputLetters[index(i,j)].toUpper());
        }
    }
}

void Board::shake()
{
    QVector<QString> vec;
    for (int i = 0; i < letters->size(); i++)
        vec.push_back(letters[i]);

    for (int i = 0; i < vec.size(); i++) {
        vec_index = rand()%size;
        swap(vec[i],vec[vec_index]);
        letters[i] = vec.at(i);
    }
}

void Board::reset()
{
    shake();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            letters_index = rand()%size;
            QString singleLetter = QString(this->letters[index(i, j)].at(letters_index));
            if(singleLetter =="Q"){
                singleLetter = "Qu";
            }
            letterChosen[index(i, j)] = singleLetter.toLower();
            this->cubes[index(i, j)]->setLetter(singleLetter);
        }
    }
}

void Board::highlightCube(int x, int y)
{
    this->cubes[index(x, y)]->highlight();
}

void Board::cancelHighlightCube(int x, int y)
{
    this->cubes[index(x, y)]->cancelHighlight();
}
