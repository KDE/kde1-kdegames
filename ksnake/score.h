#ifndef SCORE_H
#define SCORE_H

#include <qdialog.h>
#include <qdatetm.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qgrpbox.h>
#include <qlined.h>
#include <qstring.h>
#include <qfile.h>

class ScoreBox : public QWidget
{
    Q_OBJECT
public:
    ScoreBox( QWidget *parent=0, const char *name=0 );

    void setTitle( const char *);
    void setScore(int, int, const char *);
protected:
    void resizeEvent( QResizeEvent * );
private:
    void init();

    QGroupBox   *box;
    QLabel      *label;

    QLabel *points[5];
    QLabel *player[5];
};

class Score : public QObject
{
    Q_OBJECT
public:
    Score( QWidget *parent=0, const char *name=0 );
public slots:
    void setScore(int);
    void display();
private:

    struct {
	int  points;
	char *player;
    } hall[5];

    struct {
	QDate date;
	int  points;
	char *player;
    } today[5];


    void read();
    void write();
    QString getPlayerName();
    QString playerName;
    QFile file;
};


#endif // SCORE_H
