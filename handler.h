#ifndef __HANDLER_H
#define __HANDLER_H

#include <qxml.h>
#include <qgrid.h>
#include <kapplication.h>
#include <math.h>

class FlickrHandler : public QXmlDefaultHandler
{
public: 
  FlickrHandler( QGrid *g ); 

  bool startElement( const QString&, const QString&, const QString &, const QXmlAttributes & );

private:
  QGrid *grid;
  int dwidth;
  int dheight;
  float dratio;
  float dfloor;
  float dceil;
};

#endif
