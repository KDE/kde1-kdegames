#include <qwidget.h>
#include <qpixmap.h>
#include <qlist.h>

#include <stdlib.h>

#include "basket.h"
#include "board.h"
#include "pixServer.h"
#include "qtimer.h"

Kaffee::Kaffee(int pos)
{
  p = pos;
  t = Red;
  int r = rand() % 40000;
  QTimer::singleShot( r, this, SLOT(golden()) );
}

void Kaffee::golden()
{
  t = (t == Red ? Golden : Red);
  int r = rand() % 40000;
  QTimer::singleShot( r, this, SLOT(golden()) );
}

Basket::Basket(Board *b, PixServer *p)
{
  board = b;
  pixServer = p;
  list = new QList<Kaffee>;
  list->setAutoDelete( TRUE );
}

void Basket::clear()
{
  if( !list->isEmpty())
    list->clear();
}

void Basket::newApples()
{
  int x;
  int i = 0;

  while(i < 10) {
    x =  rand() % board->size();
    if ((unsigned)x < board->size() && board->isEmpty(x) && x > BoardWidth+4) {
      Kaffee *g = new Kaffee(x);
      board->set(x, Apple);
      list->append(g);
      i++;
    }
  }
}

void Basket::repaint( )
{
    Kaffee *g;
    for ( g  = list->first(); g != 0; g = list->next())
	pixServer->draw(g->position(), ApplePix, (int)g->type());
}

Fruits Basket::eaten(int i)
{
  Kaffee *g;
  Fruits f = Bummer;

  if (!board->isApple(i))
    return f;

  for (g = list->first(); g != 0; g = list->next() )
    {
    if (g->position() == i) {
      f = g->type();
      list->remove(g);
      break;
    }
  }
  if (list->isEmpty())
    emit openGate();

  return f;
}
