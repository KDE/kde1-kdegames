#include <qdatetm.h>
#include <stdlib.h>
#include <qtimer.h>
#include <qlist.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qkeycode.h>

#include <kapp.h>

#include "rattler.h"
#include "board.h"
#include "level.h"
#include "basket.h"
#include "ball.h"
#include "snake.h"
#include "pixServer.h"


QLabel *label = 0;

int speed[4] = { 85, 75, 55, 40 };

Rattler::Rattler( QWidget *parent, const char *name )
    : QWidget( parent, name )
{

    setFocusPolicy(QWidget::StrongFocus);

    KConfig *conf = kapp->getConfig();
    numBalls = conf->readNumEntry("Balls", 1);
    numSnakes = conf->readNumEntry("ComputerSnakes", 1);
    skill = conf->readNumEntry("Skill", 1);
    room = conf->readNumEntry("StartingRoom", 1);

    board = new Board(35*35);
    level = new Level(board);

    pix = new PixServer(board, this);
    setFixedSize(pix->levelPix().size());

    basket = new Basket(board, pix);
    samy = new SamySnake(board, pix);

    computerSnakes = new QList<CompuSnake>;
    computerSnakes->setAutoDelete( TRUE );

    balls = new QList<Ball>;
    balls->setAutoDelete( TRUE );

    connect( samy, SIGNAL(closeGate(int)), this, SLOT(closeGate(int)));
    connect( samy, SIGNAL(score(bool, int)), this, SLOT(scoring(bool,int)));
    connect( samy, SIGNAL(goingOut()), this, SLOT(speedUp()));
    connect( basket, SIGNAL(openGate()), this, SLOT(openGate()));

    QTime midnight( 0, 0, 0 );
    srand( midnight.secsTo(QTime::currentTime()) );

    gameState = Demo;
    timerCount = 0;
    QTimer::singleShot( 2000, this, SLOT(demo()) );
}

void Rattler::paintEvent( QPaintEvent *e)
{
    QRect rect = e->rect();
    bool dirty = FALSE;

    if (!rect.isEmpty()) {
	dirty = TRUE;
	bitBlt(this, rect.x(), rect.y(),
	       &pix->levelPix(), rect.x(), rect.y(), rect.width(), rect.height());
    }

    if (gameState != Init && gameState != Over) {

	basket->repaint();

	if(gameState != Demo)samy->repaint( dirty );

	for (CompuSnake *as = computerSnakes->first(); as != 0;
	     as = computerSnakes->next())
	    if (as) as->repaint( dirty );

	for (Ball *b = balls->first(); b != 0;
	     b = balls->next())
	    if (b) b->repaint();
    }
}

void Rattler::timerEvent( QTimerEvent * )
{

    timerCount++;

    if ( !leaving )		// advance progressBar unless Samy
	emit advance();		// is going out

    for (CompuSnake *c = computerSnakes->first(); c != 0;
	 c = computerSnakes->next())
	if(c) c->nextMove();

    for (Ball *b = balls->first(); b != 0;
	 b = balls->next())
	if (b) b->nextMove();

    samyState state = ok;

    if(gameState != Demo)
	state = samy->nextMove(direction);

    repaint(0,0,0,0, FALSE);

    if (state == ko) newTry();
    else if (state == out) levelUp();
}

void Rattler::closeGate(int i)
{
    board->set(i, brick);
    pix->restore(i);
}

void Rattler::openGate()
{
    board->set(NORTH_GATE, empty);
    pix->erase(NORTH_GATE);
}

void Rattler::scoring(bool win, int i)
{

    Fruits fruit  = basket->eaten(i);
    if (fruit == Bummer)
	return;
    if (gameState == Demo) win = TRUE;

    if (win) {
	switch (fruit) {
	case Red: if (!timerHasRunOut)
	    points += 1 + skill*2;
	else points++;
	break;
	case Golden: if (!timerHasRunOut)
	    points += 5 + (skill*2) + (numSnakes*2);
	else points += 2;
	break;
	case Bummer:
	default:
	    break;
	}
    }
    else {
	switch (fruit) {
	case Red: points -= 2;
	    break;
	case Golden: points -= 5;
	    break;
	case Bummer:
	default:
	    break;
	}
    }
    if (points < 0)
	points = 0;
    emit setPoints(points);

    if(points > check*50) {
	check++;
	if(gameState != Demo)
	    emit setTrys(++trys);
    }
}

void Rattler::pause()
{
    if (gameState == Playing) {
	gameState = Paused;
	killTimer(gameTimer );

	label = new QLabel(this);
	label->setFont( QFont( "Times", 18, QFont::Bold ) );
	label->setText("Game Paused\n Press F3 To Resume\n");
	label->setAlignment( AlignCenter );
	label->setFrameStyle( QFrame::Panel | QFrame::Raised );
	label->setGeometry(192, 206, 178, 80);
	label->show();
    }
    else if (gameState == Paused) {
	gameState = Playing;
	gameTimer = startTimer( speed [skill] );
	delete label;
    }

}

void Rattler::restartDemo()
{
    if (gameState != Demo)
	return;

    int r = rand() % 60000;
    QTimer::singleShot( r, this, SLOT(restartDemo()) );

    level->create(Samy);
    basket->clear();
    basket->newApples();
    restartBalls(FALSE);
    restartComputerSnakes(FALSE);
    repaint();
}


void Rattler::demo()
{
    static  bool first_time = TRUE;

    if(gameState == Init) return;

    QTimer::singleShot( 60000, this, SLOT(restartDemo()) );
    stop();
    gameState = Init;
    resetFlags();

    if(!first_time) {
	level->create(Samy);
	pix->initRoomPixmap();
    }
    repaint(rect(), FALSE);
    init(FALSE);
    runDemo();

    first_time = FALSE;
}

