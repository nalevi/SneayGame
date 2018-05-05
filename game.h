#ifndef GAME_H
#define GAME_H

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QHBoxLayout>
#include "sneakygame.h"



class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();

private slots:
    void startGame();
    void playerPosition(Coords player);
    void wallPlaces(QVector<Coords> walls);
    void guardPlaces(QVector<Coords> guards);
    void gameOver(bool);

private:
    QVector<QVector<QPushButton*> > _boardMX;
    SneakyGame* _manager;
    QGridLayout* _boardLayout;
    QPushButton* _pause;
    QPushButton* _up;
    QPushButton* _right;
    QPushButton* _down;
    QPushButton* _left;
    QPushButton* _smallBoard;
    QPushButton* _midBoard;
    QPushButton* _bigBoard;
};

#endif // GAME_H
