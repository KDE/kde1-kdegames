#include <qpixmap.h>
#include <qstring.h>
#include <kapp.h>

#include "trys.h"


Trys::Trys( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
  QString pixDir;
  pixDir.setStr(KApplication::kdedir());
  pixDir.append("/share/apps/ksnake/pixmaps/");
  pixmap.load((const char *)(pixDir + "samy.xpm"));
  snakes = 2;
  repaint();
}

void Trys::paintEvent( QPaintEvent *)
{
  for ( int x = 0; x < snakes; x++)
      bitBlt(this, 30+ ( (pixmap.width()+16)*x), 2,
	     &pixmap, 0, 0, pixmap.width(), pixmap.height());
}

void Trys::set(int i)
{
  snakes = i;
  repaint();
}

