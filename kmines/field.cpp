#include "field.h"
#include "defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <qpainter.h>
#include <qdrawutl.h>
#include <qpixmap.h>

#include "field.moc"

Field::Field( QWidget *parent, const char *name)
: QWidget( parent, name )
{
	initMetaObject();
	
	srandom(time(NULL));
	
	left_down = FALSE; 
	mid_down  = FALSE;
	stop = FALSE;
	pfield = NULL;
	isPaused = FALSE;
  
	pt = new QPainter();
  
	pm_flag = new QPixmap(16,16);
	pm_flag->fill(backgroundColor());
	pt->begin(pm_flag);
	pt->setPen(black);
	pt->drawLine(6, 13, 14, 13);
	pt->drawLine(8, 12, 12, 12);
	pt->drawLine(9, 11, 11, 11);
	pt->drawLine(10, 2, 10, 10);
	pt->setPen(red);
	pt->setBrush(red);
	pt->drawRect(4, 3, 6, 5);
	pt->end();
	
	pm_mine = new QPixmap(20,20);
	pm_mine->fill(backgroundColor());
	pt->begin(pm_mine);
	createMinePixmap();
	pt->end();
  
	pm_exploded = new QPixmap(20,20);
	pm_exploded->fill(red);
	pt->begin(pm_exploded);
	createMinePixmap();
	pt->end();
	
	pm_error = new QPixmap(20,20);
	pm_error->fill(backgroundColor());
	pt->begin(pm_error);
	createMinePixmap();
	pt->setPen(red);
	pt->drawLine(3, 3, 17, 17);
	pt->drawLine(4, 3, 17, 16);
	pt->drawLine(3, 4, 16, 17);
	pt->drawLine(3, 17, 17, 3);
	pt->drawLine(3, 16, 16, 3);
	pt->drawLine(4, 17, 17, 4);
	pt->end();
	
	setFont( QFont::QFont("Helvetica", 13, QFont::Bold) );
	
	msg = new QLabel(this);
	msg->hide();
	msg->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	msg->setAlignment(AlignCenter);

	pb = new QPushButton(this);
	pb->hide();
	connect( pb, SIGNAL(clicked()),
			 this, SLOT(resume()) );
}

void Field::adjustSize()
{
	char spa[] = "Game paused";
	char sre[] = "Press to resume";
	int sw, sh, sw2, sh2;
	
	QFontMetrics fm = msg->fontMetrics();
	sw = fm.width(spa) + LABEL_WC;
	sh = fm.height() + LABEL_HC;
	QFontMetrics fm2 = pb->fontMetrics();
	sw2 = fm2.width(sre) + LABEL_WC;
	sh2 = fm2.height() + LABEL_HC;

	msg->setGeometry((width() - sw)/2, (height() - sh-sh2)/3, sw,sh);
	msg->setText(spa);
	pb->setGeometry((width() - sw2)/2, 2*(height() - sh-sh2)/3, sw2, sh2);
	pb->setText(sre);
}

void Field::createMinePixmap()
{
	pt->setPen(black);
	pt->drawLine(0,0,19,0);
	pt->drawLine(0,19,19,19);
	pt->drawLine(0,1,0,18);
	pt->drawLine(19,1,19,18);
	pt->drawLine(10,3,10,16);
	pt->drawLine(3,10,16,10);
	pt->drawPoint(5,5); pt->drawLine(8,5,12,5); pt->drawPoint(15,5);
	pt->drawRect(6,6,9,2);
	pt->drawRect(5,8,3,2); pt->drawRect(10,8,6,2);
	pt->drawRect(5,11,11,2);
	pt->drawRect(6,13,9,2);
	pt->drawPoint(5,15); pt->drawLine(8,15,12,15); pt->drawPoint(15,15);
}


int Field::computeNeighbours(int i, int j)
{
	int nm = 0;
	
	if (pfield[i-1][j]   & MINE) nm++;
	if (pfield[i-1][j+1] & MINE) nm++;
	if (pfield[i-1][j-1] & MINE) nm++;
	if (pfield[i][j+1]   & MINE) nm++;
	if (pfield[i][j-1]   & MINE) nm++;
	if (pfield[i+1][j]   & MINE) nm++;
	if (pfield[i+1][j+1] & MINE) nm++;
	if (pfield[i+1][j-1] & MINE) nm++;
	
	return nm;
}


void Field::Start(int nb_width, int nb_height, int nb_mines)
{
	int i,j;

	/* if game is paused : resume */
	if ( isPaused ) {
		resume();
		emit freezeTimer();
	}
	
	/* nb_w must be the old value ! */
	if (pfield!=NULL) {
		for (i=0; i<nb_w+2; i++) delete pfield[i];
		delete pfield;
	}
  
	nb_w = nb_width;
	nb_h = nb_height;
	nb_m = nb_mines;
	
	stop = FALSE;
	first_click = TRUE;
	
	/* reserve memory */
	pfield = new int *[nb_w+2];
	for (i=0; i<nb_w+2; i++)   
	    pfield[i] = new int [nb_h+2];
	
	/* initialize field */
	for (i=1; i<=nb_w; i++)
	    for (j=1; j<=nb_h; j++)
	        pfield[i][j] = COVERED;
  
	/* for the sake of the uncover process */
	for (i=0; i<nb_w+2; i++) {
		pfield[i][0] = UNCOVERED;
		pfield[i][nb_h+1] = UNCOVERED;
	}
	
	for (j=1; j<=nb_h; j++) {
		pfield[0][j] = UNCOVERED;
		pfield[nb_w+1][j] = UNCOVERED;
	}
  
	for(int k=0; k<nb_m; k++) {
		do {
			i = random() % nb_w;
			j = random() % nb_h;
		}
		while(pfield[i+1][j+1] & MINE);

		pfield[i+1][j+1] |= MINE;
	}
	
	adjustSize();
	repaint();
}


