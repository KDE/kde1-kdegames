

#ifndef __KASTMSG_H__
#define __KASTMSG_H__

#include <qwidget.h>


class KAstMsg : public QWidget
{
public:
    KAstMsg( QWidget *parent, const char *name = 0 );

    void setMessage( const char *m );

protected:
    virtual void paintEvent( QPaintEvent * );

private:
    QString msg;
};

#endif

