#ifndef KTETRIS_GT_H
#define KTETRIS_GT_H 

#include "board.h"

#define NB_CASES_RANDOM_LINE 8

/* this class has the gestion of removing full lines :
 * it is used for "normal tetris" */
class GTBoard : public Board
{
 public:
	GTBoard( QWidget *parent=0, const char *name=0);
	
	int  getLinesRemoved() 
		{ return nLinesRemoved; }
	
	void clearBoard();
	void startGame();
	void checkOpponentGift();

 protected:
	void  internalPieceDropped(int dropHeight);
	
 private:
	int  nLinesRemoved;
	int  filled[Height]; /* how many columns are filled? */
	
	void gluePiece();
	void removeFullLines();
	void opponentGift(int nb_cases);
}; 

#endif
