/* Start point for KAbablone */

#include "AbTop.h"

int main(int argc, char *argv[])
{
	KApplication app(argc, argv, "kabalone");
	AbTop abTop;

	app.setMainWidget( &abTop );
	abTop.show();
	
	return app.exec();
}

