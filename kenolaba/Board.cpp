/* Class Board
 * 
 *   with methods for
 *    - play/take back moves
 *    - generate allowed moves
 *    - calculate rating for position
 *    - search for best move 
 * 
 * Josef Weidendorfer, 28.8.97
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include "Board.h"
#if 1
#define CHECK(b)  if (!b) { m.print(); print(); ASSERT(b); }
#else
#define CHECK(b)
#endif


/*********************** Class MainCombination *************************/

void MainCombination::clear(int d)
{
	int i,j;
	
	for(i=0;i<maxDepth;i++)
	  for(j=0;j<maxDepth;j++) {
		  move[i][j].type = Move::none;
	  }	
	actMaxDepth = (d<maxDepth) ? d:maxDepth-1;
}

void MainCombination::update(int d, Move& m)
{
	int i;

	if (d>actMaxDepth) return;
	for(i=d+1;i<=actMaxDepth;i++) {
		move[d][i]=move[d+1][i];
		move[d+1][i].type = Move::none;
	}
	move[d][d]=m;
}



/****************************** Class Board ****************************/


/* Board for start of a game */
int Board::startBoard[]={
                       10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	             10,  1,  1,  1,  1,  1, 10, 10, 10, 10, 10,
	           10,  1,  1,  1,  1,  1,  1, 10, 10, 10, 10,
                 10,  0,  0,  1,  1,  1,  0,  0, 10, 10, 10,
	       10,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10,
             10,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,
           10, 10,  0,  0,  0,  0,  0,  0,  0,  0, 10,
         10, 10, 10,  0,  0,  2,  2,  2,  0,  0, 10,
       10, 10, 10, 10,  2,  2,  2,  2,  2,  2, 10,
     10, 10, 10, 10, 10,  2,  2,  2,  2,  2, 10,
   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10          };


/* first centrum of board, then rings around (numbers are indexes) */
int Board::order[]={
          60,
	  61,72,71,59,48,49,
	  62,73,84,83,82,70,58,47,36,37,38,50,
	  63,74,85,96,95,94,93,81,69,57,46,35,24,25,26,27,39,51,
	  64,75,86,97,108,107,106,105,104,92,80,68,56,45,34,23,12,
	  13,14,15,16,28,40,52 };

/* ratings for fields: central room has highest ratings... */
int Board::fieldValue[] = {
	  50,
	  38,36,39,37,38,39,
	  34,32,30,35,34,32,34,36,33,31,35,30,
	  28,29,21,27,29,29,27,20,29,28,29,29,28,20,29,27,29,20,
	  0,2,3,1,1,5,3,1,0,2,4,2,0,1,5,2,0,3,4,1,0,2,2,1 
};

/* Value added to board rating according to the difference of 
 * stones in game of player1 and player2 */
int Board::colorValues[]= {
		0,-800,-1800,-3000,-4400,-6000 };

int Board::direction[]= {
		-11,1,12,11,-1,-12,-11,1 };

/* Values added to board rating for content of neighbored fields
 * e.g. (color1,color1) gives 3, (free,color2) gives -1 */
int Board::clusterValue[]= {
		0,1,-1,1,8,0,-1,0,-8 };


Board::Board()
{
	clear();
	breakOut = false;
	boardOk = true;
	debug = 0;
	realMaxDepth = 1;
}


void Board::begin(int startColor)
{
	int i;
	
	for(i=0;i<AllFields;i++)
	  field[i] = startBoard[i];
	storedFirst = storedLast = 0;
	color = startColor;
	color1Count = color2Count = 14;
	boardOk = true;
}

void Board::clear()
{
	int i;
	
	for(i=0;i<AllFields;i++)
	  field[i] = (startBoard[i] == out) ? out: free;
	storedFirst = storedLast = 0;
	color1Count = color2Count = 0;
	boardOk = true;
}

