#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <kapplication.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <kio/netaccess.h>
#include "handler.h"

class MainWindow : public KMainWindow
{
  Q_OBJECT

public:
  MainWindow( const char *name );

private:
  void grabPhotos();

  QGrid *grid;
};

#endif