void Field::paintEvent( QPaintEvent *)
{
	for (int i=1; i<=nb_w; i++)
	    for (int j=1; j<=nb_h; j++)
		    drawCase(i, j, computeNeighbours(i,j));
}


void Field::changeCaseState(int i, int j, int new_st)
{
	if (pfield[i][j] & MINE) {
		emit changeCase(pfield[i][j] ^ MINE,-1);
		pfield[i][j] = MINE | new_st;
	} else {
		emit changeCase(pfield[i][j],-1);
		pfield[i][j] = new_st;
	}
  
	emit changeCase(new_st,1);
  
	if (!stop)
	    emit updateStatus();
	drawCase(i,j,computeNeighbours(i,j));
}


#define ipos (i-1)*CASE_W
#define jpos (j-1)*CASE_W

void Field::drawCase(int i, int j, int nbs)
{
	char nb[1];
	
	pt->begin(this);
	pt->eraseRect(ipos, jpos, CASE_W, CASE_W);
  
	if (pfield[i][j] & COVERED)
	    qDrawWinPanel(pt, ipos, jpos, CASE_W, CASE_W, colorGroup());
	else if (pfield[i][j] & MARKED) {
		qDrawWinPanel(pt, ipos, jpos, CASE_W, CASE_W, colorGroup());
		bitBlt( this, ipos+2, jpos+2, pm_flag );
	} else if (pfield[i][j] & UNCERTAIN) {
		qDrawWinPanel(pt, ipos, jpos, CASE_W, CASE_W, colorGroup());
		nb[0] = '?'; nb[1] = 0;
		pt->setPen(black);
		pt->drawText(ipos, jpos, CASE_W, CASE_W, AlignCenter, nb);
	} else if (pfield[i][j] & ERROR)
	    bitBlt( this, ipos, jpos, pm_error );
	else {
		if (pfield[i][j] & MINE) {
			if (pfield[i][j] & EXPLODED)
			    bitBlt( this, ipos, jpos, pm_exploded);
			else
			    bitBlt( this, ipos, jpos, pm_mine);
		} else {
			qDrawWinPanel(pt, ipos, jpos, CASE_W, CASE_W, colorGroup(), TRUE);
			if (nbs!=0) {
				nb[0] = nbs + '0'; nb[1] = 0;
				switch(nb[0]) {
				 case '1'  : pt->setPen(blue);        break;
				 case '2'  : pt->setPen(darkGreen);   break;
				 case '3'  : pt->setPen(darkYellow);  break;
				 case '4'  : pt->setPen(darkMagenta); break;
				 case '5'  : pt->setPen(red);         break;
				 case '6'  : pt->setPen(darkRed);     break;
				 case '7'  :
				 case '8'  : pt->setPen(black);       break;
				}
				pt->drawText(ipos, jpos, CASE_W, CASE_W, AlignCenter, nb);
			}
		}
	}
    pt->end();
}


void Field::uncover(int i, int j)
{
	int nbs;
	
	if ( !(pfield[i][j] & COVERED) )
	    return;
	nbs = computeNeighbours(i,j);
  
	if (!nbs) {
		changeCaseState(i,j,UNCOVERED);
		uncover(i-1,j+1);
		uncover(i-1,j  );
		uncover(i-1,j-1);
		uncover(i,  j+1);
		uncover(i,  j-1);
		uncover(i+1,j+1);
		uncover(i+1,j  );
		uncover(i+1,j-1);
	}else 
	    changeCaseState(i,j,UNCOVERED);
}

#define icpos (ic-1)*CASE_W
#define jcpos (jc-1)*CASE_W
void Field::mousePressEvent( QMouseEvent *e )
{
	if (stop)
	    return;

	updateSmiley(STRESS);

	ic = e->pos().x() / CASE_W + 1;
	jc = e->pos().y() / CASE_W + 1;
	
	if (first_click) {
		emit startTimer();
		first_click = FALSE;
	}

	if (e->button()==LeftButton) {
		left_down = TRUE;
		pressCase(ic, jc, TRUE);
	} else if (e->button()==RightButton) {
		if (pfield[ic][jc] & COVERED)
			changeCaseState(ic, jc, MARKED);
		else if (pfield[ic][jc] & MARKED) { /* ? mark option */
			if (u_mark)
				changeCaseState(ic, jc, UNCERTAIN);
			else
				changeCaseState(ic, jc, COVERED);
		} else if (pfield[ic][jc] & UNCERTAIN)
			changeCaseState(ic, jc, COVERED);
	} else if (e->button()==MidButton) {
		mid_down = TRUE;
		pressClearFunction(ic, jc, TRUE);
	}
}