/* generate moves starting at field <startField> */
inline int Board::generateFieldMoves(int startField, MoveList& list)
{
	int value = 0;
	int d, dir, c, actField, left, right;
	int opponent = (color == color1) ? color2 : color1;

	ASSERT( field[startField] == color );
	
	/* 6 directions	*/
	for(d=1;d<7;d++) {
		dir = direction[d];
		  
		/* 2nd field */
		c = field[actField = startField+dir];
		if (c == free) {			
			/* (c .) */
			list.insert(startField, d, Move::move1);
			continue;
		}		
		if (c != color) 
		  continue;
		
		/* 2nd == color */
		
		left = (field[startField+direction[d-1]] == free);
		if (left) {
			left = (field[actField+direction[d-1]] == free);
			if (left) 
			  /* 2 left */
			  list.insert(startField, d, Move::left2);
		}
		
		right = (field[startField+direction[d+1]] == free);
		if (right) {
			right = (field[actField+direction[d+1]] == free);
			if (right) 
			  /* 2 right */
			  list.insert(startField, d, Move::right2);
		}
		
		/* 3rd field */
		c = field[actField += dir];
		if (c == free) {
			/* (c c .) */
			list.insert(startField, d, Move::move2);
			continue;
		}		
		else if (c == opponent) {
			
			/* 4th field */
			c = field[actField += dir];
			if (c == free) {
				/* (c c o .) */
				list.insert(startField, d, Move::push1with2);
				value++;
			}
			else if (c == out) {
				/* (c c o |) */
				list.insert(startField, d, Move::out1with2);
				value+=6;
			}
			continue;
		}
		if (c != color)
		  continue;
		
		/* 3nd == color */

		if (left) {
			if (field[actField+direction[d-1]] == free) 
			  /* 3 left */
			  list.insert(startField, d, Move::left3);
		}

		if (right) {
			if (field[actField+direction[d+1]] == free)
			  /* 3 right */
			  list.insert(startField, d, Move::right3);
		}
		
		/* 4th field */
		c = field[actField += dir];
		if (c == free) {
			/* (c c c .) */
			list.insert(startField, d, Move::move3);
			value++;
			continue;
		}
		if (c != opponent)
		  continue;

		/* 4nd == opponent */
		
		/* 5. field */
		c = field[actField += dir];
		if (c == free) {
			/* (c c c o .) */
			list.insert(startField, d, Move::push1with3);
			value+=2;
			continue;
		}
		else if (c == out) {
			/* (c c c o |) */
			list.insert(startField, d, Move::out1with3);
			value+=6;
			continue;
		}
		if (c != opponent)
		  continue;

		/* 5nd == opponent */

		/* 6. field */
		c = field[actField += dir];
		if (c == free) {
			/* (c c c o o .) */
			list.insert(startField, d, Move::push2);
			value+=3;
		}
		else if (c == out) {
			/* (c c c o o |) */
			list.insert(startField, d, Move::out2);
			value+=7;
		}
	}
	return value;
}


int Board::generateMoves(MoveList& list)
{
	int value=0;
	int actField, f;

	ASSERT( boardOk );

	for(f=0;f<RealFields;f++) {
		actField = order[f];
		if ( field[actField] == color)
		   value += generateFieldMoves(actField, list);
	}
	return value;
}


