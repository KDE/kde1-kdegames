#include "dialogs.h"
#include "defines.h"
#include "net.h"

#include <stdlib.h>

#include <qpushbt.h>
#include <qfileinf.h>
#include <qmsgbox.h>

#include <kapp.h>
#include <Kconfig.h>

#include "dialogs.moc"


#define DEC   10
#define HDEC  -2
#define I_LAB 30
#define N_LAB 70
#define S_LAB 60
#define DEC2  DEC + I_LAB + 5
#define DEC3  DEC2 + N_LAB + 40
#define iDEC  DEC + i*(HDEC+LABEL_H)

/* HighScore dialogs */
WHighScores::WHighScores( bool show, int score,
						  QWidget *parent, const char *name)
: QDialog(parent, name, TRUE)
{
	kconf = kapp->getConfig();
	
	showHS(show, score);
}

void WHighScores::showHS( bool show, int score)
{
	int i;
	
	range = -1;
	QString str1, str2;
	
	kconf->setGroup(HS_GRP);
	/* set highscore ? */
	if ( !show ) {
        /* a better score ?? */
        for (i=0; i<NB_HS; i++) {
			str1.sprintf("%s%i", HS_SCORE_KEY, i);
            if ( score > kconf->readNumEntry(str1) ) {
				range = i;
				break;
			}
		}
		if (range == -1)
			return;

		for (i=NB_HS; i>range; i--) {
			str1.sprintf("%s%i", HS_SCORE_KEY, i);
			str2.sprintf("%s%i", HS_SCORE_KEY, i-1);
			kconf->writeEntry(str1, kconf->readEntry(str2));
			str1.sprintf("%s%i", HS_NAME_KEY, i);
            str2.sprintf("%s%i", HS_NAME_KEY, i-1);
            kconf->writeEntry(str1, kconf->readEntry(str2));
		}
		
		str1.sprintf("%s%i", HS_SCORE_KEY, range);
		kconf->writeEntry(str1, score);
	}
			
	setCaption("Hall of Fame");
		
	QLabel *label;
	
	for (i=0; i<NB_HS; i++) {
		str1.setNum(i);
		ADD_LABEL(label, str1, DEC, iDEC, I_LAB, LABEL_H);
		str1.sprintf("%s%i", HS_NAME_KEY, i);
		if ( show || range !=i ) {
			ADD_LABEL( label, kconf->readEntry(str1), DEC2, iDEC,
					  N_LAB, LABEL_H );
			label->setAlignment( AlignLeft | AlignVCenter);
		} else {
			ADD_LABEL( lb, "", DEC2, iDEC, N_LAB, LABEL_H );
			lb->setAlignment( AlignLeft | AlignVCenter);
			lb->hide();
			ADD_EDIT( qle, 15, DEC2, iDEC, N_LAB, LABEL_H);
			connect( qle,  SIGNAL(returnPressed()),
					 this, SLOT(writeName()) );
		}
		str1.sprintf("%s%i", HS_SCORE_KEY, i);
		ADD_LABEL( label, kconf->readEntry(str1), DEC3, iDEC,
				   S_LAB, LABEL_H );
        label->setAlignment( AlignRight | AlignVCenter);
	}

	ADD_BUTTON( pb, "Ok", (DEC3+S_LAB+DEC-70)/2, iDEC + 20, 70, LABEL_H,
			    accept() );
	
	if ( !show )
		pb->hide();
	
	adjustSize();
	exec();
}

void WHighScores::writeName()
{
	QString str = qle->text();
	if ( str.isNull() )
		str = "Anonymous";

	QString str2;
	str2.sprintf("%s%i", HS_NAME_KEY, range);
	kconf->writeEntry(str2, str);
	
	/* show the entered highscore */
	delete qle;
	pb->show();
	lb->setText(str);
	lb->show();
}
     


/* Options dialog */
Options::Options( QWidget *parent, const char *name )
: QDialog(parent, name, TRUE)
{
	initMetaObject();

	QLabel *ql;
	ADD_LABEL(ql, "Options :", 10, 10, 90, LABEL_H); 
	
	QPushButton *pb;
	ADD_BUTTON(pb, "Ok", 35, 130, 50, 30, accept());
	
	resize(120,170);
	exec();
}


/* network game Dialog */
netDialog::netDialog( NetObject **p_net_object,
					  QWidget *parent, const char *name )
