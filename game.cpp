#include "game.h"
#include <QMessageBox>

Game::Game(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800,800);
     setWindowTitle(trUtf8("Lopakodó"));

     _manager = new SneakyGame();
     connect(_manager, SIGNAL(playerMoved(Coords,int)), this, SLOT(playerPosition(Coords)));
     connect(_manager, SIGNAL(guardMoved(QVector<Coords>)), this, SLOT(guardPlaces(QVector<Coords>)));
     connect(_manager, SIGNAL(wall(QVector<Coords>)), this, SLOT(wallPlaces(QVector<Coords>)));
     connect(_manager, SIGNAL(gameOver(bool)), this, SLOT(gameOver(bool)));

     _pause = new QPushButton(trUtf8("Pause/Resume"));
     _pause->setEnabled(false);
     connect(_pause, SIGNAL(clicked()), _manager, SLOT(pauseGame()));

     _smallBoard = new QPushButton(trUtf8("7x7 board"));
     connect(_smallBoard, SIGNAL(clicked()), this, SLOT(startGame()));
     _midBoard = new QPushButton(trUtf8("11x11 board"));
     connect(_midBoard, SIGNAL(clicked()), this, SLOT(startGame()));
     _bigBoard = new QPushButton(trUtf8("15x15 board"));
     connect(_bigBoard, SIGNAL(clicked()), this, SLOT(startGame()));

     QHBoxLayout *hLayout = new QHBoxLayout();
     hLayout->addWidget(_smallBoard);
     hLayout->addWidget(_midBoard);
     hLayout->addWidget(_bigBoard);
     hLayout->addWidget(_pause);

     QVBoxLayout *vLayout = new QVBoxLayout();
     _boardLayout = new QGridLayout();
     vLayout->addLayout(hLayout);
     vLayout->addLayout(_boardLayout);

     QHBoxLayout *controls = new QHBoxLayout();
     _up = new QPushButton("^");
     connect(_up, SIGNAL(clicked()), _manager, SLOT(upButton()));
     _right = new QPushButton("->");
     connect(_right, SIGNAL(clicked()), _manager, SLOT(rightButton()));
     _down = new QPushButton("v");
     connect(_down, SIGNAL(clicked()), _manager, SLOT(downButton()));
     _left = new QPushButton("<-");
     connect(_left, SIGNAL(clicked()), _manager, SLOT(leftButton()));
     controls->addWidget(_up);
     controls->addWidget(_right);
     controls->addWidget(_down);
     controls->addWidget(_left);

     vLayout->addLayout(controls);
     setLayout(vLayout);
}

Game::~Game()
{
    delete _manager;
}

void Game::startGame()
{

    for (int i=0;i<_boardMX.size();++i)
    {
        foreach(QPushButton* button, _boardMX[i])
        {
           _boardLayout->removeWidget(button);
           delete button;
        }
    }

    if (QObject::sender() == _smallBoard)
    {
        _boardMX.resize(7);
    }
    else if (QObject::sender() == _midBoard)
    {
        _boardMX.resize(11);
    }
    else
    {
        _boardMX.resize(15);
    }


    for (int i=0;i<_boardMX.size();i++)
        {
           _boardMX[i].resize(_boardMX.size());
            for (int j=0;j<_boardMX.size();j++)
                {
                    _boardMX[i][j]= new QPushButton(0,this);
                    _boardMX[i][j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
                    _boardMX[i][j]->setFixedSize(350 / _boardMX.size(), 350 / _boardMX.size());
                    _boardMX[i][j]->setIcon(QPixmap(":/image/empty.png"));
                    _boardMX[i][j]->setIconSize(QSize(350 / _boardMX.size(),350 /_boardMX.size()));
                    _boardLayout->addWidget(_boardMX[i][j], i,j);
                }
        }

    _manager->newGame(_boardMX.size());
    _pause->setEnabled(true);
}

void Game::playerPosition(Coords player)
{
    _boardMX[player.x][player.y]->setIcon(QPixmap(":/images/player.png"));
}

void Game::wallPlaces(QVector<Coords> walls)
{
    for (int i=0;i<walls.size();i++)
        {
            _boardMX[walls[i].x][walls[i].y]->setIcon(QPixmap(":/images/wall.png"));
        }
}

void Game::guardPlaces(QVector<Coords> guards)
{
    for (int i=0;i<guards.size();i++)
        {
            _boardMX[guards[i].x][guards[i].y]->setIcon(QPixmap(":/images/guard.png"));
        }
}

void Game::gameOver(bool won)
{
    _pause->setEnabled(false);
    if(won)
        QMessageBox::information(this, trUtf8("Game Over"), trUtf8("You won! "));
    else
        QMessageBox::information(this, trUtf8("Game Over"), trUtf8("You lost! "));
}