void Board::playMove(const Move& m)
{
	int f, dir, dir2;
	int opponent = (color == color1) ? color2:color1;

	CHECK( boardOk );
	CHECK( isConsistent() );
	
	if (++storedLast == MvsStored) storedLast = 0;
	
	/* Buffer full -> delete oldest entry */
	if (storedLast == storedFirst)
	  	if (++storedFirst == MvsStored) storedFirst = 0;
	
	storedMove[storedLast] = m;

	f = m.field;
	CHECK( (m.type >= 0) && (m.type < Move::none));
	CHECK( field[f] == color );
	field[f] = free;
	dir = direction[m.direction];
	
	switch(m.type) {
	 case Move::out2:        /* (c c c o o |) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == opponent );
		CHECK( field[f + 4*dir] == opponent );
		CHECK( field[f + 5*dir] == out );
		field[f + 3*dir] = color;
		break;		
	 case Move::out1with3:   /* (c c c o |)   */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == opponent );
		CHECK( field[f + 4*dir] == out );
		field[f + 3*dir] = color;
		break;
	 case Move::move3:       /* (c c c .)     */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == free );
		field[f + 3*dir] = color;
		break;
	 case Move::out1with2:   /* (c c o |)     */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == opponent );
		CHECK( field[f + 3*dir] == out );
		field[f + 2*dir] = color;
		break;
	 case Move::move2:       /* (c c .)       */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == free );
		field[f + 2*dir] = color;
		break;
	 case Move::push2:       /* (c c c o o .) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == opponent );
		CHECK( field[f + 4*dir] == opponent );
		CHECK( field[f + 5*dir] == free );
		field[f + 3*dir] = color;
		field[f + 5*dir] = opponent;
		break;
	 case Move::left3:		
		dir2 = direction[m.direction-1];
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + dir2] == free );
		CHECK( field[f + dir+dir2] == free );
		CHECK( field[f + 2*dir+dir2] == free );
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		break;
	 case Move::right3:
		dir2 = direction[m.direction+1];
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + dir2] == free );
		CHECK( field[f + dir+dir2] == free );
		CHECK( field[f + 2*dir+dir2] == free );
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		break;
	 case Move::push1with3:   /* (c c c o .) => (. c c c o) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == opponent );
		CHECK( field[f + 4*dir] == free );
		field[f + 3*dir] = color;
		field[f + 4*dir] = opponent;
		break;
	 case Move::push1with2:   /* (c c o .) => (. c c o) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == opponent );
		CHECK( field[f + 3*dir] == free );
		field[f + 2*dir] = color;
		field[f + 3*dir] = opponent;
		break;
	 case Move::left2:
		dir2 = direction[m.direction-1];
		CHECK( field[f + dir] == color );
		CHECK( field[f + dir2] == free );
		CHECK( field[f + dir+dir2] == free );
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		break;
	 case Move::right2:
		dir2 = direction[m.direction+1];
		CHECK( field[f + dir] == color );
		CHECK( field[f + dir2] == free );
		CHECK( field[f + dir+dir2] == free );
		field[f+dir2] = color;
		field[f+=dir] = free;
		field[f+dir2] = color;
		break;
	 case Move::move1:       /* (c .) => (. c) */
		CHECK( field[f + dir] == free );
		field[f + dir] = color;
		break;
	}

	if (m.isOutMove()) {
		if (color == color1)
		  color2Count--;
		else
		  color1Count--;
	}       
	
	/* change actual color */
	color = opponent;
	
	CHECK( isConsistent() );
	
}

