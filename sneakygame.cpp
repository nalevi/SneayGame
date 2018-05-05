#include "sneakygame.h"

#include <QTime>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <random>
#include <time.h>
#include <algorithm>
#include <math.h>

SneakyGame::SneakyGame()
{
    _timer = new QTimer();
    _timer->setInterval(750);

    qsrand(QTime::currentTime().msec());

    connect(_timer, SIGNAL(timeout()), this, SLOT(timer()));

}

SneakyGame::~SneakyGame()
{
    _timer->stop();
    delete _timer;
}

void SneakyGame::newGame(int size)
{
    _player.dir = 0;
    _boardSize = size;
    Coords tmp;
    _won = false;


    if(size==7)
    {
        QFile file(":/map/7x7.txt");
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
        }
        QTextStream stream(&file);
        _walls.resize(6);
        _guards.resize(2);
        int i = 0;

        while(i < _guards.size() * 2 + 4)
        {
            //player position
            if(i == 0)
            {
               tmp.x = stream.readLine().toInt();
               ++i;
             }
            else if(i == 1)
            {
                tmp.y = stream.readLine().toInt();
                _player = tmp;
                ++i;
            }
            //guard positions
            else if(i < _guards.size() * 2 + 2)
            {
                if (i % 2 == 0)
                {
                    tmp.x = stream.readLine().toInt();
                 }
                else
                {
                    tmp.y = stream.readLine().toInt();
                    _guards[(i-1)/2] = tmp;
                }
                    ++i;
            }
            //exit position
            if(i == _guards.size() * 2 + 2)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else if(i == _guards.size() * 2 + 3)
            {
                tmp.y = stream.readLine().toInt();
                _exit = tmp;
                ++i;
            }
        }
        //wall positions
        while(i < (_walls.size() * 2 + _guards.size() * 2 + 4) )
        {
            if(i%2 == 0)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else
            {
                tmp.y = stream.readLine().toInt();
                _walls[(i-1)/2] = tmp;
                ++i;
            }
        }
        file.close();
    }

    else if(size == 11)
    {
        QFile file(":/map/11x11.txt");
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
        }

        QTextStream stream(&file);
        _walls.resize(20);
        _guards.resize(4);
        int i = 0;

        while(i < _guards.size() * 2 + 4)
        {
            //player position
            if(i == 0)
            {
               tmp.x = stream.readLine().toInt();
               ++i;
             }
            else if(i == 1)
            {
                tmp.y = stream.readLine().toInt();
                _player = tmp;
                ++i;
            }
            //guard positions
            else if(i < _guards.size() * 2 + 2)
            {
                if (i % 2 == 0)
                {
                    tmp.x = stream.readLine().toInt();
                 }
                else
                {
                    tmp.y = stream.readLine().toInt();
                    _guards[(i-1)/2] = tmp;
                }
                    ++i;
            }
            //exit position
            if(i == _guards.size() * 2 + 2)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else if(i == _guards.size() * 2 + 3)
            {
                tmp.y = stream.readLine().toInt();
                _exit = tmp;
                ++i;
            }
        }
        //wall positions
        while(i < (_walls.size() * 2 + _guards.size() * 2 + 4) )
        {
            if(i%2 == 0)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else
            {
                tmp.y = stream.readLine().toInt();
                _walls[(i-1)/2] = tmp;
                ++i;
            }
        }
      file.close();
    }

    else
    {
        QFile file(":/map/15x15.txt");
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
        }
        QTextStream stream(&file);
        _walls.resize(44);
        _guards.resize(6);
        int i = 0;

        while(i < _guards.size() * 2 + 4)
        {
            //player position
            if(i == 0)
            {
               tmp.x = stream.readLine().toInt();
               ++i;
             }
            else if(i == 1)
            {
                tmp.y = stream.readLine().toInt();
                _player = tmp;
                ++i;
            }
            //guard positions
            else if(i < _guards.size() * 2 + 2)
            {
                if (i % 2 == 0)
                {
                    tmp.x = stream.readLine().toInt();
                 }
                else
                {
                    tmp.y = stream.readLine().toInt();
                    _guards[(i-1)/2] = tmp;
                }
                    ++i;
            }
            //exit position
            if(i == _guards.size() * 2 + 2)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else if(i == _guards.size() * 2 + 3)
            {
                tmp.y = stream.readLine().toInt();
                _exit = tmp;
                ++i;
            }
        }
        //wall positions
        while(i < (_walls.size() * 2 + _guards.size() * 2 + 4) )
        {
            if(i%2 == 0)
            {
                tmp.x = stream.readLine().toInt();
                ++i;
            }
            else
            {
                tmp.y = stream.readLine().toInt();
                _walls[(i-1)/2] = tmp;
                ++i;
            }
        }
        file.close();
    }

    srand(time(NULL));
    //playerMoved(_player, _direction);
    for(int num = 0; num < _guards.size(); ++num)
    {
        _guards[num].dir = rand()%4;
    }

    guardMoved(_guards);
    wall(_walls);
    _timer->start();

}

