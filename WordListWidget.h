#ifndef WORDLISTWIDGET_H
#define WORDLISTWIDGET_H

#include "WordTable.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class WordListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordListWidget(QWidget *parent = nullptr, QString label = "");
    void addScore(int s);
    void addWord(QString word);
    void reset();
    void update();
signals:

public slots:

private:
    QString label;
    QList<QString> words;
    int score;
    WordTable *wordTable;
    QLabel *nameLabel;
    QLabel *scoreLabel;
    QVBoxLayout *layout;
    QHBoxLayout *headLayout;
};

#endif // WORDLISTWIDGET_H