void Board::takeBack()
{
	int f, dir, dir2;
	int opponent = color;
	Move& m = storedMove[storedLast];

	CHECK( boardOk );
	CHECK( isConsistent() );
	
	if (storedFirst == storedLast) return;

	/* change actual color */
	color = (color == color1) ? color2:color1;	

	if (m.isOutMove()) {
		if (color == color1)
		  color2Count++;
		else
		  color1Count++;
	}       
	
	f = m.field;
	CHECK( field[f] == free );
	field[f] = color;
	dir = direction[m.direction];
	
	switch(m.type) {
	 case Move::out2:        /* (. c c c o |) => (c c c o o |) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == color );
		CHECK( field[f + 4*dir] == opponent );
		CHECK( field[f + 5*dir] == out );
		field[f + 3*dir] = opponent;
	 case Move::out1with3:   /* (. c c c |) => (c c c o |) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == color );
		CHECK( field[f + 4*dir] == out );
		field[f + 3*dir] = opponent;
		break;
	 case Move::move3:       /* (. c c c) => (c c c .)     */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == color );
		field[f + 3*dir] = free;
		break;
	 case Move::out1with2:   /* (. c c | ) => (c c o |)     */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == out );
		field[f + 2*dir] = opponent;
		break;
	 case Move::move2:       /* (. c c) => (c c .)       */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		field[f + 2*dir] = free;
		break;
	 case Move::push2:       /* (. c c c o o) => (c c c o o .) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == color );
		CHECK( field[f + 4*dir] == opponent );
		CHECK( field[f + 5*dir] == opponent );
		field[f + 3*dir] = opponent;
		field[f + 5*dir] = free;
		break;
	 case Move::left3:
		dir2 = direction[m.direction-1];
		CHECK( field[f + dir] == free );
		CHECK( field[f + 2*dir] == free );
		CHECK( field[f + dir2] == color );
		CHECK( field[f + dir+dir2] == color );
		CHECK( field[f + 2*dir+dir2] == color );
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		break;
	 case Move::right3:
		dir2 = direction[m.direction+1];
		CHECK( field[f + dir] == free );
		CHECK( field[f + 2*dir] == free );
		CHECK( field[f + dir2] == color );
		CHECK( field[f + dir+dir2] == color );
		CHECK( field[f + 2*dir+dir2] == color );
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		break;
	 case Move::push1with3:   /* (. c c c o) => (c c c o .) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == color );
		CHECK( field[f + 4*dir] == opponent );
		field[f + 3*dir] = opponent;
		field[f + 4*dir] = free;
		break;
	 case Move::push1with2:   /* (. c c o) => (c c o .) */
		CHECK( field[f + dir] == color );
		CHECK( field[f + 2*dir] == color );
		CHECK( field[f + 3*dir] == opponent );
		field[f + 2*dir] = opponent;
		field[f + 3*dir] = free;
		break;
	 case Move::left2:
		dir2 = direction[m.direction-1];
		CHECK( field[f + dir] == free );
		CHECK( field[f + dir2] == color );
		CHECK( field[f + dir+dir2] == color );
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		break;
	 case Move::right2:
		dir2 = direction[m.direction+1];
		CHECK( field[f + dir] == free );
		CHECK( field[f + dir2] == color );
		CHECK( field[f + dir+dir2] == color );
		field[f+dir2] = free;
		field[f+=dir] = color;
		field[f+dir2] = free;
		break;
	 case Move::move1:       /* (. c) => (c .) */
		CHECK( field[f + dir] == color );
		field[f + dir] = free;
		break;
	}

	if (--storedLast < 0) storedLast = MvsStored-1;
	
	CHECK( isConsistent() );
}

void Board::showMove(const Move& m, int step)
{
	int f, dir, dir2;
	int opponentNew, colorNew;
	bool afterMove;

	if (boardOk) {
	  /* board ok means: board has the normal state before move */
	  CHECK( isConsistent() );
	  if (step == 0)
	    return;      /* nothing to be done */
	}
	boardOk = (step == 0) ? true:false;

	if (color == color1) {
	  colorNew = (step<2) ? color1 : ((step>2) ? color1bright:free );
	  opponentNew = (step<2) ? color2 : color2bright;
	}
	else {
	  colorNew = (step<2) ? color2 : ((step>2) ? color2bright:free );
	  opponentNew = (step<2) ? color1 : color1bright;
	}

	afterMove = (step == 1) || (step == 4);

	f = m.field;
	CHECK( (m.type >= 0) && (m.type < Move::none));
	dir = direction[m.direction];

	/* first field */
	field[f] = afterMove ? free : colorNew;

	switch(m.type) {
	case Move::out2:        /* (c c c o o |) */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  field[f + 3*dir] = afterMove ? colorNew : opponentNew;
	  field[f + 4*dir] = opponentNew;
	  break;		
	case Move::out1with3:   /* (c c c o |)   */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  field[f + 3*dir] = afterMove ? colorNew : opponentNew;
	  break;
	case Move::move3:       /* (c c c .)     */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  field[f + 3*dir] = afterMove ? colorNew : free;
	  break;
	case Move::out1with2:   /* (c c o |)     */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = afterMove ? colorNew : opponentNew;
	  break;
	case Move::move2:       /* (c c .)       */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = afterMove ? colorNew : free;
	  break;
	case Move::push2:       /* (c c c o o .) */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  field[f + 3*dir] = afterMove ? colorNew : opponentNew;
	  field[f + 4*dir] = opponentNew;
	  field[f + 5*dir] = afterMove ? opponentNew : free;
	  break;
	case Move::left3:		
	  dir2 = direction[m.direction-1];
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  break;
	case Move::right3:
	  dir2 = direction[m.direction+1];
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  break;
	case Move::push1with3:   /* (c c c o .) => (. c c c o) */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  field[f + 3*dir] = afterMove ? colorNew : opponentNew;
	  field[f + 4*dir] = afterMove ? opponentNew : free;
	  break;
	case Move::push1with2:   /* (c c o .) => (. c c o) */
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = afterMove ? colorNew : opponentNew;
	  field[f + 3*dir] = afterMove ? opponentNew : free;
	  break;
	case Move::left2:
	  dir2 = direction[m.direction-1];
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  break;
	case Move::right2:
	  dir2 = direction[m.direction+1];
	  field[f+dir2] = afterMove ? colorNew : free;
	  field[f+=dir] = afterMove ? free : colorNew;
	  field[f+dir2] = afterMove ? colorNew : free;
	  break;
	case Move::move1:       /* (c .) => (. c) */
	  field[f + dir] = afterMove ? colorNew : free;
	  break;
	}
}

