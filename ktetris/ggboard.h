#ifndef KTETRIS_GG_H
#define KTETRIS_GG_H 

#include "board.h"


/* this class has the gestion of removing group of same-colored groups
 * it is used for "globule game" */
class GGBoard : public Board
{
 public:
	GGBoard( QWidget *parent=0, const char *name=0 );
	
 protected:
	void internalOneLineDown();
	void internalPieceDropped(int dropHeight);
	
 private:
	void defragmentBoard();
	void gluePiece();
	void removeColoredAreas();
	
	int fragmentation;
};

#endif