#include "spiece.h"

#include <qpainter.h>
#include "drawButton.h"

#include "spiece.moc"

ShowNextPiece::ShowNextPiece( QWidget *parent, const char *name )
: QFrame( parent, name )
{
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	xOffset = -1;     /* -1 until first resizeEvent. */
}

void ShowNextPiece::resizeEvent( QResizeEvent *e )
{
	QSize sz = e->size();
	blockWidth  = (sz.width()  - 3)/5;
	blockHeight = (sz.height() - 3)/6;
	xOffset     = (sz.width()  - 3)/5;
	yOffset     = (sz.height() - 3)/6;
}

void ShowNextPiece::paintEvent( QPaintEvent * )
{
	QPainter p;
	
	p.begin( this );
	drawFrame( &p );
	p.end();
	emit update();
}

void ShowNextPiece::drawNextSquare(int x, int y,QColor *color)
{
	if (xOffset == -1) return;		/* Before first resizeEvent? */
	
	QPainter paint(this);
	drawTetrisButton( &paint, xOffset+x*blockWidth, yOffset+y*blockHeight,
					 blockWidth, blockHeight, color );
}