void Board::showStart(const Move& m, int step)
{
	int f, dir;
	int colorNew;

	if (boardOk) {
	  /* board ok means: board has the normal state before move */
	  CHECK( isConsistent() );
	  if (step == 0)
	    return;      /* nothing to be done */
	}
	boardOk = (step == 0) ? true:false;

	if (color == color1)
	  colorNew = (step==0) ? color1 : color1bright;
	else 
	  colorNew = (step==0) ? color2 : color2bright;

	f = m.field;

	/* first field */
	field[f] = colorNew;

	switch(m.type) {
	case Move::left3:
	case Move::right3:
	  dir = direction[m.direction];
	  field[f + dir] = colorNew;
	  field[f + 2*dir] = colorNew;
	  break;
	case Move::left2:
	case Move::right2:
	  dir = direction[m.direction];
	  field[f + dir] = colorNew;
	}
}


int Board::calcValue()
{
        int value=0;
	int i,j,k=0;
	
	for(i=0;i<RealFields;i++) {		  
		j=field[order[i]];
		if (j == color1) value += fieldValue[i];
		if (j == color2) value -= fieldValue[i];
		value += clusterValue[k*3+j]; k=j;
	}
	return(value + colorValues[14 - color1Count] - 
	               colorValues[14 - color2Count]);
}

bool Board::isConsistent()
{
        int c1 = 0, c2 = 0;
	int i,j;
	
	for(i=0;i<RealFields;i++) {		  
		j=field[order[i]];
		if (j == color1) c1++;
		if (j == color2) c2++;
	}
	return (color1Count == c1 && color2Count == c2); 
}

void Board::changeRating()
{
	int i,tmp;
	
	/* innermost ring */
	tmp=fieldValue[1];
	for(i=1;i<6;i++)
	  fieldValue[i] = fieldValue[i+1];
	fieldValue[6] = tmp;
	
	tmp=fieldValue[7];
	for(i=7;i<18;i++)
	  fieldValue[i] = fieldValue[i+1];
	fieldValue[18] = tmp;
	
	tmp=fieldValue[19];
	for(i=19;i<36;i++)
	  fieldValue[i] = fieldValue[i+1];
	fieldValue[36] = tmp;

	/* the outermost ring */
	tmp=fieldValue[37];
	for(i=37;i<61;i++)
	  fieldValue[i] = fieldValue[i+1];
	fieldValue[61] = tmp;
}

/*
void Board::showHist()
{	
	Move m1, m2;
	
	printf("After playing ");
	(m1=lastMove()).print();
	print();
	printf("TakeBack "); m1.print();
	takeBack(); print();
	printf("TakeBack ");
	(m2=lastMove()).print();
	takeBack(); print();
	printf("Play "); m2.print();
	playMove(m2); print();
	printf("Play "); m1.print();
	playMove(m1); print();
	getchar();
}


void indent(int d)
{
	char tmp[]="                                  ";
	tmp[d*3] = 0;
	printf("%s",tmp);
}
*/

