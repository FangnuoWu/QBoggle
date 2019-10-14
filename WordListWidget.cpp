#include "WordListWidget.h"
#include "WordTable.h"
#include <QTableView>

WordListWidget::WordListWidget(QWidget *parent, QString label) : QWidget(parent)
{
    reset();

    layout = new QVBoxLayout();
    headLayout = new QHBoxLayout();

    nameLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPointSize(20);
    nameLabel->setFont(font);
    scoreLabel->setFont(font);
    nameLabel->setText(label);
    scoreLabel->setText(QString::number(score));

    headLayout->addWidget(nameLabel, 0, Qt::AlignmentFlag::AlignLeft);
    headLayout->addWidget(scoreLabel, 0, Qt::AlignmentFlag::AlignRight);
    layout->addLayout(headLayout);

    QFrame *hline = new QFrame();
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hline);

    wordTable = new WordTable();
    layout->addWidget(wordTable);

    setLayout(layout);
}

void WordListWidget::addScore(int score)
{
    this->score += score;
    scoreLabel->setText(QString::number(this->score));
}
void WordListWidget::addWord(QString word)
{
    this->words.append(word);
    this->wordTable->addWord(word);
}
void WordListWidget::reset()
{
    this->score = 0;
    this->words.clear();
}

void WordListWidget::update()
{
    this->score = 0;
    this->words.clear();
    scoreLabel->setText(QString::number(this->score));
    this->wordTable->reset();
    layout->removeWidget(wordTable);
    wordTable = new WordTable;
    layout->addWidget(wordTable);
    setLayout(layout);
}

