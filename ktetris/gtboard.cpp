#include "gtboard.h"

#include <stdlib.h>

/* GTBoard */
GTBoard::GTBoard(  QWidget *p, const char *name )
: Board(TETRIS_TYPE, p, name)
{
	for(int i = 0 ; i < Height ; i++)
		filled[i] = 0;
	
	nLinesRemoved     = 0;
}


void GTBoard::clearBoard()
{
	for(int i = Height - nClearLines - 1 ; i >= 0 ; i--)
		filled[i] = 0;
	GenericTetris::clearBoard();
}


void GTBoard::startGame()
{
	Board::startGame();
	nLinesRemoved      = 0;
    updateRemoved(nLinesRemoved);
}


void GTBoard::internalPieceDropped(int dropHeight)
{
	gluePiece();
	score = score + dropHeight;
	
	removeFullLines();
	
	updateScore(score);
	pieceDropped(dropHeight);
}


void GTBoard::gluePiece()
{
	int x,y;
	int min;
	
	if (currentLine == -1)
		return;
	for(int i = 0 ; i < 4 ; i++) {
		currentPiece.getCoords(i,x,y);
		board[currentPos + x][currentLine - y] = currentPiece.getColor(i);
		filled[currentLine - y]++;
	}
	
	min = currentPiece.getMinY();
	if (currentLine - min >= Height - nClearLines)
		nClearLines = Height - currentLine + min - 1;
}


void GTBoard::removeFullLines()
{
	int i, j, k = 0;
	int nFullLines = 0;
	
	for(i = 0; i < Height - nClearLines ; i++) {
		while (filled[k] == Width) {
			nFullLines++;
			k++;
		}
			
		if (k != i) {
			for (j = 0 ; j < Width ; j++)
				if (board[j][i] != board[j][k]) {
					board[j][i] = board[j][k];
					draw(j, i, board[j][k]);
				}
			filled[i] = filled[k];
		}
		k++;
	}
	
	if (nFullLines) {
		nClearLines   = nClearLines + nFullLines;
		nLinesRemoved = nLinesRemoved + nFullLines;
		updateRemoved(nLinesRemoved);
		
		/* updateScore must be called by caller! */
		/* Assign score according to level and nb of lines (gameboy style) */
		switch (nFullLines) {
		 case 0: break;
		 case 1: score += 40 * level; break;
		 case 2: score += 100 * level; break;
		 case 3: score += 300 * level; break;
		 case 4: score += 1200 * level; break;
		}
		
		/* If we make a multiplum of ten lines, increase level */
		if ((nLinesRemoved / 10) != ((nLinesRemoved-nFullLines)/10)) {
			level++;
			updateLevel(level);
		}
		
		/* Clear the top */
		for (i = Height - nClearLines ;
			 i < Height - nClearLines + nFullLines ; i++) {
			filled[i] = 0;
			for(j = 0 ; j < Width ; j++)
				if (board[j][i] != 0) {
					draw(j,i,0);
					board[j][i] = 0;                
				}
		}
	}
}


void GTBoard::opponentGift(int nb_cases)
{
	int i, j;
	bool reshow_piece = FALSE;
	
	if ( nClearLines==0 ) {
		gameOver();
		return;
	}
	
	/* we must be careful before lifting the lines : a moving piece could
	 * be low enough to be touch by the change */
	if ( currentLine<=(Height-nClearLines+1) ) {
		if ( !canMoveTo(currentPos, currentLine-1) )
			internalPieceDropped(0);
		else 
			reshow_piece = TRUE;
	}
	
	/* lift all the cases of one line */
	for (i=Height-nClearLines; i>=1; i--) {
		for (j=0; j<Width; j++) {
			board[j][i] = board[j][i-1];
			draw(j, i, board[j][i]);
		}
		filled[i] = filled[i-1];
	}

	/* empty the first line */
	for (j=0; j<Width; j++) {
		board[j][0] = 0;
		draw(j, 0, 0);
	}
	
	/* empty the the emptied low line with garbage :) */
	int nb = nb_cases;
	do {
		j = random() % Width;
		if ( board[j][0]==0 ) {
			board[j][0] = 8;
			draw(j, 0, board[j][0]);
			nb--;
		}
	} while ( nb!=0);
	filled[0] = nb_cases;
	nClearLines--;
		
	/* re show piece if necessary */
	if ( reshow_piece )
		showPiece();
}


void GTBoard::checkOpponentGift()
{
	int nb = net_obj->getOpponentGift();
	
	for (int i=2; i<=nb; i++)
		opponentGift(10-i);
}