int Board::search1(int depth, int alpha, int beta)
{
	int actValue=-16000, value, moveValue;
	Move m;
	MoveList list;
	bool stop = false;

	/* We make a depth search for the following move types... */
	int maxType = (depth < maxDepth/2) ? Move::maxMoveType() :
	              (depth < maxDepth)   ? Move::maxPushType() : 
                                             Move::maxOutType();

	moveValue = generateMoves(list);

	// if (debug >2) printf(">>>>>>>> Depth %d\n", depth);
	
	/* check for a old best move in main combination */
	if (inMainCombination) {
		m = mc[depth];
		if (!list.isElement(m, 0)) 
		  m.type = Move::none;
		if (m.type == Move::none)
		  inMainCombination = false;
		if (m.type > maxType)
		  m.type = Move::none;
	}
	if (m.type == Move::none)
	  stop = !list.getNext(m, maxType);

	/* depth first search */
	while(!stop) {
/*
		if (debug>0) {
			indent(depth);
			m.print();
		}
*/				
		playMove(m);
		if (!isValid())
		  value = ((depth < maxDepth) ? 15999:14999) - depth;
		else
		  value = search2(depth+1,alpha,beta);
		takeBack();

/*		
		if (debug> 0) {
			indent(depth);
			printf("=> (%d - %d): Value %d [ %d ] for ",
			       alpha, beta, value,actValue);
			m.print();
		}		
*/

		if (value > actValue) {
			actValue = value;
			mc.update(depth, m);
			
			if (maxDepth - depth >3)
			  emit searchBreak();		
			
			if (actValue >= beta) {
/*
				if (debug>0) {
					indent(depth);
					printf("CUTOFF\n");
				}				
*/
				return actValue;
			}			
			if (actValue > alpha) alpha = actValue;
		}
		
		stop = (!list.getNext(m, maxType)) || breakOut;
	}
	
	/* other moves: calculate rating */
	while(list.getNext(m, Move::none)) {
		
		playMove(m);
		if (!isValid())
		  value = ((depth < maxDepth) ? 15999:14999) - depth;
		else
		  value = calcValue();
		takeBack();
/*
		if (debug> 0) {
			indent(depth);
			printf("> (%d - %d): Value %d [ %d ] for ",
			       alpha, beta, value, actValue);
			m.print();
		}
*/		
		if (value > actValue) {
			actValue = value;
			mc.update(depth, m);
			
			if (actValue >= beta) {
/*
				if (debug>0) {
					indent(depth);
					printf("CUTOFF\n");
				}				
*/
				break;
			}
			
			if (actValue > alpha) alpha = actValue;
		}
	}	
	return actValue;
}


int Board::search2(int depth, int alpha, int beta)
{
	int actValue=16000, value, moveValue;
	Move m;
	MoveList list;
	bool stop = false;

	/* We make a depth search for the following move types... */
	int maxType = (depth < maxDepth/2) ? Move::maxMoveType() :
	              (depth < maxDepth)   ? Move::maxPushType() : 
                                             Move::maxOutType();

	moveValue = generateMoves(list);

//	if (debug>2) printf(">>>>>>>> Depth %d\n", depth);

	/* check for a old best move in main combination */
	if (inMainCombination) {
		m = mc[depth];
		if (!list.isElement(m,0)) 
		  m.type = Move::none;
		if (m.type == Move::none)
		  inMainCombination = false;
		if (m.type > maxType)
		  m.type = Move::none;
	}
	if (m.type == Move::none)
	  stop = !list.getNext(m, maxType);

	/* depth first search */
	while(!stop) {
/*
		if (debug>0) {
			indent(depth);
			m.print();
		}
*/		
		playMove(m);
		if (!isValid())
		  value = depth - ((depth < maxDepth) ? 15999:14999);
		else
		  value = search1(depth+1,alpha,beta);
		takeBack();

/*
		if (debug> 0) {
			indent(depth);
			printf("=> (%d - %d): Value %d [ %d ] for ",
			       alpha, beta, value, actValue);
			m.print();
		}
*/
		
		if (value < actValue) {
			actValue = value;
			mc.update(depth, m);
			if (maxDepth - depth >3) 
			  emit searchBreak();		
			
			if (actValue <= alpha) {
/*
				if (debug>0) {
					indent(depth);
					printf("CUTOFF\n");
				}				
*/
				return actValue;
			}			
			if (actValue < beta) beta = actValue;
		}		
		
		stop = (!list.getNext(m, maxType)) || breakOut;
	}
	
	/* other moves: calculate rating */
	while(list.getNext(m, Move::none)) {
		
		playMove(m);
		if (!isValid())
		  value = depth - ((depth < maxDepth) ? 15999:14999);
		else
		  value = calcValue();
		takeBack();

/*
		if (debug> 0) {
			indent(depth);
			printf("> (%d - %d): Value %d [ %d ] for ",
			       alpha, beta, value, actValue);
			m.print();
		}
*/
		
		if (value < actValue) {
			actValue = value;
			mc.update(depth, m);
			
			if (actValue <= alpha) {
/*
				if (debug>0) {
					indent(depth);
					printf("CUTOFF\n");
				}
*/
				break;
			}
			if (actValue < beta) beta = actValue;
		}		
	}	
	return actValue;
}