void Rattler::restart()
{
    if (gameState == Init) return;
    stop();
    gameState = Init;
    resetFlags();

    level->set(room);
    level->create(Banner);
    pix->initRoomPixmap();
    repaint();

    QTimer::singleShot( 2000, this, SLOT(showRoom()) );
}

void Rattler::newTry()
{
    stop();

    if(trys==0) {
	gameState = Over;
	level->create(GameOver);
	pix->initRoomPixmap();
	repaint();
	QTimer::singleShot( 5000, this, SLOT(demo()) );
	emit setScore(points);
	return;
    }
    --trys;
    gameState = Init;
    level->create(Room);
    pix->initRoomPixmap();
    init(TRUE);
    repaint();
    QTimer::singleShot( 1000, this, SLOT(runPlay()) );
}

void Rattler::levelUp()
{
    stop();
    gameState = Init;

    points += level->get()+5+(2*numSnakes)+numBalls;

    level->up();
    level->create(Banner);
    pix->initRoomPixmap();
    repaint();

    QTimer::singleShot( 2000, this, SLOT(showRoom()) );
}

/* this slot is called by the progressBar  when value() == 0
or by a compuSnake wich manages to exit */
void Rattler::restartTimer()
{
    timerHasRunOut = TRUE;
    timerCount = 0;
    emit rewind();

    if ( board->isEmpty(NORTH_GATE) )
	closeGate(NORTH_GATE);
    basket->newApples();
}

void Rattler::speedUp()
{
    leaving = TRUE;
    stop();
    start( 45 );
}

void Rattler::resetFlags()
{
    trys = 2;
    check  = 1;
    points = 0;
}

void Rattler::runDemo()
{
    gameState = Demo;
    start();
}

void Rattler::runPlay()
{
    direction = N;
    gameState = Playing;
    start();
}

void Rattler::showRoom()
{
    level->create(Room);
    pix->initRoomPixmap();
    init(TRUE);
    repaint();
    runPlay();
}

void Rattler::start()
{
    gameTimer = startTimer( speed [skill] );
}

void Rattler::start(int t)
{
    gameTimer = startTimer(t);
}

void Rattler::stop()
{
    killTimer( gameTimer );
}

void Rattler::init(bool play)
{
    leaving = FALSE;
    timerHasRunOut = FALSE;
    timerCount = 0;
    emit rewind();

    emit setTrys(trys);
    emit setPoints(points);

    basket->clear();
    basket->newApples();
    restartBalls(play);
    restartComputerSnakes(play);
    if(play) samy->init();
}

void Rattler::restartComputerSnakes(bool play)
{
    if( !computerSnakes->isEmpty())
	computerSnakes->clear();

    int i  = (play == FALSE && numSnakes == 0 ? 1 : numSnakes);

    for (int x = 0; x < i; x++) {
	CompuSnake *as = new CompuSnake(board, pix);
	connect( as, SIGNAL(closeGate(int)), this, SLOT(closeGate(int)));
	connect( as, SIGNAL(restartTimer()), this, SLOT(restartTimer()));
	connect( as, SIGNAL(score(bool, int)), this, SLOT(scoring(bool,int)));
	computerSnakes->append(as);
    }
}

void Rattler::restartBalls(bool play)
{
    if( !balls->isEmpty())
	balls->clear();

    int i = (play == FALSE && numBalls == 0 ? 1 : numBalls);

    for (int x = 0; x < i; x++) {
	Ball *b = new Ball(board, pix, BoardWidth+1+x);
	balls->append(b);
    }
}

void Rattler::setBalls(int i)
{
    Ball *b;
    numBalls = i;
    int count = balls->count();

    if (gameState == Playing || gameState == Demo) {
	if ( i > count) {
	    while ( i > count) {
		Ball *b = new Ball(board, pix, BoardWidth+1+count);
		balls->append(b);
		i--;
	    }
	}
	else if (i < count) {
	    while (i < count) {
		b = balls->getLast();
		b->zero();
		balls->removeLast();
		i++;
	    }
	}
    }
}

void Rattler::setCompuSnakes(int i)
{
    CompuSnake *cs;
    numSnakes = i;
    int count = computerSnakes->count();

    if (gameState == Playing || gameState == Demo) {
	if ( i > count) {
	    while ( i > count) {
		CompuSnake *as = new CompuSnake(board, pix);
		connect( as, SIGNAL(closeGate(int)), this, SLOT(closeGate(int)));
		connect( as, SIGNAL(restartTimer()), this, SLOT(restartTimer()));
		connect( as, SIGNAL(score(bool, int)), this, SLOT(scoring(bool,int)));
		computerSnakes->append(as);
		i--;
	    }
	}
	else if (i < count) {
	    while (i < count) {
		cs = computerSnakes->getLast();
		cs->zero();
		computerSnakes->removeLast();
		i++;
	    }
	}
    }
}

void Rattler::setSkill(int i)
{
    skill = i;
    if (gameState == Playing || gameState == Demo) {
	stop();
	start();
    }
}

void Rattler::setRoom(int i)
{
    room = i;
}

void Rattler::keyPressEvent( QKeyEvent *k )
{
    switch ( k->key()) {
    case Key_Up:
	direction = N;
	break;
    case  Key_Down:
	direction = S;
	break;
    case Key_Right:
	direction = E;
	break;
    case Key_Left:
	direction = W;
	break;
    default:
	k->ignore();
	break;
    }
}
