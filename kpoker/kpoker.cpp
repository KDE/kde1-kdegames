/*
 * $Id$
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * 
 * 
 * Comment:
 * This is my first "bigger" application I did with Qt and my very first KDE application. 
 * So please apologize some strange coding styles --> if you can't stand some really
 * bad parts just send me a patch including your "elegant" code ;)
 *  
 */

#include <qpushbt.h>
#include <qlabel.h>
#include <qpopmenu.h>
#include <qtimer.h>
#include <qtooltip.h>


#include <kapp.h>
#include <kmenubar.h>
#include <kmsgbox.h>
#include <kkeyconf.h>



#include <time.h>
#include <stdio.h>

#include "kpoker.h" 
#include "kpoker.moc"

CardImages    *cardImage;
KApplication  *KPoker;


kpok::kpok(QWidget *parent, const char *name) 
: QWidget(parent, name)
{
	int w;
	char version[270];
	
       	setFixedSize(420,250);	
	
	sprintf(version, "%s %s", kapp->getCaption(), PVERSION);
	setCaption( version );
	
	
	QString bitmapdir = KPoker->kdedir() + QString("/lib/pics/kpoker/");
	
	
	/* KKeyCode initialization */
	kKeys->addKey("Quit", "ALT+Q");
	kKeys->addKey("New game", "F2");
	kKeys->addKey("Help", "F1");
	kKeys->addKey("Ok dialog", "Return");
	kKeys->addKey("Cancel dialog", "Escape");
	
	
	/* connections */
	kKeys->registerWidget("kpoker", this);
	kKeys->connectFunction("kpoker", "Quit", qApp, SLOT(quit()));
	kKeys->connectFunction("kpoker", "New game", this, SLOT(initPoker()));
	kKeys->connectFunction("kpoker", "Help", this, SLOT(help()));
	
	
	QFont myFixedFont("Helvetica",12);
	
	drawButton = new QPushButton(this,0);
	
	drawButton->setGeometry(210-30,CLHDistFromTop,60,30);
	drawButton->setText("Draw !");
	connect( drawButton, SIGNAL(clicked()), this, SLOT( drawClick() ) );
	
	for (w=0;w < 5; w++)
	  {
		  CardFrames[w] = new QFrame(this, 0);
		  CardFrames[w]->setGeometry(cardHDist+w*(cardHDist+cardWidth),cardDistFromTop,cardWidth +2,cardHeight +2);
		  CardFrames[w]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	  }
	
	
	QFont wonFont("Helvetica", 14, QFont::Bold);
	wonLabel = new QLabel(this, 0);
	
	wonLabel->hide();
	wonLabel->setFont(wonFont);
	wonLabel->setAutoResize(true);
	wonLabel->move(this->width() /2  - wonLabel->width() / 2, wonLabelVDist);
	
	
	QFont clickToHoldFont("Helvetica", 11);
	clickToHold = new QLabel(this,0);
	
	clickToHold->hide();
	clickToHold->setFont(clickToHoldFont);
	clickToHold->setAutoResize(true);
	clickToHold->setText("Click a card to hold it");
	clickToHold->move(this->width() /2  - clickToHold->width() / 2, clickToHoldVDist);
	
	
	/* load all cards into pixmaps */	
	
	cardImage = new CardImages(this,0);
	cardImage->hide();
	cardImage->loadCards(bitmapdir);
	
	for (w=0;w<5;w++) {
		cardW[w] = new CardWidget(CardFrames[w], 0);
		connect( cardW[w], SIGNAL(clicked()), cardW[w], SLOT( ownClick() ) );
		connect( cardW[w], SIGNAL(pClicked(CardWidget *)), this, SLOT(frameClick(CardWidget *)));
	}
	
	for (w=0; w<5;w++) {
		heldLabels[w] = new QLabel(this, 0);
		heldLabels[w]->hide();
		heldLabels[w]->setFont(myFixedFont);	       
		heldLabels[w]->setAutoResize(true);
		heldLabels[w]->setText("Held");
		heldLabels[w]->move(
				    ((cardWidth+2) /2 - heldLabels[w]->width() /2) +
				    cardHDist+w*(cardHDist + cardWidth), cardDistFromTop -15
				    );
		cardW[w]->heldLabel = heldLabels[w];
	}
	cashFrame = new QFrame(this,0);
	
	cashFrame->setGeometry(CLHBorderDistance, CLHDistFromTop, CLHWidth, 30);
	cashFrame->setFrameStyle(QFrame::Panel | QFrame::Plain);
	
	cashLabel= new QLabel(cashFrame,0);
	cashLabel->setAutoResize(true);
        cashLabel->setFont(myFixedFont);
	
	LHFrame = new QFrame(this,0);
	LHFrame->setGeometry(this->width() - CLHBorderDistance - CLHWidth, CLHDistFromTop, CLHWidth, 30);
	LHFrame->setFrameStyle(QFrame::Panel | QFrame::Plain);
	
	LHLabel= new QLabel(LHFrame,0);
	LHLabel->setAutoResize(true);
        LHLabel->setFont(myFixedFont);
	
	for (w=0; w<= highestCard / 4; w++)
	    cardHelp[w*4+1]=cardHelp[w*4+2]=cardHelp[w*4+3]=cardHelp[w*4+4]=w;
	
	blinkTimer = new QTimer(this);
	connect( blinkTimer, SIGNAL(timeout()), SLOT(bTimerEvent()) );
	blinkStat=0;
	
	drawTimer = new QTimer(this);
	connect (drawTimer, SIGNAL(timeout()), SLOT(drawCardsEvent()) );
	
	srandom(time(NULL));
	
	QToolTip::add( drawButton, "draw new cards" );
	QToolTip::add( LHLabel, "your last hand" );
        QToolTip::add( LHFrame, "your last hand" );
	QToolTip::add( cashLabel, "money left");
	QToolTip::add( cashFrame, "money left");
	
	
	initPoker();
}

