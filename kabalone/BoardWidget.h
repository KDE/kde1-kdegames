#ifndef _BOARDWIDGET_H_
#define _BOARDWIDGET_H_

#include "Move.h"
#include <qwidget.h>

class Board;

class BoardWidget : public QWidget
{
	Q_OBJECT
	
 public:
	BoardWidget(Board&, QWidget *parent = 0, const char *name = 0);
	~BoardWidget();

	virtual void resizeEvent(QResizeEvent *e);
	virtual void paintEvent(QPaintEvent *e);
	virtual void mousePressEvent( QMouseEvent* pEvent );
	virtual void mouseReleaseEvent( QMouseEvent* pEvent );
	virtual void mouseMoveEvent( QMouseEvent* pEvent );

	void draw();
	void drawBoard();
	
	void choseMove(MoveList*);

 signals:
	void moveChoosen(Move&);
	
 private:
	int positionOf(int x, int y);
	bool isValidStart(int pos, bool);
	bool isValidEnd(int pos);

	QPixmap pm, boardPM;
	Board& board;
	
	/* for getting user Move */
	MoveList *pList;
	Move actMove;
	bool gettingMove, mbDown, startValid, startShown;
	int startPos, actPos, oldPos, shownDirection;
	int startField, startField2, actField, oldField, startType;
	QColor *boardColor, *redColor, *yellowColor, *redHColor, *yellowHColor;
	QCursor *arrowAll, *arrow[7];	
};

#endif /* _BOARDWIDGET_H_ */

