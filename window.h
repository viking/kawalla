#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <kapplication.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <kwin.h>
#include <kio/netaccess.h>
#include <dcopref.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qptrlist.h> 
#include <qdir.h>
#include <qcstring.h>
#include <Magick++.h>
using namespace Magick;

struct Photo {
  KURL *url;
  QLineEdit *box;
  int width;
  int height;
  float ratio;
};

class MainWindow : public KMainWindow
{
  Q_OBJECT

public:
  MainWindow( const char *name );
  ~MainWindow();
  void addFlickr( QString &, QString &, QString &, QString &, int, int, float ); 

private slots:
  void go();

private:
  void grabPhotos();
  QWidget     *central;
  QGridLayout *grid;
  KPushButton *goButton;
  QPtrList<Photo> photos;
  int count;
  int desktops;
  int dwidth;
  int dheight;
  float dratio;
};

#endif

#include "handler.h"