: QDialog(parent, name, TRUE)
{
	initMetaObject();

	timer = new QTimer();
	connect( timer, SIGNAL(timeout()), SLOT(timeout()) );
	
	p_net_obj = p_net_object;
	
	/* create title label */
	ADD_LABEL( labTitle, "Network Dialog", 10, 10, 150, LABEL_H);
		
	/* create the choice buttons (SERVER, CLIENT & CANCEL) */
	ADD_BUTTON( butServer, "Server", 35, 50, 100, 30, server());
	ADD_BUTTON( butClient, "Client", 35, 90, 100, 30, client());
	ADD_BUTTON( butCancel, "Cancel", 35, 130, 100, 30, cancel());

	/* create labels and edit lines for ASK_ADD */
	ADD_LABEL( labNameE, "Player name :", 10, 40, 100, LABEL_H);
	labNameE->hide();
	ADD_LABEL( labAddE, "Server address :", 10, 60, 100, LABEL_H );
	labAddE->hide();
	ADD_LABEL( labPortE, "Port : ", 10, 90, 100, LABEL_H); labPortE->hide();

	ADD_LABEL( labName, "", 120, 40, 100, LABEL_H ); labName->hide();
	ADD_LABEL( labAdd, "", 120, 60, 200, LABEL_H ); labAdd->hide();
	ADD_LABEL( labPort, "", 120, 90, 50, LABEL_H ); labPort->hide();

	ADD_EDIT( edName, 10, 120, 40, 100, 20); edName->hide();
	ADD_EDIT( edAdd, 40, 120, 60, 200, 20); edAdd->hide();
	ADD_EDIT( edPort, 4, 120, 90, 40, 20); edPort->hide();
	
	/* create buttons and label for CONNECT & WAIT_PLAYERS */
	ADD_BUTTON( butConnect, "Connect", 200, 130, 100, LABEL_H, connecting() );
	butConnect->hide();

	ADD_LABEL( labState, "Connecting ...", 160, 130, 200, LABEL_H );
	labState->setAlignment( AlignCenter ); labState->hide();
	
	ADD_BUTTON( butPlay, "PLAY !",  200, 130, 100, LABEL_H, play() );
	butPlay->hide();

	/* variable that indicates in which state we are */
	state = ASK_MODE;
	
	adjustSize();
}

void netDialog::showConnect(int new_state)
{
	int i;
	QString str;
	QLabel *label;
	
	state = new_state;
	
	/* ASK_MODE : is called if cancel in ASK_ADD */
	if ( state==ASK_MODE ) {
		/* change title label */
		labTitle->setText("Network Dialog");
		labTitle->resize(150, LABEL_H);
		/* hide labels & edLines used in ASK_ADD */
		labNameE->hide();
		labAddE->hide();
		labPortE->hide();
		if ( net_obj->isServer() )
			labAdd->hide();
		else
			edAdd->hide();
		edPort->hide();
		edName->hide();
		/* hide connect button and show server/client buttons */
		butConnect->hide();
		butServer->show();
		butClient->show();
	}
	
	/* ASK_ADD */
	if ( state==ASK_ADD ) {
		str = "Network Game";
		/* change the title label */
		if ( net_obj->isServer() )
			str += " : Server mode";
		else
			str += " : Client mode";
		labTitle->resize(200, LABEL_H); 
		labTitle->setText(str);
		
		/* hide SERVER & CLIENT buttons */
		butServer->hide();
		butClient->hide();
		
		/* show the two labels "server address" & "port" */
		labNameE->show();
		labAddE->show();
		labPortE->show();
		
		/* show enventually the label for address in server mode */
		if ( net_obj->isServer() ) {
			labAdd->setText(net_obj->t_address);
			labAdd->show();
		}
		
		/* show edit lines */
		edAdd->setText(net_obj->t_address);
		if ( net_obj->isClient() )
			edAdd->show();
		edPort->setText(net_obj->t_port);
		edPort->show();
		edName->setText(net_obj->t_name);
		edName->show();
			
		/* show CONNECT button */
		butConnect->show();
	}

	/* RE_ASK_ADD */
	if ( state==RE_ASK_ADD ) {
		/* hide labels formed from edit lines & reshow edits */
		if ( net_obj->isClient() ) {
			labAdd->hide();
			edAdd->show();
		}
		labPort->hide();
		edPort->show();
		labName->hide();
		edName->show();

		/* hide state label and show connect button */
		labState->hide();
		butConnect->show();
	}
	
	/* CONNECT */
	if ( state==CONNECT ) {
		/* hide edit lines & create the correspnding labels */
		if ( net_obj->isClient() ) {
			edAdd->hide();
			net_obj->t_address = edAdd->text();
			labAdd->setText(net_obj->t_address);
			labAdd->show();
		}
		edPort->hide();
		/* t_port has been updated in net_obj->checkParam */
		labPort->setText(net_obj->t_port);
		labPort->show();
		edName->hide();
		net_obj->t_name = edName->text();
		labName->setText(net_obj->t_name);
		labName->show();
		
		/* hide CONNECT button & show the corresponding label */
		butConnect->hide();
		labState->show();
		
		oldNbPlayers = 0;
	}
	
	/* WAIT_PLAYERS */
	if ( state==WAIT_PLAYERS ) {
		/* update state label */
		if ( net_obj->getNbPlayers()==0 ) {
			labState->setText("Waiting for other(s) player(s) ...");
			labState->show();
			butPlay->hide();
		}
		/* delete labels & create the new list */
		for (i=1; i<=net_obj->getNbPlayers(); i++) {
			net_obj->deleteLabel(i);
			net_obj->getPlayerInfos(net_obj->client_name, i);
			ADD_LABEL( label, net_obj->client_name, 
					   20, 150+i*30, 300, LABEL_H );
				label->show();
			net_obj->createLabel(i, label);
		}
		/* if the number of players is >= 2 */
		/* delete state label & create "PLAY" button if not already */
		if ( net_obj->getNbPlayers()>=1 ) {
			labState->hide();
			butPlay->show();
		} 
		oldNbPlayers = net_obj->getNbPlayers();
	}

	adjustSize();
}


