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
#include <kscrollview.h>
#include <kconfig.h>
#include <kio/netaccess.h>
#include <dcopref.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qptrlist.h> 
#include <qdir.h>
#include <qcstring.h>
#include <qlistbox.h>
#include <qcolor.h>
#include <qcombobox.h>
#include <Magick++.h>
#include "photo.h"
#include "row.h"
#include "handler.h"
using namespace Magick;

class PhotoRow;

class MainWindow : public KMainWindow
{
  Q_OBJECT

public:
  MainWindow( const char *name );
  ~MainWindow();
  void addPhoto( QString &, QString &, QString &, QString &, QString &, int, int, float ); 

private slots:
  void go();
  void next();
  void back();

private:
  void grabPhotos();
  void switchPage();

  KScrollView *sv;
  QWidget     *central;
  QGridLayout *grid;
  QVBox       *vbox;
  KPushButton *goButton;
  KPushButton *nextButton;
  KPushButton *backButton;
  QPtrList<Photo> photos;
  QPtrList<PhotoRow> rows;
  QColor alternateBackground;
  int count;
  int desktops;
  int dwidth;
  int dheight;
  int page;
  float dratio;
};

#endif
