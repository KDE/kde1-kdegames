#include<kapp.h>
#include "game.h"


int main( int argc, char **argv )
{
  KApplication a( argc, argv, "ksnake" );
  Game g;
  a.setMainWidget( &g );
  g.show();
  return a.exec();
}
