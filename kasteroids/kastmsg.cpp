
#include <qpainter.h>
#include "kastmsg.h"


KAstMsg::KAstMsg( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    setBackgroundColor( black );
}

void KAstMsg::setMessage( const char *m )
{
    msg = m;
    QPainter painter;
    painter.begin( this );
    QRect rect = painter.fontMetrics().boundingRect( msg );
    resize( rect.width(), rect.height() + 4 );
    painter.end();
}

void KAstMsg::paintEvent( QPaintEvent *pe )
{
    QPainter painter;

    painter.begin( this );
    int ascent = painter.fontMetrics().ascent();
    painter.setPen( yellow );
    painter.drawText( 0, ascent, msg );
    painter.end();
}

