#include <stdlib.h>

#include <qobject.h>
#include <qdstream.h>
#include <qfile.h>
#include <qstring.h>
#include <qpushbt.h>
#include <qframe.h>
#include <qlabel.h>
#include <qgrpbox.h>
#include <qdatetm.h>
#include <qlined.h>

#include <kapp.h>

#include "score.h"


Score::Score( QWidget *parent, const char *name )
    :QObject( parent, name )
{
    QString libDir;
    libDir.setStr(KApplication::kdedir());
    libDir.append("/share/apps/ksnake/");
    file.setName( (const char *) (libDir + "highScores"));
    read();
}

void Score::display()
{
    QDialog *dlg = new QDialog(0, "Hall Of Fame", TRUE);
    dlg->setCaption("Snake Race High Scores");

    ScoreBox *sb1 = new ScoreBox(dlg);
    sb1->setGeometry(10, 10, 400, 225);
    sb1->setFixedSize(400, 225);
    sb1->setTitle("Hall of Fame");

    ScoreBox *sb2 = new ScoreBox(dlg);
    sb2->setGeometry(10, 240, 400, 225);
    sb2->setFixedSize(400, 225);
    sb2->setTitle("Today's High Scores");

    QPushButton *b = new QPushButton( dlg);
    b->setText("Ok");
    b->setAutoDefault(TRUE);
    b->setFocus();
    b->move(300, 480);
    connect( b, SIGNAL(clicked()),dlg, SLOT(accept()) );

    for ( int x = 0; x < 5; x++) {
	sb1->setScore(x, hall[x].points, hall[x].player);
	sb2->setScore(x, today[x].points, today[x].player);
    }

    dlg->exec();
    delete dlg;
}

QString Score::getPlayerName() {
    QDialog *dlg = new QDialog(0, "Hall Of Fame", TRUE);
    dlg->resize(300, 175);
    dlg->setCaption("Snake Race High Scores");
    QLabel *label  = new QLabel("you have achieved a high score!\n please enter your name", dlg);
    label->setAlignment(AlignCenter);
    label->setFont( QFont( "Times", 16, QFont::Bold ) );

    QLineEdit *le = new QLineEdit(dlg);
    le->setFocus();
    le->setText(playerName);
    le->selectAll();

    QFrame *sep = new QFrame( dlg);
    sep->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    QPushButton *b = new QPushButton("Ok", dlg);
    b->setDefault(TRUE);
    b->setAutoDefault(TRUE);
    connect(b, SIGNAL(released()), dlg, SLOT(accept()));
    connect(le, SIGNAL(returnPressed()), dlg, SLOT(accept()));

    label->setGeometry(0, 0, 300, 50 );
    le->setGeometry(50, 65, 200, 25 );
    sep->setGeometry(0, 100, 400, 25 );
    b->setGeometry(110, 125, 80, 32 );

    dlg->exec();

    QString s = le->text();
    delete dlg;
    return s;
}

void Score::setScore(int s)
{
    read();

    bool checkHall = FALSE;
    bool checkToday = FALSE;
    int x, xx;

    for ( x = 0; x < 5; x++)
	if ( s > hall[x].points) {
	    checkHall = TRUE;
	    break;
	}

    for ( xx = 0; xx < 5; xx++)
	if ( s > today[xx].points) {
	    checkToday = TRUE;
	    break;
	}

    if (checkHall == TRUE || checkToday == TRUE) {

	playerName  = getPlayerName();

	if (checkHall) {
	    for (int i = 4; i > x && i > 0; i--)
		hall[i] = hall[i-1];

	    hall[x].points = s;
	    hall[x].player = playerName.data() ;
	}

	if (checkToday) {
	    for (int i = 4; i > xx && i > 0; i--)
		today[i] = today[i-1];

	    today[xx].points = s;
	    today[xx].player =  playerName.data();
	    today[xx].date = QDate::currentDate();
	}

	display();
	write();
    }
}

void Score::read()
{

    if ( file.exists() ) {
	if (file.open( IO_ReadOnly )) {
	    QDataStream s( &file );
	    for ( int x = 0; x < 5; x++)
		s >> hall[x].points >> hall[x].player;
	    for ( int x = 0; x < 5; x++) {
		s >> today[x].points >> today[x].player >> today[x].date;
		if ( today[x].date != QDate::currentDate())
		    today[x].points = 0;
	    }
	    file.close();
	}
    } else {
	for( int x = 0; x < 5; x++) {
	    hall[x].points = 0;
	    hall[x].player = "";

	    today[x].points = 0;
	    today[x].player = "";
	    today[x].date = QDate::currentDate();
	}
	write();
    }
}

void Score::write()
{

    if (file.open( IO_WriteOnly )) {
	QDataStream s( &file );
	for ( int x = 0; x < 5; x++)
	    s << hall[x].points << hall[x].player;
	for ( int x = 0; x < 5; x++)
	    s << today[x].points << today[x].player << today[x].date;
	file.close();
    }
}

ScoreBox::ScoreBox( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    init();
}

void ScoreBox::init()
{
    box = new QGroupBox(this);

    label = new QLabel(this);
    label->setFrameStyle( QFrame::Panel   | QFrame::Raised );
    label->setFont( QFont( "Times", 18, QFont::Bold ) );
    label->setAlignment( AlignCenter );

    for ( int x = 0; x < 5; x++) {
	QLabel *l = new QLabel(this);
	CHECK_PTR(l);
	l->setFont( QFont( "Times", 16, QFont::Bold ) );
	l->setAlignment( AlignRight );
	points[x] = l;

	QLabel *l2 = new QLabel(this);
	CHECK_PTR(l2);
	l2->setFont( QFont( "Times", 16, QFont::Bold ) );
	l2->setAlignment( AlignLeft );
	player[x] = l2;
    }
}

void ScoreBox::setTitle( const char *s )
{
    label->setText( s );
}

void ScoreBox::setScore( int x, int p, const char * pl )
{
    if (x > 5)
	return;

    if (p == 0) {
	points[x]->setText("");
	player[x]->setText("");
    }
    else {
	QString s;
	s.sprintf("%d",p);
	points[x]->setText(s.data());
	player[x]->setText(pl);
    }
}

void ScoreBox::resizeEvent( QResizeEvent * )
{
    box->resize( width(), height() - 25 );
    box->setGeometry(0, 25, width(), height()-25 );

    label->setGeometry(100, 10, 200, 30 );

    int p = 65;
    for ( int x = 0; x < 5; x++) {
	points[x]->setGeometry(25, p, 100, 25);
	player[x]->setGeometry(150, p, 225, 25);
	p+= 30;
    }
}