void netDialog::createNetObject(bool server)
{
	QString tmp_add, tmp_name, tmp_port;
	
	net_obj = *p_net_obj;
	
	/* if net_object exists and if net_object is client_type/server_type
	   : delete it */
	if ( net_obj!=NULL )
		if ( (server && net_obj->isClient())
			 || (!server && net_obj->isServer()) ) {
			tmp_add = net_obj->t_address;
			tmp_name = net_obj->t_name;
			tmp_port = net_obj->t_port;
		    delete net_obj;
			net_obj = NULL;
		}

	/* create a new net object if it does not already exist */
	if ( net_obj==NULL ) {
		if ( server )
			net_obj = new ServerNetObject(tmp_add, tmp_name, tmp_port);
		else
			net_obj = new ClientNetObject(tmp_add, tmp_name, tmp_port);
		*p_net_obj = net_obj;
	}
	
	showConnect(ASK_ADD);
}


void netDialog::connecting()
{
	/* test and set address & port */
	if ( !net_obj->checkParam( edName->text(), edAdd->text(), edPort->text(),
							   serror) ) {
		ERROR(serror);
		return;
	}
	
	/* update dialog in connecting mode */
	showConnect(CONNECT);
	
	/* in the case of a server_net_object : the connection socket (#0) may
	 * already exist (if != -1) ; in this case it is not necessary to create
	 * it and possibly if the port used is the same, it is not necessary
	 * to bind/listen it. */
	if ( net_obj->isClient() || !net_obj->connectSocketExists() ) {
		/* create the first socket */
		if ( !net_obj->createSocket(serror) ) {
			ERROR(serror);
			showConnect(RE_ASK_ADD);
			return;
		}
	}
	
	if ( net_obj->isClient() || !net_obj->connectSocketExists()
		 || !net_obj->samePort() ) {
		net_obj->setPort();
		/* connect socket (ie connect or listen according to the mode) */
		if ( !net_obj->connectSocket(serror) ) {
			ERROR(serror);
			showConnect(RE_ASK_ADD);
			return;
		}
	}

	/* update dialog in waiting mode */
	showConnect(WAIT_PLAYERS);
	
	/* waiting loop */
	timer->start(NET_TIMEOUT);
}

void netDialog::timeout()
{
	if ( !net_obj->dialogTimeout(serror) ) {
		if ( !serror.isEmpty() ) {
			timer->stop();
			ERROR(serror);
			reject();
		}
	} else {
		if ( net_obj->isClient() )
			accept();
		else
			/* number of clients has changed */
			showConnect(WAIT_PLAYERS);
	}
}


void netDialog::cancel()
{
	if ( state==ASK_ADD )
		showConnect(ASK_MODE);
	else {
		if ( net_obj) 
			net_obj->endClientConnection();
		reject();
	}
}


void netDialog::play()
{
	for (int i=0; i<=net_obj->getNbPlayers(); i++)
		net_obj->deleteLabel(i);
	
	accept();
}
