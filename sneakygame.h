#ifndef SNEAKYGAME_H
#define SNEAKYGAME_H

#include <QVector>
#include <QTimer>
#include <QIcon>
#include <QPixmap>

struct Coords
{
    int x;
    int y;
    int dir;
};



class SneakyGame : public QObject
{
   Q_OBJECT

public:
    SneakyGame();
    ~SneakyGame();

public slots:
    void newGame(int size);
    void pauseGame();

private slots:
    void timer();
    void controlButton(int x);
    void leftButton() {controlButton(1);}
    void upButton() {controlButton(2);}
    void rightButton() {controlButton(3);}
    void downButton() {controlButton(4);}
    bool gameCheck();

signals:
    void playerMoved(Coords player, int direction);
    void guardMoved(QVector<Coords>);
    void wall(QVector<Coords> walls);
    void gameOver(bool);

private:
    void guardTurn(Coords& guard, int direction);

    QTimer* _timer;
    QVector<Coords> _walls;
    QVector<Coords> _guards;
    Coords _player;
    Coords _exit;
    int _boardSize;
    bool _won;
};

#endif // SNEAKYGAME_H
