#include "ball.h"
#include "board.h"
#include "pixServer.h"

int bounce[8][8]={
    { NE, NW, SE, SW, N, E, S, W },
    { SE, SW, NE, NW, S, E, N, W },
    { NW, NE, SW, SE, N, W, S, E },
    { SW, SE, NW, NE, S, W, N, E },
    { NE, NW, SE, SW, N, E, S, W },
    { SE, SW, NE, NW, S, E, N, W },
    { NW, NE, SW, SE, N, W, S, E },
    { SW, SE, NW, NE, S, W, N, E }
};

Ball::Ball(Board *b, PixServer *p, int i)
{
    board = b;
    pixServer = p;
    hold = index = i;
    next = SE;
}

void Ball::zero()
{
    board->set(index, empty);
    pixServer->erase(index);
}

void Ball::nextMove()
{
    hold = index;
    board->set(hold, empty);
    int nextSq = index;

    for ( int x = 0; x < 8 ; x++) {
	nextSq = board->getNext(bounce[next][x], index);

	if (board->isHead(nextSq) || board->isEmpty(nextSq)) {
	    next = bounce[next][x];
	    index = nextSq;
	    board->set(index, Balle);
	    break;
	}
    }
}

void Ball::repaint()
{
    static int i = 0;
    static bool rotate = TRUE;

    pixServer->erase(hold);
    pixServer->draw(index, BallPix, i);

    if (rotate) {
	i++; rotate = FALSE;
	if (i>3)
	    i=0;
    } else
	rotate = TRUE;
}
