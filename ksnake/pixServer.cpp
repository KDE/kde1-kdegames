#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qrect.h>
#include <qstring.h>

#include <kapp.h>
#include <qbitmap.h>

#include "pixServer.h"
#include "board.h"

PixServer::PixServer( Board *b, QWidget *parent)
{
    pixDir.setStr(KApplication::kdedir());
    pixDir.append("/share/apps/ksnake/pics/");

    board = b;
    initPixmaps();
    initRoomPixmap();
    w = parent;
}

void PixServer::erase(int pos)
{
    QRect rect = board->rect(pos);
    bitBlt( w, rect.x(), rect.y(), &backPix,
	    rect.x(), rect.y(), rect.width(), rect.height());
}

void PixServer::restore(int pos)
{
    QRect rect = board->rect(pos);
    bitBlt( w, rect.x(), rect.y(), &roomPix,
	    rect.x(), rect.y(), rect.width(), rect.height());
}

void PixServer::draw(int pos, PixMap pix, int i = 0)
{
    QPixmap p;
    p.resize(16, 16);

    QRect rect = board->rect(pos);

    bitBlt( &p, 0, 0, &backPix,
	    rect.x(), rect.y(), rect.width(), rect.height());

    switch (pix) {
    case SamyPix:        bitBlt(&p ,0,0, &samyPix[i]);
	break;
    case CompuSnakePix:  bitBlt(&p ,0,0, &compuSnakePix[i]);
	break;
    case ApplePix:       bitBlt(&p ,0,0, &applePix[i]);
	break;
    case BallPix:        bitBlt(&p ,0,0, &ballPix[i]);
	break;
    default:
	break;
    }

    bitBlt(w, rect.x(), rect.y(), &p);
}

void PixServer::initPixmaps()
{

    QPixmap PIXMAP;

    PIXMAP.load((const char *)(pixDir + "snake1.xpm"));
    for (int x = 0 ; x < 14; x++){
	compuSnakePix[x].resize(16, 16);
	bitBlt(&compuSnakePix[x] ,0,0, &PIXMAP,x*16, 0, 16, 16, CopyROP, TRUE);
	compuSnakePix[x].setMask(compuSnakePix[x].createHeuristicMask());
    }

    PIXMAP.load((const char *)(pixDir + "snake2.xpm"));
    for (int x = 0 ; x < 14; x++){
	samyPix[x].resize(16, 16);
	bitBlt(&samyPix[x] ,0,0, &PIXMAP,x*16, 0, 16, 16, CopyROP, TRUE);
	samyPix[x].setMask(samyPix[x].createHeuristicMask());
    }

    PIXMAP.load((const char *)(pixDir + "ball.xpm"));
    for (int x = 0 ; x < 4; x++){
	ballPix[x].resize(16, 16);
	bitBlt(&ballPix[x] ,0,0, &PIXMAP,x*16, 0, 16, 16, CopyROP, TRUE);
	ballPix[x].setMask(ballPix[x].createHeuristicMask());
    }

    PIXMAP.load((const char *)(pixDir + "apples.xpm"));
    for (int x = 0 ; x < 2; x++){
	applePix[x].resize(16, 16);
	bitBlt(&applePix[x] ,0,0, &PIXMAP,x*16, 0, 16, 16, CopyROP, TRUE);
	applePix[x].setMask(applePix[x].createHeuristicMask());
    }

    PIXMAP.load((const char *)(pixDir + "background.xpm"));

    int  pw = PIXMAP.width();
    int  ph = PIXMAP.height();

    backPix.resize(560, 560 );
    for (int x = 0; x <= 560; x+=pw)
	for (int y = 0; y <= 560; y+=ph)
	    bitBlt(&backPix, x, y, &PIXMAP);
    backPix.resize(560, 560 );

    PIXMAP.load((const char *)(pixDir + "brick.xpm"));
    pw = PIXMAP.width();
    ph = PIXMAP.height();

    offPix.resize(560, 560 );
    for (int x = 0; x <= 560; x+=pw)
	for (int y = 0; y <= 560; y+=ph)
	    bitBlt(&offPix, x, y, &PIXMAP);
    offPix.resize(560, 560 );
}

void PixServer::initRoomPixmap()
{
    QPainter paint;
    roomPix.resize(560, 560);
    bitBlt(&roomPix,0,0, &backPix);
    paint.begin(&roomPix);

    for (unsigned int x = 0; x < board->size(); x++) {
	if (board->isBrick(x))
	    drawBrick(&paint, x);
    }
    paint.end();
}

void PixServer::drawBrick(QPainter *p ,int i)
{
    QColor light("gray90");
    QColor dark("gray40");

    int topSq   = board->getNext(N, i);
    int botSq   = board->getNext(S, i);
    int rightSq = board->getNext(E ,i);
    int leftSq  = board->getNext(W, i);

    QRect rect = board->rect(i);

    int x = rect.x();
    int y = rect.y();

    int X = x;
    int Y = y;
    int width, height;
    int Width, Height;

    int highlight = 3;

    Width = Height = width = height = rect.width();


    p->fillRect(x, y, width, height, light);

    if(!board->isBrick(topSq))    {Y+=highlight; Height-=highlight;}
    if(!board->isBrick(leftSq))   {X+=highlight; Width-=highlight;}
    if(!board->isBrick(botSq))     Height-=highlight;
    if(!board->isBrick(rightSq))   Width-=highlight;

    QRect xyz(X,Y,Width,Height);

    bitBlt(&roomPix, xyz.x(), xyz.y(), &offPix,
	   xyz.x(), xyz.y(), xyz.width(), xyz.height());


    if (board->isBrick(topSq) && board->isBrick(rightSq)
	&& !board->isBrick(board->getNext(NE ,i)))
	p->fillRect(x+width-highlight, y, highlight, highlight, dark);

    if (board->isBrick(botSq) && board->isBrick(rightSq)
	&& !board->isBrick(board->getNext(SE, i)))
	p->fillRect(x+width -highlight, y+height-highlight, highlight, highlight, dark);

    if (board->isBrick(topSq) && board->isBrick(leftSq)
	&& !board->isBrick(board->getNext(NW, i)))
	p->fillRect(x, y, highlight, highlight, light);

    if (board->isBrick(leftSq) && board->isBrick(botSq)
	&& !board->isBrick(board->getNext(SW ,i)))
	p->fillRect(x, y+height-highlight, highlight, highlight, light);


    QBrush brush( dark);
    p->setBrush(brush);
    p->setPen(NoPen);

    QPointArray a;

    if(!board->isBrick(topSq) && !board->isBrick(rightSq)){
	a.setPoints( 4,
		     x+width-highlight, y+height-1,
		     x+width-1, y+height-1,
		     x+width-1, y+1,
		     x+width-highlight, y+highlight);
	p->drawPolygon(a);
    }
    else if(!board->isBrick(rightSq))
	p->fillRect(x + width -highlight, y, highlight, height, dark);

    if(!board->isBrick(leftSq) && !board->isBrick(botSq)){
	a.setPoints( 4,
		     x+width-1, y+height-highlight,
		     x+width-1, y+height-1,
		     x+1, y+height-1,
		     x+highlight, y+height-highlight);
	p->drawPolygon(a);
    }
    else if(!board->isBrick(botSq))
	p->fillRect(x , y+height-highlight, width, highlight, dark);
}
