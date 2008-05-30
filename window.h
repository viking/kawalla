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
#include <kio/netaccess.h>
#include <dcopref.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qptrlist.h> 
#include <qdir.h>
#include <qcstring.h>
#include <qlistbox.h>
#include <Magick++.h>
#include "dbox.h"
using namespace Magick;

struct Photo {
  KURL *url;
  DesktopBox *box;
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
  void updateBoxes(DesktopBox*, const QString&, const QString&);

private:
  void grabPhotos();
  KScrollView *sv;
  QWidget     *central;
  QGridLayout *grid;
  KPushButton *goButton;
  QPtrList<Photo> photos;
  int count;
  int desktops;
  int dwidth;
  int dheight;
  float dratio;
  bool *selected;
};

#endif

#include "handler.h"