void Field::mouseReleaseEvent( QMouseEvent *e )
{
	if (stop)
		return;

	/* if not out of the limits of the field */
	if (ic>=1 && ic<=nb_w && jc>=1 && jc<=nb_h) {
		if (e->button()==LeftButton) {
			left_down = FALSE;
			uncoverCase(ic, jc);
		} else if (e->button()==MidButton) {
			mid_down = FALSE;
			clearFunction(ic, jc);
		}
	}
  
	if (!stop) 
		updateSmiley(OK);
}

void Field::mouseMoveEvent( QMouseEvent *e )
{
	if (stop)
		return; 

	/* if not out of the limits of the field */
	if (ic>=1 && ic<=nb_w && jc>=1 && jc<=nb_h) {
		if (left_down)
			pressCase(ic, jc, FALSE);
		else if (mid_down)
			pressClearFunction(ic, jc, FALSE);
	}
	
	ic = e->pos().x() / CASE_W + 1;
	jc = e->pos().y() / CASE_W + 1;

	/* if not out of the limits of the field */
	if (ic>=1 && ic<=nb_w && jc>=1 && jc<=nb_h) {
		if (left_down)
			pressCase(ic, jc, TRUE);
		else if (mid_down)
			pressClearFunction(ic, jc, TRUE);
	} 
}

void Field::showMines(int i0, int j0)
{
	for(int i=1; i<=nb_w; i++)
		for(int j=1; j<=nb_h; j++)
		    if (pfield[i][j] & MINE) {
				if ( !(pfield[i][j] & EXPLODED) )
					changeCaseState(i,j,UNCOVERED);
			} else if (pfield[i][j] & MARKED)
	            changeCaseState(i,j,ERROR);
  
	changeCaseState(i0,j0,EXPLODED);
}

void Field::Stop()
{
	stop = TRUE;
}

void Field::pressCase(int i, int j, int state)
{
	if (pfield[i][j] & COVERED) {
		pt->begin(this);
		pt->eraseRect(ipos, jpos, CASE_W, CASE_W);
		qDrawWinPanel(pt, ipos, jpos, CASE_W, CASE_W, colorGroup(), state);
		pt->end();
	}
}

void Field::pressClearFunction(int i, int j, int state)
{
	pressCase(i-1, j+1, state);
	pressCase(i-1, j  , state);
	pressCase(i-1, j-1, state);
	pressCase(i  , j+1, state);
	pressCase(i  , j  , state);
	pressCase(i  , j-1, state);
	pressCase(i+1, j-1, state);
	pressCase(i+1, j  , state);
	pressCase(i+1, j+1, state);
}

void Field::clearFunction(int i, int j)
{
	pressClearFunction(i, j, FALSE);
	
	if (pfield[i][j] & COVERED)
		return; 
	
	int nm;
	/* number of mines around the case */
	nm = computeNeighbours(i, j);
	
	if (pfield[i-1][j]   & MARKED) nm--;
	if (pfield[i-1][j+1] & MARKED) nm--;
	if (pfield[i-1][j-1] & MARKED) nm--;
	if (pfield[i][j+1]   & MARKED) nm--;
	if (pfield[i][j-1]   & MARKED) nm--;
	if (pfield[i+1][j]   & MARKED) nm--;
	if (pfield[i+1][j+1] & MARKED) nm--;
	if (pfield[i+1][j-1] & MARKED) nm--;
	
	if (!nm) { /* the number of surrounding mines is equal */
		       /* to the number of marks :) */
		uncoverCase(i+1, j+1);
		uncoverCase(i+1, j);
		uncoverCase(i+1, j-1);
		uncoverCase(i,   j+1);
		uncoverCase(i,   j-1);
		uncoverCase(i-1, j+1);
		uncoverCase(i-1, j);
		uncoverCase(i-1, j-1);
	}
}
	  
void Field::uncoverCase(int i, int j)
{
	if (pfield[i][j] & COVERED) {
		if (pfield[i][j] & MINE)
		    changeCaseState(i, j, UNCOVERED);
		else
			uncover(i, j);
	}
  
	/* to enable multiple explosions ... */
	if ( (pfield[i][j] & MINE) && (pfield[i][j] & UNCOVERED) ) {
		emit endGame(FALSE);
		showMines(i, j);
	}
}

void Field::setUMark(int um)
{
	u_mark = um;
}

void Field::pause()
{
	if (first_click || stop)
		return;
	
	/* if already in pause : resume game */
	if ( isPaused ) {
		resume();
		return;
	}

	emit freezeTimer();

	QPainter *p = new QPainter;
	p->begin(this);
	p->eraseRect(0, 0, width(), height());
	p->end();
	
	msg->show();
	pb->show();
	
	isPaused = TRUE;
}

void Field::resume()
{
	isPaused = FALSE;
	
	msg->hide();
	pb->hide();
	
	emit startTimer(); 
}