void kpok::initPoker()
{
	status=0;
	int w;
	
	for (w=0;w<5;w++) {
		cards[w]=0;
		cardW[w]->heldLabel->hide();
		cardW[w]->show();
		cardW[w]->paintCard(cards[w],0,0);
	}
	wonLabel->hide();
	clickToHold->hide();
	
	drawStat=0;
	cleanFoundCard();
	setCash(100);
	setHand("nothing");
}



void kpok::drawCards(int skip[5])
{
	int got;
	int w;
	
	for(w=0;w<5; w++) {
		if (skip[w] == 0) {
			got=random() % highestCard;
			while(done[got] == 1) got=random() % highestCard;
			done[got]=1;
			cards[w]=(got+1);
		}
	}
}

void kpok::addFoundCard(int cardNum, int cardType)
{
	int found=0;
	int w=0;
	for (w=0; foundCards[w].cardType !=0 ; w++)
	    if (foundCards[w].cardNum == cardNum) found=1; 
	
	if (!found) {
		foundCards[w].cardNum=cardNum;
		foundCards[w].cardType=cardType;
	}
}

void kpok::cleanFoundCard()
{
	int w;
	for (w=0; w<5*2; w++) 
	    foundCards[w].cardType=0;
}


int kpok::testFlush()
{
	int w, mycolor;
	mycolor=cards[0] % 4;
	for (w=1;w<5;w++)
	    if (mycolor != cards[w] %4)
		return 0;
	
	for (w=0; w<5; w++)
	    addFoundCard(w,cards[w]);
	return 1;
}

int kpok::findCardTypes(int cardT[5], int card)
{
	int w;
	for (w=0; w<5; w++)
	    if (cardT[w] == card)
		return 1;
	return 0;
}

int kpok::testStraight()
{
	int cardTypes[5];
	int lowest=100; /* lowest cardtype --> set to something high first :) */
	int w; 
	
	for (w=0; w<5; w++) {
		cardTypes[w]=cardHelp[cards[w]];
		if (cardTypes[w]  < lowest) lowest=cardTypes[w];
	}
	
	for (w=0; w<5; w++)
	    if (!findCardTypes(cardTypes,lowest+w))
		return 0;
	
	for (w=0; w<5; w++)
	    addFoundCard(w,cards[w]);
	
	if (lowest == 0) return 2; // could be a royal flush :-o 
	return 1;
}


int kpok::testHand()
{
	int matching=0;
	int w,w2;
	int isRoyal;
	
	
	if ((isRoyal=testStraight())) {
		if (testFlush())
		  {
			  if (isRoyal == 2)
			      return 10; // royal flush detected
			  else
			      return 9; // straight flush detected
		  }
		
		else
		    return 7;
	}
	if (testFlush()) return 8;
	
	
	for(w=0;w < 5; w++) // this searches for pairs/three/four of a kind
	    for (w2=w+1; w2<5; w2++) {
		    if ( cardHelp[cards[w]] == cardHelp[cards[w2]] ) 
		      {
			      matching++;
			      addFoundCard(w,cards[w]);
			      addFoundCard(w2,cards[w2]);			    
		      }
	    }
	return matching;
}

void kpok::setCash(int newCash)
{
	char buf[255];
	cash = newCash;
	sprintf(buf,"Cash: $ %d",cash);
	cashLabel->setText(buf);
	cashLabel->move(cashFrame->width() / 2 - cashLabel->width() / 2, cashFrame->height() / 2 - cashLabel->height() / 2);
}

int  kpok::getCash()
{
	return cash;
}

void kpok::setHand(char *newHand)
{
	char buf[255];
	sprintf(buf,"Last Hand: %s", newHand);
	LHLabel->setText(buf);
	LHLabel->move(LHFrame->width() / 2 - LHLabel->width() / 2, LHFrame->height() / 2 - LHLabel->height() / 2);
}

void kpok::frameClick(CardWidget *MyCW)
{
	if (status != 0) {
		if (MyCW->toggleHeld() == 1) 
		    MyCW->heldLabel->show();
		else
		    MyCW->heldLabel->hide();
	}
}

