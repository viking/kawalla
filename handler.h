#ifndef __HANDLER_H
#define __HANDLER_H

#include <qxml.h>
#include <math.h>
#include <kapplication.h>
#include "window.h"

class FlickrHandler : public QXmlDefaultHandler
{
public: 
  FlickrHandler( MainWindow *w ); 

  bool startElement( const QString&, const QString&, const QString &, const QXmlAttributes & );

private:
  MainWindow *window;
  int count;
  int dwidth;
  int dheight;
  float dratio;
  float dfloor;
  float dceil;
};

#endif
