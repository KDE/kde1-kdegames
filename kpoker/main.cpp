#include <kapp.h>

#include <top.h>

int main( int argc, char *argv[] )
{
	KApplication *myApp = new KApplication( argc, argv );
	PokerWindow *MainScreen = new PokerWindow;
	
	myApp->setMainWidget( MainScreen );
	MainScreen->show();
		  
	int ret = myApp->exec();
		  
	delete MainScreen;
	delete myApp;
	
	return ret;

}
