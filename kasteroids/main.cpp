
#include <kapp.h>
#include <klocale.h>
#include <time.h>
#include "toplevel.h"

#include <X11/Xlib.h>

KLocale locale( "kasteroids" );

int main( int argc, char *argv[] )
{
    KApplication app( argc, argv, "kasteroids" );

    srandom( time(0) );

    KAstTopLevel mainWidget;
    mainWidget.show();
    app.setMainWidget( &mainWidget );

    app.exec();

    XAutoRepeatOn( qt_xdisplay() );

    return 0;
}

