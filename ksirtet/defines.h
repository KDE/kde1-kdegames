#ifndef KTETRIS_D_H
#define KTETRIS_D_H

/* Version name and date */
const char SNAME[] = "ktetris 0.2.5 alpha";
const char SDATE[] = "28/05/97";

/* number of highscores */
#define NB_HS    10

#define HS_GRP "High Scores"
#define HS_NAME_KEY "name"
#define HS_SCORE_KEY "score"
#define OP_MENUBAR_VIS "menubar visible"

/* misc defines */
#define NO_TYPE      0
#define TETRIS_TYPE  1
#define GLOBULE_TYPE 2

#define CANCEL 0
#define SERVER 1
#define CLIENT 2

/* Layout */
#define NO_MENU_H 1
#define BOARD_H 333
#define BOARD_W 153
#define TOP_H    10
#define BASE_H   10
#define LCD_H    50
#define LCD_W   100 
#define NTILE_H  94
#define NTILE_W  78
#define SIDE_W   10
#define MID_W    30
#define MID_H    7
#define BAR_W    10

#define LABEL_W  LCD_W
#define LABEL_H  30


#define ADD_LABEL( label, str, x, y, w, h) \
    { label = new QLabel(str, this); \
      label->setGeometry(x, y, w, h); }

#define ADD_BUTTON( button, str, x, y, w, h, slot) \
    { button = new QPushButton(str, this); \
      button->setGeometry(x, y, w, h); \
	  connect( button, SIGNAL(clicked()), SLOT(slot) );}

#define ADD_EDIT( ledit, maxL, x, y, w, h) \
    { ledit = new QLineEdit(this); \
      ledit->setGeometry(x, y, w, h); \
      ledit->setMaxLength(maxL); }

#define ERROR( msg ) \
    { QMessageBox ab; \
	  ab.setCaption("Error"); \
      ab.setText(msg); \
	  ab.setButtonText("Oops"); \
	  ab.show(); }

/* used in network code */
#define MAX_CLIENTS 10
#define MAX_BUFF    200
#define DEFAULT_PORT 5000

#include <sys/types.h>
#include <sys/socket.h> 

extern  int  connect(int  sockfd, struct sockaddr *serv_addr, int addrlen );
extern  int accept(int s, struct sockaddr *addr, int *addrlen);

#endif