void kpok::drawClick()
{
	int cardsToDraw[5];
	int w;
	
	drawButton->setEnabled(false);
	
	if (status == 0) {
		
		wonLabel->hide();
		
		cleanFoundCard();
		stopBlinking();
		
		setCash(getCash() - cashPerRound);
		
		for (w=0; w<5;w++) {
			cardsToDraw[w]=0; 
			cardW[w]->setHeld(0);
		}
		
		for (w=0; w<highestCard;w++) done[w]=0;
		
		for (w=0;w<5;w++)  {
			cards[w]=0;
			cardW[w]->heldLabel->hide();
			cardW[w]->show();
			cardW[w]->paintCard(cards[w],0,0);
		}
		
		drawCards(cardsToDraw);
		
		if (cardW[0]->queryHeld())
		    drawTimer->start(0, TRUE);
		else
		    drawTimer->start(drawDelay, TRUE);
		
	} 
	else {
		clickToHold->hide();
		for (w=0; w<5;w++) cardsToDraw[w] = cardW[w]->queryHeld();
		
		for (w=0;w<5;w++)  {
			if (!cardsToDraw[w]) {
				cards[w]=0;
				cardW[w]->show();
				cardW[w]->paintCard(cards[w],0,0);
			}
		}
		
		drawCards(cardsToDraw);
		
		
		if (cardW[0]->queryHeld())
		    drawTimer->start(0, TRUE);
		else
		    drawTimer->start(drawDelay, TRUE);
		
	}
}

void kpok::drawCardsEvent()
{
	int testResult;
	
	cardW[drawStat]->show();
	cardW[drawStat]->paintCard(cards[drawStat],0,0);
	
	if (drawStat == 4) /* just did last card */
	  {
		  drawButton->setEnabled(true);
		  drawStat=0;
		  if (status == 1)
		    {
			    testResult=testHand();
			    switch (testResult) {
			    case 1 : if (foundCards[0].cardType >= 17) {
				    foundCards[0].cardType=0; foundCards[1].cardType=0; displayWin("nothing",0); break;
			    }
				    displayWin("One Pair",5);  break;
			    case 2 : displayWin("Two Pairs", 10); break;
			    case 3 : displayWin("3 of a kind", 15); break;
			    case 4 : displayWin("Full House", 40); break;
			    case 6 : displayWin("4 of a kind", 125); break;
			    case 7 : displayWin("Straight",20); break;
			    case 8 : displayWin("Flush",25); break;
			    case 9 : displayWin("Straight Flush",250); break;
			    case 10 : displayWin("Royal Flush",2000); break;
				    
			    default: displayWin("nothing",0); break;
			    }
			    startBlinking();
			    status = 0;
			    
			    if (getCash() < cashPerRound)  {
				    KMsgBox::message(0,"You Lost", 
						     "Oops - you went bankrupt.\nA highscoretable will appear over here later...",
						     KMsgBox::EXCLAMATION,"New game");
				    initPoker();
			    }
			    
		    }
		  else {
			  clickToHold->show();
			  status =1;
		  }
		  
	  }
	else { /* only inc drawStat if not done with displaying */
		drawStat++;
		/* look at next card and if it is held instantly call drawCardEvent again */
		if (cardW[drawStat]->queryHeld()) 
		    drawTimer->start(0,TRUE);
		else
		    drawTimer->start(drawDelay,TRUE);
	}
}

void kpok::startBlinking()
{
	blinkTimer->start(650);
}

void kpok::stopBlinking()
{
	blinkTimer->stop();
	blinkStat=1;
}

void kpok::bTimerEvent()
{
	int w;
	
	if (blinkStat)
	  {
		  for (w=0; w<5; w++) {
			  if (foundCards[w].cardType != 0) {
				  cardW[foundCards[w].cardNum]->hide();
			  }
		  }
		  blinkStat=0;
	  }
	else {
		for (w=0; w<5; w++) {
			if (foundCards[w].cardType != 0) 
			  {
				  cardW[foundCards[w].cardNum]->show();
				  cardW[foundCards[w].cardNum]->paintCard(foundCards[w].cardType,0,0);
			  }
		}
		blinkStat=1;
	}
}

void kpok::displayWin(char *hand, int cashWon)
{
	char buf[200];
	
	setHand(hand);
	setCash(getCash() + cashWon);
	
	if (cashWon)
	    sprintf(buf,"You won $ %d !", cashWon);
	else
	    sprintf(buf,"Game Over");
	
	wonLabel->setText(buf);
	wonLabel->move(this->width() / 2 - wonLabel->width() / 2, wonLabelVDist);
	wonLabel->show();
}


void kpok::showAboutBox()
{
	char aboutText[250];
	sprintf(aboutText,"%s v%s (%s)\n\ncopyright 1997 by Jochen Tuchbreiter\n<whynot@digicron.com>\n\nCredits for cardimages go to\nJohn Fitzgibbon(http://www.sb.net/fitz/card.htm)\n\nSuggestions, bug reports etc. are welcome", kapp->getCaption(),PVERSION, PDATE);
	KMsgBox::message(0, kapp->getCaption(), aboutText);
}


void kpok::help()
{
	KApplication::getKApplication()->invokeHTMLHelp("kpoker/kpoker.html", "");
}