Move& Board::bestMove()
{
	int alpha=-15000,beta=15000;
	int nalpha,nbeta, actValue;
	
	mc.clear(realMaxDepth);
	
	maxDepth=1;
	show = false;
	breakOut = false;

//	if (debug) printf(">>>>>>>>>>>>>>>>>> New Search\n");
	do {	       
//		if (debug) printf(">> MaxDepth: %d\n", maxDepth);
		// ShowTiefe(maxtiefe);
		do {			
//			if (debug) printf(">> Alpha/Beta: (%d ... %d)\n", alpha, beta);
			nalpha=alpha, nbeta=beta;
			inMainCombination = (mc[0].type != Move::none);
			actValue = (color == color1) ?
			  search1(0,alpha,beta) : search2(0,alpha,beta);
			
			/* Don't break out if we haven't found a move */
			if (mc[0].type == Move::none) 
			  breakOut=false;
			if (actValue > 15000 || actValue < -15000)
			  breakOut=true;
			if (actValue <= nalpha)
			  alpha = -15000, beta=actValue+1;
			if (actValue >= nbeta)
			  alpha = actValue-1, beta=15000;
		}
		while(!breakOut && (actValue<=nalpha || actValue>=nbeta));
		if ( (maxDepth+((color == color2)?1:0)) %2
		      ==1)
		  alpha=actValue-200, beta=actValue+1;
		else 
		  alpha=actValue-1, beta=actValue+200;
		// ShowHv2();
	}	
	while(++maxDepth< realMaxDepth && !breakOut);
	
	changeRating();
	
	return(mc[0]);
}	  

Move Board::randomMove()
{
	static int i = 999;
	unsigned int j,l;
	struct timeval tv;

	Move m;
	MoveList list;

	gettimeofday(&tv,0);
	j = tv.tv_sec * 1000000 + tv.tv_usec;
	
	generateMoves(list);
	l = list.getLength();

	j = (j % i) % l +1;
	if ( (i+=7)>10000) i-=10000;
	
	while(j != 0) {
		list.getNext(m, Move::none);
		j--;
	}	
	
	return m;
}
	

void Board::print()
{
/*
	int row,i;
	char spaces[]="      ";
	char *z[]={". ","O ","X ", "o ", "x "};

	printf("\n       -----------\n");
	for(row=0;row<4;row++) {
		printf("%s/ ",spaces+row);
		for(i=0;i<5+row;i++) printf("%s",z[field[row*11+12+i]]);
		printf("\\\n");
	}
	printf("  | ");
	for(i=0;i<9;i++) printf("%s",z[field[56+i]]);
	printf("|\n");
	for(row=0;row<4;row++) {
		printf("%s\\ ",spaces+3-row);
		for(i=0;i<8-row;i++) printf("%s",z[field[68+row*12+i]]);
		printf("/\n");
	}
	printf("       -----------     O: %d  X: %d\n",
	       color1Count, color2Count);	
*/
}

#include "Board.moc"

