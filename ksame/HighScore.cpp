#include <stdio.h>
#include <qbutton.h> 
#include <qtabdlg.h>
#include "HighScore.moc"
#include "ScoreWidget.h"

 HighScore::HighScore( QWidget *parent ) : QTabDialog(parent) {

     addTab(new ScoreWidget(this),"&Two Colors");
     addTab(new ScoreWidget(this),"Th&ree Colors");
     addTab(new ScoreWidget(this),"&Four Colors");
 } 
