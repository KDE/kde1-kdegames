
#ifndef __AST_VIEW_H__
#define __AST_VIEW_H__

#include <qwidget.h>
#include <qlist.h>
#include <QwSpriteField.h>
#include "sprites.h"

class KAsteroidsView : public QWidget
{
    Q_OBJECT
public:
    KAsteroidsView( QWidget *parent = 0, const char *name = 0 );
    virtual ~KAsteroidsView();

    void reset();
    void setRockSpeed( double rs ) { rockSpeed = rs; }
    void addRocks( int num );
    void newGame();
    void endGame();
    void newShip();

    void rotateLeft( bool r ) { rotateL = r; }
    void rotateRight( bool r ) { rotateR = r; }
    void thrust( bool t ) { thrustShip = t; }
    void shoot() { shootShip = true; }


signals:
    void shipKilled();
    void rockHit( int size );
    void rocksRemoved();

protected:
    void readSprites();
    void processMissiles();
    void processShip();

    virtual void resizeEvent( QResizeEvent *event );
    virtual void timerEvent( QTimerEvent * );

private:
    QwImageSpriteField field;
    QwSpriteFieldView view;
    QList<QwRealMobileSprite> rocks;
    QwSpritePixmapSequence *largeRockImg;
    QwSpritePixmapSequence *mediumRockImg;
    QwSpritePixmapSequence *smallRockImg;
    QList<KMissile> missiles;
    QwSpritePixmapSequence *missileImg;
    QList<KBit> bits;
    QwSpritePixmapSequence *bitImg;
    QwRealMobileSprite *ship;

    bool rotateL;
    bool rotateR;
    bool thrustShip;
    bool shootShip;

    double shipDx;
    double shipDy;

    double rockSpeed;
};

#endif

