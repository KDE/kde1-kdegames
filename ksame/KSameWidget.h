/*
 *   ksame 0.2 - simple Game
 *   Copyright (C) 1997  Marcus Kreutzberger
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *    
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _KSAMEWIDGET
#define _KSAMEWIDGET
#include <qlined.h> 
#include <qwidget.h>
#include <kbutton.h>
#include <qpushbt.h>
#include <qlcdnum.h>
#include "StoneWidget.h"
#include "ScoreWidget.h"
#include <ktopwidget.h> 
#include <kmenubar.h>

class KSameWidget: public KTopLevelWidget {
 Q_OBJECT
public:
    KSameWidget(); 
    ~KSameWidget();
 
private:
    StoneWidget *stone;
    ScoreWidget *hiscore;
    KStatusBar *status;

public slots: 
  /* File Menu */
    void m_new();
    void m_load();
    void m_save();
    void m_quit();
    
    /* Options Menu */
    void m_colors3();
    void m_colors4();
    void m_colors5();
    void m_colors6();
    void m_tglboard();
    void m_tglmultispin();
  /* Help Menu */
    void m_help();
    void m_about(); 
    
    void gameover(int);
    void updatescore(int);
    void updateboard(int);
    void updatecolors(int);
  

};



#endif
