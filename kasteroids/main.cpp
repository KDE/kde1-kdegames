
#include <kapp.h>
#include <time.h>
#include "toplevel.h"

int main( int argc, char *argv[] )
{
    KApplication app(argc,argv, "kasteroids");

    srandom( time(0) );

    KAstTopLevel mainWidget;
    mainWidget.show();
    app.setMainWidget( &mainWidget );

    return app.exec();
}