void SneakyGame::pauseGame()
{
    if(_timer->isActive())
        _timer->stop();
    else
        _timer->start();
}

void SneakyGame::guardTurn(Coords &guard, int direction)
{
    int i = 0;
    bool found = false;

    while(i < _walls.size() && !found)
    {
        found = ( guard.x == _walls[i].x && guard.y == _walls[i].y);
        ++i;
     }

    if(found && direction == 0)
    {
        guard.y--;
        srand(time(NULL));
        guard.dir = rand()%3 + 1;
    }
    else if(found && direction == 1)
    {
        guard.x--;
        srand(time(NULL));
        guard.dir = rand()%3 + 2;
    }
    else if(found && direction == 2)
    {
        guard.y++;
        srand(time(NULL));
        guard.dir = rand()%2;
    }
    else if(found && direction == 3)
    {
        guard.x++;
        srand(time(NULL));
        guard.dir = rand()%3 ;
    }

}

void SneakyGame::timer()
{
    if(gameCheck())
    {
        _timer->stop();
        gameOver(_won);
        return;
    }

    for(int i = 0; i < _guards.size(); ++i)
    {
       switch(_guards[i].dir)
       {
        case 0: _guards[i].y++;
                    guardTurn(_guards[i],0);
           break;
       case 1: _guards[i].x++;
                    guardTurn(_guards[i],1);
            break;
       case 2: _guards[i].y--;
                    guardTurn(_guards[i],2);
            break;
       case 3: _guards[i].x--;
                    guardTurn(_guards[i],3);
            break;
       }
    }

    guardMoved(_guards);

}

void SneakyGame::controlButton(int x)
{
    if(!_timer->isActive())
        return;

    switch(x)
    {
    case 1: _player.dir = (_player.dir + 3) % 4;
        break;
    case 2: _player.dir = (_player.dir + 4) % 4;
        break;
    case 3: _player.dir = (_player.dir + 1) % 4;
        break;
    case 4: _player.dir = (_player.dir + 2) % 4;
        break;
    }

    playerMoved(_player, _player.dir);
}

bool SneakyGame::gameCheck()
{
    switch(_player.dir)
        {
        case 0 : if(_player.y+1 >= _boardSize) return true; break;
        case 1 : if(_player.x+1 >= _boardSize) return true; break;
        case 2 : if(_player.y-1 < 0) return true; break;
        case 3 : if(_player.x-1 < 0) return true; break;
        }

    for (int i=0;i<_walls.size();++i)
    {
        switch(_player.dir)
        {
        case 0 : if(_player.y+1 == _walls[i].y && _player.x==_walls[i].x) return true; break;
        case 1 : if(_player.x+1 == _walls[i].x && _player.y==_walls[i].y) return true; break;
        case 2 : if(_player.y-1 == _walls[i].y && _player.x==_walls[i].x) return true; break;
        case 3 : if(_player.x-1 == _walls[i].x && _player.y==_walls[i].y) return true; break;
        }
    }

    for(int i = 0; i < _guards.size();++i)
    {
        for(int j = 0; j < _walls.size(); ++j)
        {
            switch (_player.dir)
            {
            case 0: if(std::sqrt(std::pow(_player.y+1 - _guards[i].y, 2.0) + std::pow(_player.x - _guards[i].x, 2.0)) < 5 )
                break;
            }
        }
    }

    switch(_player.dir)
        {
        case 0 : if(_player.y+1 == _exit.y && _player.x == _exit.x) _won = true; return true; break;
        case 1 : if(_player.x+1 == _exit.x && _player.y == _exit.y) _won = true; return true; break;
        case 2 : if(_player.y-1 == _exit.y && _player.x == _exit.x) _won = true; return true; break;
        case 3 : if(_player.x-1 == _exit.x && _player.y == _exit.y) _won = true; return true; break;
        }
}
